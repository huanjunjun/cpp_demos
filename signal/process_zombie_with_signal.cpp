#include <iostream>
#include <csignal>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// 信号处理函数
void sigchld_handler(int signal) {
    int status;
    pid_t pid;
    
    // 使用 waitpid 循环回收所有终止的子进程
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
            std::cout << "Child process " << pid << " exited with status " << WEXITSTATUS(status) << std::endl;
        } else if (WIFSIGNALED(status)) {
            std::cout << "Child process " << pid << " was killed by signal " << WTERMSIG(status) << std::endl;
        }
    }
}

int main() {
    // 注册 SIGCHLD 信号处理函数
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP; // SA_NOCLDSTOP 表示不接收子进程停止的信号

    //sigaction()函数用来改变指定信号的处理方式,此处是将SIGCHLD信号的处理方式改为sigchld_handler，SIGCHLD信号是子进程退出时发出的信号
    if (sigaction(SIGCHLD, &sa, nullptr) == -1) {
        std::cerr << "Failed to set SIGCHLD handler" << std::endl;
        return 1;
    }

    // 创建子进程
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Fork failed" << std::endl;
        return 1;
    } else if (pid == 0) {
        // 子进程
        std::cout << "Child process started, PID: " << getpid() << std::endl;
        sleep(2); // 模拟子进程运行
        std::cout << "Child process exiting" << std::endl;
        return 42; // 子进程退出状态
    } else {
        // 父进程
        std::cout << "Parent process waiting for child process to exit" << std::endl;
        while (true) {
            sleep(1); // 父进程继续运行
        }
    }

    return 0;
}