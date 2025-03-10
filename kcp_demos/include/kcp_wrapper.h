#ifndef KCP_WRAPPER_H
#define KCP_WRAPPER_H

#include <string>
#include <functional>
#include <chrono>
#include <netinet/in.h>
#include "ikcp.h"
#include <net/if.h>
#include <ifaddrs.h>
#include <sys/ioctl.h>


#define INET_ADDRSTR_LEN 16

class KCPWrapper {
public:
    // 文件传输协议头
    struct FileHeader {
        uint32_t magic;          // 魔数，用于验证
        uint8_t type;           // 0:文件 1:文件夹
        uint64_t size;          // 文件大小
        char filename[256];     // 文件名
    };

    // 回调函数类型定义
    using DataCallback = std::function<void(const char*, size_t)>;
    
    // 文件传输相关的错误码
    enum class ErrorCode {
        SUCCESS = 0,
        FILE_NOT_FOUND,
        FILE_READ_ERROR,
        FILE_WRITE_ERROR,
        SEND_ERROR,
        INIT_ERROR,
        INVALID_PARAMETER
    };

    // 文件传输进度回调
    using ProgressCallback = std::function<void(const std::string&, size_t, size_t)>;
    
    KCPWrapper();
    ~KCPWrapper();

    // 初始化KCP和组播
    bool init(const std::string& multicastIP, uint16_t port, uint32_t conv);
    
    // 发送数据
    bool send(const char* data, size_t len);
    
    // 设置数据接收回调
    void setDataCallback(DataCallback callback);
    
    // 更新KCP (需要定期调用)
    void update();

    // 发送文件
    ErrorCode sendFile(const std::string& filepath);
    
    // 发送文件夹
    ErrorCode sendDirectory(const std::string& dirpath);
    
    // 设置进度回调
    void setProgressCallback(ProgressCallback callback);

private:
    // UDP输出回调函数
    static int udp_output(const char* buf, int len, ikcpcb* kcp, void* user);
    
    // 处理接收到的数据
    void handleReceive();
    
    ikcpcb* kcp_;                    // KCP实例
    int socket_fd_;                  // UDP socket
    struct sockaddr_in mcast_addr_;  // 组播地址
    DataCallback data_callback_;     // 数据接收回调
    bool initialized_;               // 初始化标志
    
    static const int BUFFER_SIZE = 65535;  // 最大缓冲区大小
    char recv_buffer_[BUFFER_SIZE];        // 接收缓冲区
    
    static const int CHUNK_SIZE = 1024 * 64;  // 64KB chunks for file transfer
    ProgressCallback progress_callback_;
    
    ErrorCode sendFileInternal(const std::string& filepath, const std::string& relativePath);

    inline uint32_t iclock() {
        auto now = std::chrono::steady_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        return static_cast<uint32_t>(ms);
    }
};

#endif // KCP_WRAPPER_H 