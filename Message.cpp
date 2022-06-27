#include "Message.h"
#include "DBManager.h"
#include <iostream>
#include "Channel.h"
#include <json/json.h>
#include "Tool.h"

void RegisterRequestMessage::process(std::shared_ptr<Channel> channel)
{ //process register requests

   char buff[512];

   snprintf(buff,sizeof(buff),\
   "select LoginName from IMUser where LoginName =\'%s\' ",msg_info["Name"].asCString());

   std::string sql =buff;
   std::string key ="LoginName";

   Json::Value reply;

   DBQueryResult result= DBManager::GetDBManager()->Query(key,sql);

   if(result.GetRowCount()==0)
   {

      snprintf(buff, sizeof(buff),
               "insert into IMUser(LoginName,NickName,UserPassword,CreateTime) values(\'%s\',\'%s\',\'%s\',Now())",
               msg_info["Name"].asCString(), msg_info["NickName"].asCString(), msg_info["Password"].asCString());

      std::string insert_sql = buff;


      int ret = DBManager::GetDBManager()->Update(insert_sql);
      if (ret > 0)
      { // insert sucessful
         reply["State"] = "Success";
         reply["ErrorCode"] = "0";
      }
      else
      {
         reply["State"] = "Failed";
         reply["ErrorCode"] = "1";
      }
   }
   else
   {//loginname eixts
      reply["State"] = "Failed";
      reply["ErrorCode"] = "2";
   }

   //rely to client
   //0 success 1 exits 2 failed
   
   RegisterReplyMessage msg_reply(reply);

   channel->WriteMessagePackage(msg_reply);
   /*unsigned long long buffer_length = msg_reply.msg_head.length;

   std::cout<<reply.toStyledString()<<msg_reply.msg_head.length<<std::endl;
   msg_reply.Serialize();

   
   char* buff_ptr = new char[buffer_length];
   memcpy(buff_ptr,&msg_reply.msg_head,sizeof(msg_reply.msg_head));
   memcpy((buff_ptr+sizeof(msg_reply.msg_head)), reply.toStyledString().c_str(), reply.toStyledString().size());
   channel->WriteBuffer(buff_ptr,buffer_length);

   delete []buff_ptr;*/
}


void LoginRequestMessage::process(std::shared_ptr<Channel> channel)
{
   char buff[512];

   snprintf(buff,sizeof(buff),
   "select ItemID,LoginName,NickName,UserPassword as Password from IMUser where LoginName =\'%s\'",msg_info["LoginName"].asCString());

   std::string sql =buff;
   std::string key ="ItemID,LoginName,NickName,Password";

   Json::Value reply;

   DBQueryResult result= DBManager::GetDBManager()->Query(key,sql);

   if(result.GetRowCount()==1)
   {//login name exists

      std::string user_passwd = result.Get(0,"Password");
      std::string nick_name = result.Get(0,"NickName");
      std::string login_passwd = msg_info["Password"].asCString();
      if(user_passwd == login_passwd)
      {
         std::string user_id = result.Get(0, "ItemID");
         snprintf(buff, sizeof(buff),
                  "insert into UserLoginInfo(UserID,LoginTime,LoginType) values(\'%s\',Now(),1)",
                  user_id.data());

         std::string insert_sql = buff;

         int ret = DBManager::GetDBManager()->Update(insert_sql);
         if (ret > 0)
         { // insert sucessful
            reply["State"] = "Sucessful";
            reply["ErrorCode"] = "0";
            reply["LoginName"] = result.Get(0,"LoginName");
            reply["NickName"] = nick_name;


            std::string uf_key = "PeerLoginName,PeerNickName";
            // find the user friends
            snprintf(buff, sizeof(buff),
                     "SELECT LoginName as PeerLoginName ,NickName as PeerNickName FROM IMUser WHERE ItemID IN\
          (select uf.PeerID from UserFriend uf,IMUser u where uf.UserID= u.ItemID and u.ItemID ='\%s\')",
                     user_id.data());
            sql = buff;

            DBQueryResult uf_result = DBManager::GetDBManager()->Query(uf_key, sql);

            Json::Value user_friend;
            for (size_t i = 0; i != uf_result.GetRowCount(); i++)
            {
               Json::Value friend_info;
               friend_info["PeerLoginName"] = uf_result.Get(i, "PeerLoginName");
               friend_info["PeerNickName"] = uf_result.Get(i, "PeerNickName");
               user_friend.append(friend_info);
            }

            reply["Friend"] = user_friend;
         }
         else
         {
            reply["State"] = "Failed";
            reply["ErrorCode"] = "3";
         }
      }
      else
      {// password error
         reply["State"] = "Failed";
         reply["ErrorCode"] = "2";
      }

   }
   else
   {// user not exits
      reply["State"] = "Failed";
      reply["ErrorCode"] = "1";
   }
   //rely to client
   //0 success 1 exits 2 failed
   
   LoginReplyMessage login_msg_reply(reply);
   channel->WriteMessagePackage(login_msg_reply);
}