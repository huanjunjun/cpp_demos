#include <iostream>

// 自定义的 unique_ptr 实现
template <typename T>
class UniquePtr {
private:
    T* ptr;  // 指向管理的对象的原始指针

public:
    // 构造函数
    explicit UniquePtr(T* p = nullptr) : ptr(p) {}

    // 析构函数
    ~UniquePtr() {
        delete ptr;
    }

    // 禁用拷贝构造函数
    UniquePtr(const UniquePtr&) = delete;

    // 禁用赋值运算符
    UniquePtr& operator=(const UniquePtr&) = delete;

    // 移动构造函数
    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    // 移动赋值运算符
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    // 重载解引用运算符
    T& operator*() const {
        return *ptr;
    }

    // 重载箭头运算符
    T* operator->() const {
        return ptr;
    }

    // 获取原始指针
    T* get() const {
        return ptr;
    }

    // 释放所有权
    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    // 重置指针
    void reset(T* p = nullptr) {
        if (ptr != p) {
            delete ptr;
            ptr = p;
        }
    }

    /**
     * @brief 重载 bool 类型转换运算符，用于在布尔上下文中判断 UniquePtr 是否持有有效指针。
     * 
     * 该运算符允许将 UniquePtr 对象隐式转换为 bool 类型，常用于 if 语句等布尔判断场景。
     * 当 UniquePtr 持有有效指针（即 ptr 不为 nullptr）时，返回 true；否则返回 false。
     * 
     * @return bool 如果 UniquePtr 持有有效指针，返回 true；否则返回 false。
     */
    explicit operator bool() const noexcept {
        // 判断指针是否为空，若不为空则返回 true，表示持有有效指针
        return ptr != nullptr;
    }
};

// 示例使用
class MyClass {
public:
    MyClass() { std::cout << "MyClass 构造" << std::endl; }
    ~MyClass() { std::cout << "MyClass 析构" << std::endl; }
    void doSomething() { std::cout << "MyClass 执行操作" << std::endl; }
};

int main() {
    UniquePtr<MyClass> ptr(new MyClass());
    ptr->doSomething();

    UniquePtr<MyClass> ptr2 = std::move(ptr);
    if (ptr2) {
        ptr2->doSomething();
    }

    return 0;
}
