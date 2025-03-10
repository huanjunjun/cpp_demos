#include "kcp_wrapper.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <filesystem>

namespace fs = std::filesystem;

void onProgress(const std::string& filename, size_t transferred, size_t total) {
    float percentage = (float)transferred / total * 100;
    std::cout << "\rSending " << filename << ": " 
              << transferred << "/" << total << " bytes ("
              << std::fixed << std::setprecision(2) << percentage << "%)" 
              << std::flush;
    if (transferred == total) {
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <multicast_ip> <file_or_directory_path>" << std::endl;
        return -1;
    }

    try {
        KCPWrapper kcp;
        
        // 发送端使用相同的conv，但设置最高位为1表示发送端
        uint32_t conv = 0x11223344 | 0x80000000;
        if (!kcp.init(argv[1], 12345, conv)) {
            std::cerr << "Failed to initialize KCP" << std::endl;
            return -1;
        }

        // 设置进度回调
        kcp.setProgressCallback(onProgress);

        // 获取要发送的路径
        fs::path path(argv[2]);
        KCPWrapper::ErrorCode result;

        // 判断是文件还是文件夹
        if (fs::is_directory(path)) {
            std::cout << "Sending directory: " << path << std::endl;
            result = kcp.sendDirectory(path.string());
        } else if (fs::is_regular_file(path)) {
            std::cout << "Sending file: " << path << std::endl;
            result = kcp.sendFile(path.string());
        } else {
            std::cerr << "Invalid path: " << path << std::endl;
            return -1;
        }

        if (result != KCPWrapper::ErrorCode::SUCCESS) {
            std::cerr << "Failed to send: " << static_cast<int>(result) << std::endl;
            return -1;
        }

        // 等待一段时间确保数据发送完成
        std::this_thread::sleep_for(std::chrono::seconds(1));

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
} 