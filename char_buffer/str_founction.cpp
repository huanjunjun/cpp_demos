#include <iostream>
#include <cstring>


/*
在 C 和 C++ 中，以 str 开头的函数主要用于处理以空字符 '\0' 结尾的字符串（C 风格字符串），这些函数大多声明在 <cstring>（C++）或 <string.h>（C）头文件中。以下是一些常用的 str 开头的函数及其功能：
strlen(str)：返回字符串 str 的长度，不包括结尾的空字符 '\0'。
strcpy(dest, src)：将字符串 src 复制到字符串 dest 中，包括结尾的空字符 '\0'。
strcat(dest, src)：将字符串 src 连接到字符串 dest 的末尾，包括结尾的空字符 '\0'。
strcmp(str1, str2)：比较字符串 str1 和 str2，如果 str1 小于 str2，则返回一个负数；如果 str1 等于 str2，则返回 0；如果 str1 大于 str2，则返回一个正数。
strncpy(dest, src, n)：将字符串 src 的前 n 个字符复制到字符串 dest 中，包括结尾的空字符 '\0'。
strncat(dest, src, n)：将字符串 src 的前 n 个字符连接到字符串 dest 的末尾，包括结尾的空字符 '\0'。
strncmp(str1, str2, n)：比较字符串 str1 和 str2 的前 n 个字符，如果 str1 小于 str2，则返回一个负数；如果 str1 等于 str2，则返回 0；如果 str1 大于 str2，则返回一个正数。
strchr(str, c)：在字符串 str 中查找字符 c 第一次出现的位置，并返回一个指向该位置的指针。
strstr(str1, str2)：在字符串 str1 中查找字符串 str2 第一次出现的位置，并返回一个指向该位置的指针。
strtok(str, delimiters)：将字符串 str 按照指定的分隔符 delimiters 进行分割，并返回第一个分割后的子字符串。
这些函数在处理字符串时非常有用，特别是在处理文本数据、字符串操作和字符串比较等方面。需要注意的是，这些函数在处理字符串时可能会遇到缓冲区溢出的问题，因此在使用时需要小心处理。
*/
int main() {
    // 初始化字符串
    char str1[50] = "Hello";
    char str2[50] = " World";
    const char* str3 = "Hello";
    const char* str4 = "Hel";
    const char* str5 = "lo";
    char dest[100];

    // 1. strlen
    std::cout << "=== strlen 示例 ===" << std::endl;
    size_t len = std::strlen(str1);
    std::cout << "str1 的长度: " << len << std::endl;

    // 2. strcpy
    std::cout << "\n=== strcpy 示例 ===" << std::endl;
    std::strcpy(dest, str1);
    std::cout << "复制 str1 到 dest: " << dest << std::endl;

    // 3. strncpy
    std::cout << "\n=== strncpy 示例 ===" << std::endl;
    std::strncpy(dest, str3, 3);
    dest[3] = '\0'; // 手动添加字符串结束符
    std::cout << "复制 str3 的前 3 个字符到 dest: " << dest << std::endl;

    // 4. strcat
    std::cout << "\n=== strcat 示例 ===" << std::endl;
    std::strcpy(dest, str1);
    std::strcat(dest, str2);
    std::cout << "将 str2 追加到 str1 后: " << dest << std::endl;

    // 5. strncat
    std::cout << "\n=== strncat 示例 ===" << std::endl;
    std::strcpy(dest, str1);
    std::strncat(dest, str2, 3);
    std::cout << "将 str2 的前 3 个字符追加到 str1 后: " << dest << std::endl;

    // 6. strcmp
    std::cout << "\n=== strcmp 示例 ===" << std::endl;
    int cmp_result = std::strcmp(str1, str3);
    if (cmp_result == 0) {
        std::cout << "str1 和 str3 相等" << std::endl;
    } else {
        std::cout << "str1 和 str3 不相等" << std::endl;
    }

    // 7. strncmp
    std::cout << "\n=== strncmp 示例 ===" << std::endl;
    cmp_result = std::strncmp(str1, str4, 3);
    if (cmp_result == 0) {
        std::cout << "str1 和 str4 的前 3 个字符相等" << std::endl;
    } else {
        std::cout << "str1 和 str4 的前 3 个字符不相等" << std::endl;
    }

    // 8. strchr
    std::cout << "\n=== strchr 示例 ===" << std::endl;
    char* found = std::strchr(str1, 'l');
    if (found) {
        std::cout << "字符 'l' 在 str1 中首次出现的位置: " << (found - str1) << std::endl;
    } else {
        std::cout << "字符 'l' 未在 str1 中找到" << std::endl;
    }

    // 9. strrchr
    std::cout << "\n=== strrchr 示例 ===" << std::endl;
    found = std::strrchr(str1, 'l');
    if (found) {
        std::cout << "字符 'l' 在 str1 中最后出现的位置: " << (found - str1) << std::endl;
    } else {
        std::cout << "字符 'l' 未在 str1 中找到" << std::endl;
    }

    // 10. strstr
    std::cout << "\n=== strstr 示例 ===" << std::endl;
    found = std::strstr(str1, str5);
    if (found) {
        std::cout << "子字符串 \"" << str5 << "\" 在 str1 中首次出现的位置: " << (found - str1) << std::endl;
    } else {
        std::cout << "子字符串 \"" << str5 << "\" 未在 str1 中找到" << std::endl;
    }

    return 0;
}
