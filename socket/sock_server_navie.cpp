#include<iostream>
#include<vector>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<cstring>
#include<thread>
#include<mutex>
using namespace std;


void client_handler(int client_fd){
    char read_buf[1024];
    char write_buf[1024];
    memset(read_buf,0,sizeof(read_buf));
    memset(write_buf,0,sizeof(write_buf));

    while(1){
        memset(read_buf,0,sizeof(read_buf));
        memset(write_buf,0,sizeof(write_buf));
        cout<<"wait recv"<<endl;
        int len = recv(client_fd,read_buf,sizeof(read_buf),0);
        cout<<"recv over"<<endl;
        if(len==-1){
            cout<<"recv error"<<endl;
            return;
        }
        if(len==0){
            cout<<"client close"<<endl;
            return;
        }
        cout<<"recv:"<<read_buf<<endl;
        strcat(write_buf,"server: ");
        strcat(write_buf,read_buf);
        send(client_fd,write_buf,strlen(write_buf),0);
    }
    
}

int main(){
    int socket_fd = socket(AF_INET,SOCK_STREAM,0);
    if(socket_fd==-1){
        cout<<"socket error"<<endl;
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(socket_fd,(struct sockaddr*)&addr,sizeof(addr));
    listen(socket_fd,5);
    vector<int> client_fds;
    vector<thread> client_threads;
    mutex mtx;
    
    while(1){
        
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int client_fd = accept(socket_fd,(struct sockaddr*)&client_addr,&len);
        if(client_fd==-1){
            cout<<"accept error"<<endl;
            return -1;
        }
        cout<<"client_fd:"<<client_fd<<endl;
        client_fds.push_back(client_fd);
        cout<<"client_fds.size():"<<client_fds.size()<<endl;
        thread t(client_handler,client_fd);
        client_threads.push_back(move(t));
    }

    for(int i=0;i<client_fds.size();i++){
        if(client_threads[i].joinable())
        client_threads[i].join();
    }


    return 0;
}

