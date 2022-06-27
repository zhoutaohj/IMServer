#ifndef MESSAGE_H
#define MESSAGE_H
// Message define
#include <json/json.h>
#include <netinet/in.h>
#include "Tool.h"

class Channel;
enum class ProtocoType : short
{
    JSON = 1,
    PROTOBUF =2

};

enum class MessageType : short
{
    REGISTERREQUEST,
    REGISTERREPLY,
    LOGINREQUEST,
    LOGINREPLY,
    CHATREQUEST ,
    CHATREPLY ,
    KEEPALIVE,
    SIGNOUTREQUEST,
    SIGNOUTREPLY
};

struct MessageHead
{
    unsigned short head_num; //20220423;
    MessageType message_type;
    ProtocoType protoco_type; // 1 json 2 protobuf
    unsigned short check_sum;
    unsigned long long length; // head + body

    MessageHead () =default;
    MessageHead(MessageType msg_type,ProtocoType pro_type)
    {
        head_num = 1225;
        check_sum = 0;
        message_type = msg_type;
        protoco_type= pro_type;
    }

    void Serialize()
    {
        head_num =htons(head_num);
        check_sum =htons(check_sum);
        length = htobe64(length);
    }

    void Deserialize()
    {
        head_num =ntohs(head_num);
        check_sum =ntohs(check_sum);
        length = be64toh(length);

    }
    ~MessageHead()=default;
};



struct Message
{
    MessageHead msg_head;
    Json::Value msg_info;

    Message() =default;

    Message(const Json::Value& login_info):msg_info(login_info)
    {
    }

     void Serialize() 
    {
        msg_head.Serialize();
    }

     void Deserialize()
    {
        msg_head.Deserialize();

    }

    virtual void process(std::shared_ptr<Channel>) =0;

};


struct RegisterRequestMessage :public Message
{
    RegisterRequestMessage(const Json::Value & info)
    : Message(info)
    {
       msg_head.message_type =MessageType::REGISTERREQUEST;
       msg_head.protoco_type =ProtocoType::JSON;
       msg_head.head_num =1225;
       msg_head.length = sizeof(msg_head) + msg_info.toStyledString().size();
    }
    void process(std::shared_ptr<Channel>) final;


};

struct RegisterReplyMessage :public Message
{
    RegisterReplyMessage(const Json::Value & info)
    : Message(info)
    {
       msg_head.message_type =MessageType::REGISTERREPLY;
       msg_head.protoco_type =ProtocoType::JSON;
       msg_head.head_num =1225;
       msg_head.length = sizeof(msg_head) + msg_info.toStyledString().size();
    }
    
    void process(std::shared_ptr<Channel>) final
    {

    }


};

struct LoginRequestMessage :public Message
{
    LoginRequestMessage(const Json::Value & info)
    :Message(info)
    {
       msg_head.message_type =MessageType::LOGINREQUEST;
       msg_head.protoco_type =ProtocoType::JSON;
       msg_head.head_num =1225;

       msg_head.length = sizeof(msg_head) + msg_info.toStyledString().size();
    }


    void process(std::shared_ptr<Channel>) final;
};

struct LoginReplyMessage :public Message
{
    LoginReplyMessage(const Json::Value & info)
    :Message(info)
    {
       msg_head.message_type =MessageType::LOGINREPLY;
       msg_head.protoco_type =ProtocoType::JSON;
       msg_head.head_num =1225;
       std::string str = Tool::JsonToString(msg_info);
       msg_head.length = sizeof(msg_head) + str.size();

    }


    void process(std::shared_ptr<Channel>) final
    {
        
    }
};



struct ChatRequestMessage :public Message
{
    ChatRequestMessage(const Json::Value & info)
    :Message(info)
    {
       msg_head.message_type =MessageType::CHATREQUEST;
       msg_head.protoco_type =ProtocoType::JSON;
       msg_head.head_num =1225;
       msg_head.length = sizeof(msg_head) + msg_info.toStyledString().size();
    }


    void process(std::shared_ptr<Channel>) final;


};

struct ChatReplyMessage :public Message
{
    ChatReplyMessage(const Json::Value & info)
    :Message(info)
    {
       msg_head.message_type =MessageType::CHATREPLY;
       msg_head.protoco_type =ProtocoType::JSON;
       msg_head.head_num =1225;
       msg_head.length = sizeof(msg_head) + msg_info.toStyledString().size();
    } 

    void process(std::shared_ptr<Channel>) final;


};

struct SignoutRequestMessage :public Message
{
    SignoutRequestMessage(const Json::Value & info)
    :Message(info)
    {
       msg_head.message_type =MessageType::SIGNOUTREQUEST;
       msg_head.protoco_type =ProtocoType::JSON;
       msg_head.head_num =1225;
       msg_head.length = sizeof(msg_head) + msg_info.toStyledString().size();
    }

    void process(std::shared_ptr<Channel>) final;
};

struct SignoutReplyMessage :public Message
{
    SignoutReplyMessage(const Json::Value & info)
    :Message(info)
    {
       msg_head.message_type =MessageType::SIGNOUTREPLY;
       msg_head.protoco_type =ProtocoType::JSON;
       msg_head.head_num =1225;
       msg_head.length = sizeof(msg_head) + msg_info.toStyledString().size();
    }

    void process(std::shared_ptr<Channel>) final;
};

struct KeepAliveMessage :public Message
{
    KeepAliveMessage(const Json::Value & info)
    :Message(info)
    {
       msg_head.message_type =MessageType::KEEPALIVE;
       msg_head.protoco_type =ProtocoType::JSON;
       msg_head.head_num =1225;
       msg_head.length = sizeof(msg_head) + msg_info.toStyledString().size();
    }

    void process(std::shared_ptr<Channel>) final;


};

#endif