#include "kcp_wrapper.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <filesystem>
#include <thread>
#include <iostream>

namespace fs = std::filesystem;

KCPWrapper::KCPWrapper() 
    : kcp_(nullptr)
    , socket_fd_(-1)
    , initialized_(false) {
}

KCPWrapper::~KCPWrapper() {
    if (kcp_) {
        ikcp_release(kcp_);
    }
    if (socket_fd_ >= 0) {
        close(socket_fd_);
    }
}

bool KCPWrapper::init(const std::string& multicastIP, uint16_t port, uint32_t conv) {
    // 创建UDP socket
    socket_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
    std::cout << "socket_fd_: " << socket_fd_ << std::endl;
    if (socket_fd_ < 0) {
        std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
        return false;
    }

    // 设置socket为非阻塞模式
    int flags = fcntl(socket_fd_, F_GETFL, 0);
    fcntl(socket_fd_, F_SETFL, flags | O_NONBLOCK);

    // 允许地址重用
    int reuse = 1;
    if (setsockopt(socket_fd_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        std::cerr << "Failed to set SO_REUSEADDR: " << strerror(errno) << std::endl;
        return false;
    }

    // 设置组播TTL
    int ttl = 32;
    if (setsockopt(socket_fd_, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) < 0) {
        std::cerr << "Failed to set IP_MULTICAST_TTL: " << strerror(errno) << std::endl;
        return false;
    }

    // 获取本地网络接口
    struct ifaddrs *ifap, *ifa;
    struct in_addr interface_addr;
    bool interface_found = false;
    
    if (getifaddrs(&ifap) == 0) {
        for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
                // 跳过回环接口和非活动接口
                if (strcmp(ifa->ifa_name, "lo") == 0 || !(ifa->ifa_flags & IFF_UP)) {
                    continue;
                }
                interface_addr = ((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
                std::cout << "Found interface: " << ifa->ifa_name 
                         << ", addr: " << inet_ntoa(interface_addr) << std::endl;
                interface_found = true;
                break;
            }
        }
        freeifaddrs(ifap);
    }

    if (!interface_found) {
        std::cerr << "No suitable network interface found" << std::endl;
        return false;
    }

    // 设置组播发送接口
    if (setsockopt(socket_fd_, IPPROTO_IP, IP_MULTICAST_IF, &interface_addr, sizeof(interface_addr)) < 0) {
        std::cerr << "Failed to set IP_MULTICAST_IF: " << strerror(errno) << std::endl;
        return false;
    }

    // 设置组播地址
    memset(&mcast_addr_, 0, sizeof(mcast_addr_));
    mcast_addr_.sin_family = AF_INET;
    mcast_addr_.sin_port = htons(port);
    if (inet_pton(AF_INET, multicastIP.c_str(), &mcast_addr_.sin_addr) <= 0) {
        std::cerr << "Invalid multicast address: " << multicastIP << std::endl;
        return false;
    }

    bool is_sender = (conv & 0x80000000) != 0;
    uint32_t actual_conv = conv & 0x7FFFFFFF;

    if (!is_sender) {
        // 接收端需要绑定地址
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;
        
        if (bind(socket_fd_, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            std::cerr << "Failed to bind: " << strerror(errno) << std::endl;
            return false;
        }
    }

    // 加入组播组
    struct ip_mreq mreq;
    mreq.imr_multiaddr = mcast_addr_.sin_addr;
    mreq.imr_interface = interface_addr;
    
    if (setsockopt(socket_fd_, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        std::cerr << "Failed to join multicast group: " << strerror(errno) 
                  << " (errno: " << errno << ")" << std::endl;
        return false;
    }

    std::cout << (is_sender ? "Sender" : "Receiver") 
              << " joined multicast group " << multicastIP 
              << " on interface " << inet_ntoa(interface_addr) << std::endl;

    // 创建KCP实例
    kcp_ = ikcp_create(actual_conv, this);
    if (!kcp_) {
        std::cerr << "Failed to create KCP instance" << std::endl;
        return false;
    }

    // 设置KCP参数
    ikcp_wndsize(kcp_, 128, 128);
    ikcp_nodelay(kcp_, 1, 10, 2, 1);
    kcp_->output = udp_output;

    initialized_ = true;
    std::cout << "KCP initialized successfully with conv: " << actual_conv << std::endl;
    return true;
}

int KCPWrapper::udp_output(const char* buf, int len, ikcpcb* kcp, void* user) {
    KCPWrapper* wrapper = static_cast<KCPWrapper*>(user);
    
    // 打印发送地址信息
    char ip_str[INET_ADDRSTR_LEN];
    inet_ntop(AF_INET, &wrapper->mcast_addr_.sin_addr, ip_str, sizeof(ip_str));
    std::cout << "Sending to " << ip_str << ":" << ntohs(wrapper->mcast_addr_.sin_port) << std::endl;
    
    int sent = sendto(wrapper->socket_fd_, buf, len, 0, 
                     (struct sockaddr*)&wrapper->mcast_addr_, 
                     sizeof(wrapper->mcast_addr_));
    
    if (sent < 0) {
        std::cerr << "Failed to send UDP packet: " << strerror(errno) 
                  << " (errno: " << errno << ")" << std::endl;
        
        // 检查socket状态
        int error = 0;
        socklen_t errlen = sizeof(error);
        if (getsockopt(wrapper->socket_fd_, SOL_SOCKET, SO_ERROR, &error, &errlen) == 0) {
            if (error != 0) {
                std::cerr << "Socket error: " << strerror(error) << std::endl;
            }
        }
    } else {
        std::cout << "Sent UDP packet: " << sent << " bytes" << std::endl;
    }
    
    return sent;
}

bool KCPWrapper::send(const char* data, size_t len) {
    if (!initialized_ || !data || len == 0) {
        return false;
    }
    return ikcp_send(kcp_, data, len) >= 0;
}

void KCPWrapper::setDataCallback(DataCallback callback) {
    data_callback_ = callback;
}

void KCPWrapper::update() {
    if (!initialized_) {
        return;
    }

    // 更新KCP
    ikcp_update(kcp_, iclock());

    // 处理接收数据
    handleReceive();
}

void KCPWrapper::handleReceive() {
    while (true) {
        struct sockaddr_in addr;
        socklen_t addr_len = sizeof(addr);
        
        // 接收UDP数据
        int recv_len = recvfrom(socket_fd_, recv_buffer_, BUFFER_SIZE, 0,
                               (struct sockaddr*)&addr, &addr_len);
        
        if (recv_len < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            }
            continue;
        }

        // 输入数据到KCP
        ikcp_input(kcp_, recv_buffer_, recv_len);

        // 从KCP接收数据
        while (true) {
            int kcp_recv_len = ikcp_recv(kcp_, recv_buffer_, BUFFER_SIZE);
            if (kcp_recv_len < 0) {
                break;
            }
            
            // 调用回调函数处理数据
            if (data_callback_) {
                data_callback_(recv_buffer_, kcp_recv_len);
            }
        }
    }
}

KCPWrapper::ErrorCode KCPWrapper::sendFile(const std::string& filepath) {
    return sendFileInternal(filepath, fs::path(filepath).filename().string());
}

KCPWrapper::ErrorCode KCPWrapper::sendDirectory(const std::string& dirpath) {
    try {
        fs::path dir_path(dirpath);
        for (const auto& entry : fs::recursive_directory_iterator(dirpath)) {
            if (entry.is_regular_file()) {
                std::string relative_path = fs::relative(entry.path(), dir_path).string();
                ErrorCode result = sendFileInternal(entry.path().string(), relative_path);
                if (result != ErrorCode::SUCCESS) {
                    return result;
                }
            }
        }
        return ErrorCode::SUCCESS;
    } catch (const std::exception&) {
        return ErrorCode::FILE_NOT_FOUND;
    }
}

KCPWrapper::ErrorCode KCPWrapper::sendFileInternal(
    const std::string& filepath, 
    const std::string& relativePath) {
    
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        return ErrorCode::FILE_NOT_FOUND;
    }

    // 获取文件大小
    file.seekg(0, std::ios::end);
    size_t filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    // 准备文件头
    FileHeader header;
    header.magic = 0x12345678;
    header.type = 0;  // 文件类型
    header.size = filesize;
    std::cout << "Sending file: " << relativePath << " with size: " << filesize << std::endl;
    strncpy(header.filename, relativePath.c_str(), sizeof(header.filename) - 1);
    header.filename[sizeof(header.filename) - 1] = '\0';

    std::vector<char> buffer(sizeof(FileHeader) + CHUNK_SIZE);
    size_t total_sent = 0;

    while (!file.eof()) {
        // 复制文件头
        memcpy(buffer.data(), &header, sizeof(FileHeader));
        
        // 读取文件数据
        file.read(buffer.data() + sizeof(FileHeader), CHUNK_SIZE);
        size_t read_size = file.gcount();
        
        // 发送数据
        if (!send(buffer.data(), sizeof(FileHeader) + read_size)) {
            std::cout << "Failed to send data: " << strerror(errno) << std::endl;
            return ErrorCode::SEND_ERROR;
        }

        total_sent += read_size;
        
        // 调用进度回调
        if (progress_callback_) {
            progress_callback_(relativePath, total_sent, filesize);
        }
        // 等待一小段时间，避免发送太快
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return ErrorCode::SUCCESS;
}

void KCPWrapper::setProgressCallback(ProgressCallback callback) {
    progress_callback_ = callback;
} 