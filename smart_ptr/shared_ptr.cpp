#include <iostream>

// 引用计数类
template <typename T>
class RefCount {
public:
    RefCount(T* ptr) : data(ptr), count(1) {}

    void increment() {
        ++count;
    }

    int decrement() {
        return --count;
    }

    int getCount() const {
        return count;
    }

    T* getData() const {
        return data;
    }

    ~RefCount() {
        delete data;
    }

private:
    T* data;
    int count;
};

// 自定义的 shared_ptr 实现
template <typename T>
class SharedPtr {
public:
    // 构造函数
    explicit SharedPtr(T* ptr = nullptr) : ref(nullptr) {
        if (ptr) {
            ref = new RefCount<T>(ptr);
        }
    }

    // 拷贝构造函数
    SharedPtr(const SharedPtr<T>& other) : ref(other.ref) {
        if (ref) {
            ref->increment();
        }
    }

    // 赋值运算符重载
    SharedPtr<T>& operator=(const SharedPtr<T>& other) {
        if (this != &other) {
            if (ref && ref->decrement() == 0) {
                delete ref;
            }
            ref = other.ref;
            if (ref) {
                ref->increment();
            }
        }
        return *this;
    }

    // 析构函数
    ~SharedPtr() {
        if (ref && ref->decrement() == 0) {
            delete ref;
        }
    }

    // 重载解引用运算符
    T& operator*() const {
        return *(ref->getData());
    }

    // 重载箭头运算符
    T* operator->() const {
        return ref->getData();
    }

    // 获取引用计数
    int use_count() const {
        return ref ? ref->getCount() : 0;
    }

private:
    RefCount<T>* ref;
};

// 示例使用
class MyClass {
public:
    MyClass() { std::cout << "MyClass 构造" << std::endl; }
    ~MyClass() { std::cout << "MyClass 析构" << std::endl; }
    void doSomething() { std::cout << "MyClass 执行操作" << std::endl; }
};

int main() {
    SharedPtr<MyClass> ptr1(new MyClass());
    std::cout << "ptr1 引用计数: " << ptr1.use_count() << std::endl;

    SharedPtr<MyClass> ptr2 = ptr1;
    std::cout << "ptr1 引用计数: " << ptr1.use_count() << std::endl;
    std::cout << "ptr2 引用计数: " << ptr2.use_count() << std::endl;

    ptr1->doSomething();
    ptr2->doSomething();

    return 0;
}
