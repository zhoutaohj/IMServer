#include <iostream>
#include "IMServer.h"


using namespace std;

int main(int argc, char*argv[])
{
    
    std::cout<<"IM Server is starting"<<std::endl;
    
    string server_name("IM Server");
    unsigned int port=REGISTER_SERVER_PORT;
    IMServer register_server(server_name,port);
    int ret = register_server.start();

    while (true)
    {
        
    }
    

    return 1;
    
}