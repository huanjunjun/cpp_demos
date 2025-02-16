#include <iostream>
#include <vector>
#include <algorithm>
#include <signal.h>
#include <unistd.h>
using namespace std;

void interupt_signal_handler(int signum){
    cout<<"interupt signal handler"<<endl;
}

void quit_signal_handler(int signum){
    cout<<"quit signal handler"<<endl;
}


int main(){
    signal(SIGINT, interupt_signal_handler);
    signal(SIGQUIT, quit_signal_handler);
    while (1)
    {
        /* code */
        sleep(1);
    }

    return 0;
}