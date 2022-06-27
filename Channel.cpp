#include "Channel.h"
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include "Message.h"
#include "ThreadPool.h"
#include <iostream>
#include <string>

Channel::Channel(int fd, std::weak_ptr<ThreadPool> tp_wp, std::weak_ptr<EventLoopThread> el_wp)
    : socket_fd(fd), thread_pool_wp(tp_wp), event_loop__wp(el_wp)
{
    recv_buf.reserve(buff_size);
    send_buf.reserve(buff_size);
    _poll_events = POLLRDNORM;
}

Channel::~Channel()
{
    close(socket_fd);
}

Channel::Channel(Channel &&rvalue)
{
    socket_fd = rvalue.socket_fd;
    recv_buf = std::move(rvalue.recv_buf);
    send_buf = std::move(rvalue.send_buf);
}

int Channel::read()
{
    char buff[4096];
    size_t cur_pos = 0;
    int read_count = 0;
    while (true)
    {
        int nread = ::read(socket_fd, buff + cur_pos, sizeof(buff) - cur_pos);

        if (nread < 0)
        {
            if (errno == EINTR)
            { // recv signal
                continue;
            }
            else if (errno == EAGAIN)
            { // have no more data
                break;
            }
            else
            {
                return -1;
            }
        }
        else if (nread == 0)
        {
            return 0;
        }
        else
        { // read some data
            MutexLock lock(_mutex);
            recv_buf.insert(recv_buf.end(), buff, buff + nread);
            cur_pos += nread;
            read_count += nread;
        }
    }

    return read_count;
}

int Channel::write()
{//write to net
    
    MutexLock lock(_mutex);

    if(send_buf.empty())
        return -1;

    
    int nwrite = 0;
    int nleft = send_buf.size();
    char *ptr = send_buf.data();

    while (nleft > 0)
    {
        int n = ::write(socket_fd, ptr, nleft);

        if (n <= 0)
        { // error
            if (errno == EINTR)
            {
                continue;
            }
            else if (errno == EAGAIN)
            { // write buffer is full
                break;
            }
            else
            {
                return -1;
            }
        }
        else
        { // write some data
            nleft -= n;
            nwrite += n;
            ptr += n;
        }
    }

    send_buf.erase(send_buf.begin(),send_buf.begin() + nwrite);

    if(send_buf.empty())
    {
        _poll_events =POLLRDNORM ;
    }

    return nwrite;
}

size_t Channel::WriteBuffer(char* buff,size_t buff_size)
{ // write the data to send_buff
    if (buff == nullptr)
    {
        return 0;
    }

    {
       MutexLock lokc(_mutex);
       send_buf.insert(send_buf.end(),buff,buff+buff_size);
    }

    write();
    _poll_events =POLLRDNORM | POLLWRNORM;

    return 1;
}

size_t Channel::WriteMessagePackage(Message& msg)
{
   unsigned long long buffer_length = msg.msg_head.length;

   msg.Serialize();
   //utf8 chinese
   std::string str =Tool::JsonToString(msg.msg_info);

   char* buff_ptr = new char[buffer_length];
   memcpy(buff_ptr,&msg.msg_head,sizeof(msg.msg_head));
   memcpy((buff_ptr+sizeof(msg.msg_head)), str.c_str(), str.size());
   WriteBuffer(buff_ptr,buffer_length);

   delete []buff_ptr;
}
void Channel::analyze()
{ // analyze and build the msg package
    size_t msg_head_len = sizeof(MessageHead);

    MessageHead msg_head;
    if (recv_buf.size() >= msg_head_len)
    {
        memcpy(&msg_head, &recv_buf[0], msg_head_len);
        msg_head.Deserialize();
    }
    else
    {
        return;
    }

    unsigned long long data_length = msg_head.length - sizeof(msg_head);

    if (recv_buf.size() >= msg_head.length)
    {

        char buf[data_length];
        Json::Value value;
        Json::Reader reader;

        {
            MutexLock lock(_mutex);
            memcpy(buf, &(recv_buf[msg_head_len]), data_length);

            std::string str(buf);

            bool ret = reader.parse(buf, value);

            if (!ret)
            { // bad package ,discard the data
                recv_buf.erase(recv_buf.begin(), recv_buf.begin() + msg_head.length);
                return;
            }
            recv_buf.erase(recv_buf.begin(), recv_buf.begin() + msg_head.length);
            
        }

        if (msg_head.message_type == MessageType::REGISTERREQUEST)
        {
            TaskItem task;
            std::shared_ptr<Message> sp_msg(new RegisterRequestMessage(value));
            task.msg_sp = sp_msg;
            task.channel_sp = shared_from_this();

            if (!thread_pool_wp.expired())
            {
                std::shared_ptr<ThreadPool> sp_thread_pool = thread_pool_wp.lock();
                sp_thread_pool->AddTask(task);
            }
        }
        else if(msg_head.message_type == MessageType::LOGINREQUEST)
        {
            TaskItem task;
            std::shared_ptr<Message> sp_msg(new LoginRequestMessage(value));
            task.msg_sp = sp_msg;
            task.channel_sp = shared_from_this();

            if (!thread_pool_wp.expired())
            {
                std::shared_ptr<ThreadPool> sp_thread_pool = thread_pool_wp.lock();
                sp_thread_pool->AddTask(task);
            }
        }
    }
}

short Channel::GetPollEvents()
{
    return _poll_events.load();
}
void Channel::SetPollEvents(short events)
{
    _poll_events = events;
}