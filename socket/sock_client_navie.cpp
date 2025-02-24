#include<iostream>
#include<vector>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<thread>

using namespace std;




int main(){
    int socket_fd = socket(AF_INET,SOCK_STREAM,0);
    if(socket_fd==-1){
        cout<<"socket error"<<endl;
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    int ret = connect(socket_fd,(struct sockaddr*)&addr,sizeof(addr));
    if(ret==-1){
        cout<<"connect error"<<endl;
        return -1;
    }
    char buf[1024];
    while(1){
        
        memset(buf,0,sizeof(buf));
        cin>>buf;
        send(socket_fd,buf,strlen(buf),0);
        memset(buf,0,sizeof(buf));
        recv(socket_fd,buf,sizeof(buf),0);
        cout<<"recv:"<<buf<<endl;

    }
    return 0;
}