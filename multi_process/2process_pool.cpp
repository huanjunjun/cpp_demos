#include <iostream>
#include <unistd.h>      // for fork(), pipe(), close()
#include <sys/wait.h>    // for waitpid()
#include <vector>
#include <queue>
#include <functional>
#include <cstring>       // for memset()

const int NUM_WORKERS = 4; // 工作进程数量

// 任务类型
using Task = std::function<void()>;
//std::function是一个类模板，它是一个通用的多态函数封装器。
// std::function的实例可以存储、复制和调用任何可调用目标——函数、lambda表达式、bind表达式或其他函数对象，还可以用于指定函数的签名和返回类型。


// 声明进程池类
class ProcessPool {
public:
    ProcessPool(size_t numWorkers);
    ~ProcessPool();

    // 提交任务
    void submit(Task task);

private:
    // 工作进程函数
    void worker(int readFd);

    size_t numWorkers; // 工作进程数量
    std::vector<pid_t> workerPids; // 工作进程 PID 列表
    std::vector<int> pipes; // 管道文件描述符
};

// 定义构造函数，创建工作进程
ProcessPool::ProcessPool(size_t numWorkers) : numWorkers(numWorkers) {
    for (size_t i = 0; i < numWorkers; ++i) {
        int fd[2];//fd[0] is read end, fd[1] is write end
        if (pipe(fd) < 0) {//pipe() creates a pipe, a unidirectional data channel that can be used for interprocess communication.
            std::cerr << "Error creating pipe" << std::endl;
            exit(1);
        }

        pid_t pid = fork();
        if (pid < 0) {
            std::cerr << "Error forking process" << std::endl;
            exit(1);
        } else if (pid == 0) {
            /**
             * fork()函数会创建一个子进程，它是父进程的一个副本。子进程获得父进程数据空间、堆和栈的副本。
             * fork()函数被调用一次，但返回两次。两次返回的区别是子进程的返回值是0，而父进程的返回值是子进程的进程ID。
             * fork()函数的返回值是一个非负整数，表示进程ID。如果出现错误，fork()函数返回一个负值。
             * fork()函数只在父进程中返回子进程的进程ID，在子进程中返回0。
             * 所以，通过fork()函数的返回值，可以判断当前进程是父进程还是子进程。
             */
            // 子进程（工作进程）
            close(fd[1]); // 关闭写端
            worker(fd[0]); // 执行任务
            exit(0);
        } else {
            // 主进程
            close(fd[0]); // 关闭读端
            workerPids.push_back(pid);
            pipes.push_back(fd[1]); // 保存写端
        }
    }
}

// 析构函数，回收工作进程
ProcessPool::~ProcessPool() {
    for (size_t i = 0; i < numWorkers; ++i) {
        close(pipes[i]); // 关闭管道写端
        waitpid(workerPids[i], nullptr, 0); // 等待子进程退出
    }
}

// 提交任务
void ProcessPool::submit(Task task) {
    // 将任务序列化并写入管道
    char buffer[sizeof(Task)];
    memcpy(buffer, &task, sizeof(Task));//memcpy() copies n bytes from memory area src to memory area dest.

    // 简单轮询分配任务
    static size_t nextWorker = 0;
    write(pipes[nextWorker], buffer, sizeof(Task));
    nextWorker = (nextWorker + 1) % numWorkers;
}

// 工作进程函数
void ProcessPool::worker(int readFd) {
    while (true) {
        char buffer[sizeof(Task)];
        ssize_t bytesRead = read(readFd, buffer, sizeof(Task));
        if (bytesRead <= 0) {
            break; // 管道关闭，退出
        }

        // 反序列化任务并执行
        Task task;
        memcpy(&task, buffer, sizeof(Task));
        task();
    }
    close(readFd);
}

// 示例任务函数
void exampleTask(int id) {
    std::cout << "Worker " << getpid() << " is processing task " << id << std::endl;
    sleep(1); // 模拟任务执行时间
}

// int main() {
//     // 创建进程池
//     ProcessPool pool(NUM_WORKERS);

//     // 提交任务
//     for (int i = 0; i < 10; ++i) {
//         pool.submit([i]() { exampleTask(i); });//lambda function
//     }

//     // 等待所有任务完成
//     sleep(5);

//     return 0;
// }