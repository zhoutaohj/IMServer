#include "EventLoopThread.h"
#include <iostream>
#include <poll.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <poll.h>
#include "Channel.h"

EventLoopThread::EventLoopThread(): Thread(" IM EventLoop Thread")
{
    vec_poll.reserve(1000);
    server_fd = -1;
    server_port = -1;
}

EventLoopThread::EventLoopThread(const char* name,int port ,std::weak_ptr<ThreadPool> wp): Thread(name),server_port(port),thread_pool_wp(wp)
{
    vec_poll.reserve(1000);
}

EventLoopThread::~EventLoopThread()
{

}

void EventLoopThread::Run()
{
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr= htonl(INADDR_ANY);
    server_address.sin_port = htons(server_port);

    if((server_fd =socket(AF_INET,SOCK_STREAM,0))<0)
    {
        return ;
    }

    int on_opt=1;
    setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&on_opt,sizeof(on_opt));
    int ret=0;
    if((ret=bind(server_fd,(const sockaddr *)(&server_address),sizeof(server_address)))<0)
    {
        return ;
    }
    
    if((ret=listen(server_fd,10))<0)
    {
        return ;
    }

    //poll implement
    
    pollfd listen_poll;
    listen_poll.fd =server_fd;
    listen_poll.events =POLLRDNORM;
    vec_poll.push_back(listen_poll);
  
    while (true)
    {
        for (auto poll_vec_iter = vec_poll.begin(); poll_vec_iter != vec_poll.end();)
        {
            if (poll_vec_iter->fd == -1) 
            {
			   poll_vec_iter = vec_poll.erase(poll_vec_iter);
		    }
            else 
            {
                if(poll_vec_iter->fd!=server_fd)
                {
                    poll_vec_iter->events = channel_map[poll_vec_iter->fd]->GetPollEvents();
                }
                
                ++poll_vec_iter;
            }
			
        }



         int ret = poll(&vec_poll[0],vec_poll.size(),-1);

         if(ret==-1)
         {
             if(errno == EINTR)
             {
                 std::cout<<"poll erro =1"<<std::endl;
                 continue;
             }
             else
             {
                 return ;
             }
         }
         else if(ret ==0)
         {//timeout
             continue;
         }

         if(vec_poll[0].revents & POLLRDNORM)
         {//new register connection

             struct sockaddr_in client_addr;
             socklen_t client_len =sizeof(client_addr) ;

             int clientfd = accept4(server_fd,(sockaddr *)(&client_addr),&client_len,SOCK_NONBLOCK);

             if(clientfd >0)
             {   
                 
                 std::shared_ptr<Channel> channel_sp = std::make_shared<Channel>(clientfd,thread_pool_wp,shared_from_this());
                 channel_map.insert(std::pair<int,std::shared_ptr<Channel>>(clientfd,channel_sp));

                 pollfd client_poll_info;
                 client_poll_info.fd =clientfd;
                 client_poll_info.events = channel_map[clientfd]->GetPollEvents() ;
                 vec_poll.push_back(client_poll_info);
             }
         }

         for(size_t i=1;i!=vec_poll.size();i++)
         {//list other client fd
             if(vec_poll[i].fd ==-1)
                 continue;
             if(vec_poll[i].revents & (POLLRDNORM | POLLERR))
             {
                auto iter = channel_map.find(vec_poll[i].fd);

                if(iter == channel_map.end())
                {
                    continue;
                }

                std::shared_ptr<Channel>  client_channel = iter->second;
                int nread=client_channel->read();
                if( nread<0)
                {
                     if(errno == ECONNRESET)
                     {   
                         channel_map.erase(iter);               
                         vec_poll[i].fd =-1;
                     }
                     else
                     {//read error
                         return;
                     }
                 }
                 else if(nread ==0)
                 { //peer close 
                     channel_map.erase(iter);  
                     vec_poll[i].fd =-1;
                 }
                 else
                 {// analyze and build the package
                    std::cout<<" client_channel.analyze"<<std::endl;
                    client_channel->analyze();
                 }
             }


         }
     }
     


}
