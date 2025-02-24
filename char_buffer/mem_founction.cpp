#include <iostream>
#include <cstring>

/**
 * @brief 主函数，演示各种以 mem 开头的字符串处理函数的使用
 * @return 程序退出状态码，0 表示正常退出
 */
int main() {
    // 示例 char 数组，用于存储待操作的字符串
    char source[] = "Hello, World!";
    // 用于存储 memcpy 复制结果的数组
    char destination[20];
    // 用于存储 memmove 复制结果和 memset 设置结果的数组
    char buffer[20];
    // 用于 memcmp 比较的第一个字符串数组
    char compare1[] = "abc";
    // 用于 memcmp 比较的第二个字符串数组
    char compare2[] = "abd";

    // 1. memcpy 示例
    std::cout << "=== memcpy 示例 ===" << std::endl;
    // 使用 memcpy 函数将 source 数组中的内容复制到 destination 数组中
    // 复制的字节数为 source 字符串的长度加 1（包含字符串结束符 '\0'）
    std::memcpy(destination, source, std::strlen(source) + 1);
    // 输出 memcpy 复制后的结果
    std::cout << "memcpy 复制后的结果: " << destination << std::endl;

    // 2. memmove 示例
    std::cout << "\n=== memmove 示例 ===" << std::endl;
    // 使用 memmove 函数将 source 数组中的内容复制到 buffer 数组中
    // 复制的字节数为 source 字符串的长度加 1（包含字符串结束符 '\0'）
    std::memmove(buffer, source, std::strlen(source) + 1);
    // 输出 memmove 复制后的结果
    std::cout << "memmove 复制后的结果: " << buffer << std::endl;

    // 3. memset 示例
    std::cout << "\n=== memset 示例 ===" << std::endl;
    // 使用 memset 函数将 buffer 数组的每个字节设置为字符 'A'
    // 设置的字节数为 buffer 数组的大小
    std::memset(buffer, 'A', sizeof(buffer));
    // 手动添加字符串结束符 '\0'，确保 buffer 是一个有效的 C 字符串
    buffer[sizeof(buffer) - 1] = '\0';
    // 输出 memset 设置后的结果
    std::cout << "memset 设置后的结果: " << buffer << std::endl;

    // 4. memcmp 示例
    std::cout << "\n=== memcmp 示例 ===" << std::endl;
    // 使用 memcmp 函数比较 compare1 和 compare2 数组的前 3 个字节
    // 返回值 cmp_result 表示比较结果
    int cmp_result = std::memcmp(compare1, compare2, 3);
    if (cmp_result < 0) {
        // 如果 cmp_result 小于 0，说明 compare1 小于 compare2
        std::cout << "compare1 < compare2" << std::endl;
    } else if (cmp_result > 0) {
        // 如果 cmp_result 大于 0，说明 compare1 大于 compare2
        std::cout << "compare1 > compare2" << std::endl;
    } else {
        // 如果 cmp_result 等于 0，说明 compare1 等于 compare2
        std::cout << "compare1 == compare2" << std::endl;
    }

    // 5. memchr 示例
    std::cout << "\n=== memchr 示例 ===" << std::endl;
    // 使用 memchr 函数在 source 数组中查找字符 'W' 的第一次出现位置
    // 返回值 found 是指向找到的字符的指针，如果未找到则为 nullptr
    char* found = static_cast<char*>(std::memchr(source, 'W', std::strlen(source)));
    if (found) {
        // 如果 found 不为 nullptr，说明找到了字符 'W'
        // 计算找到的字符在 source 数组中的位置并输出
        std::cout << "在位置 " << (found - source) << " 找到 'W'" << std::endl;
    } else {
        // 如果 found 为 nullptr，说明未找到字符 'W'
        std::cout << "未找到 'W'" << std::endl;
    }

    // 程序正常退出，返回 0
    return 0;
}
