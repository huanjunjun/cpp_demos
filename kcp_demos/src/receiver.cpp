#include "kcp_wrapper.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class FileReceiver {
public:
    FileReceiver(const std::string& save_path) : save_path_(save_path) {
        fs::create_directories(save_path_);
    }

    void onDataReceived(const char* data, size_t len) {
        try {
            if (len < sizeof(KCPWrapper::FileHeader)) {
                return;
            }

            const auto* header = reinterpret_cast<const KCPWrapper::FileHeader*>(data);
            
            // 验证魔数
            if (header->magic != 0x12345678) {
                return;
            }

            // 创建保存路径
            fs::path full_path = save_path_ / header->filename;
            fs::create_directories(full_path.parent_path());

            // 打开文件
            std::ofstream file(full_path, std::ios::binary | std::ios::app);
            if (!file) {
                std::cerr << "Failed to open file: " << full_path << std::endl;
                return;
            }

            // 写入数据
            const char* file_data = data + sizeof(KCPWrapper::FileHeader);
            size_t file_data_len = len - sizeof(KCPWrapper::FileHeader);
            file.write(file_data, file_data_len);

            std::cout << "Received data for: " << header->filename 
                     << " (" << file_data_len << " bytes)" << std::endl;

        } catch (const std::exception& e) {
            std::cerr << "Error processing received data: " << e.what() << std::endl;
        }
    }

private:
    fs::path save_path_;
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <multicast_ip> <save_path>" << std::endl;
        return -1;
    }

    try {
        KCPWrapper kcp;
        FileReceiver receiver(argv[2]);
        
        // 初始化KCP 连接标识符 ，用于区分不同的连接
        uint32_t conv = 0x11223344;
        if (!kcp.init(argv[1], 12345, conv)) {
            std::cerr << "Failed to initialize KCP" << std::endl;
            return -1;
        }

        // 设置数据接收回调
        kcp.setDataCallback([&receiver](const char* data, size_t len) {
            receiver.onDataReceived(data, len);
        });

        std::cout << "Waiting for files..." << std::endl;

        // 主循环
        while (true) {
            kcp.update();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
} 