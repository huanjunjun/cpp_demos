#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>

/**
 * @brief 将当前进程转换为守护进程。
 * 
 * 守护进程是在后台运行的进程，不与控制终端关联。
 * 此函数执行一系列步骤来创建守护进程，包括两次 fork、设置新会话、
 * 忽略信号、更改文件模式掩码、工作目录、关闭文件描述符、重定向标准输入输出、
 * 打开日志文件并在循环中记录日志。
 */
void daemonize() {
    // 第一次 fork，创建子进程
    pid_t pid = fork();

    // 检查 fork 是否失败
    if (pid < 0) {
        // 输出错误信息
        std::cerr << "Fork failed!" << std::endl;
        // 以失败状态退出程序
        exit(EXIT_FAILURE);
    }

    // 如果 pid 大于 0，说明当前是父进程
    if (pid > 0) {
        // 父进程正常退出
        exit(EXIT_SUCCESS);
    }

    // 以下代码在子进程中执行
    // 创建新会话，使子进程成为会话首进程，脱离原控制终端
    if (setsid() < 0) {
        // 输出错误信息
        std::cerr << "setsid failed!" << std::endl;
        // 以失败状态退出程序
        exit(EXIT_FAILURE);
    }

    // 忽略子进程结束时发送给父进程的 SIGCHLD 信号，防止产生僵尸进程
    signal(SIGCHLD, SIG_IGN);

    // 第二次 fork，确保守护进程不是会话首进程，防止重新获取控制终端
    pid = fork();

    // 检查 fork 是否失败
    if (pid < 0) {
        // 输出错误信息
        std::cerr << "Fork failed!" << std::endl;
        // 以失败状态退出程序
        exit(EXIT_FAILURE);
    }

    // 如果 pid 大于 0，说明当前是父进程
    if (pid > 0) {
        // 父进程正常退出
        exit(EXIT_SUCCESS);
    }

    // 更改文件模式掩码，使守护进程创建的文件和目录具有最大权限
    umask(0);

    // 将当前工作目录更改为根目录，避免依赖特定工作目录
    if (chdir("/") < 0) {
        // 输出错误信息
        std::cerr << "chdir failed!" << std::endl;
        // 以失败状态退出程序
        exit(EXIT_FAILURE);
    }

    // 关闭所有打开的文件描述符，防止守护进程持有不必要的文件句柄
    for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
        close(x);
    }

    // 打开日志文件，用于记录守护进程的运行信息
    int log_fd = open("/tmp/daemon.log", O_RDWR | O_CREAT | O_APPEND, 0600);
    // 检查日志文件是否打开失败
    if (log_fd < 0) {
        // 输出错误信息
        std::cerr << "Failed to open log file!" << std::endl;
        // 以失败状态退出程序
        exit(EXIT_FAILURE);
    }

    // 将标准输入重定向到 /dev/null，使守护进程不接收输入
    open("/dev/null", O_RDONLY); 
    // 将标准输出重定向到 /dev/null，使守护进程不输出信息到终端
    open("/dev/null", O_WRONLY); 
    // 将标准错误输出重定向到 /dev/null，使守护进程不输出错误信息到终端
    open("/dev/null", O_WRONLY); 

    // 守护进程的主逻辑，进入无限循环
    while (true) {
        // 向日志文件写入信息，表示守护进程正在运行
        dprintf(log_fd, "Daemon is running...\n");
        // 守护进程休眠 10 秒
        sleep(10); 
    }

    // 关闭日志文件描述符
    close(log_fd);
}

int main() {
    daemonize();
    return 0;
}