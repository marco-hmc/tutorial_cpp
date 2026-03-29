#include <iostream>
#include <new>

/*
placement new:
  1. grammar:
      new (ADDR)TYPE(); 
  2. 不会开辟空间，就在ADDR上这个地址
  3. 需要自己调用析构，需要自己释放内存

purpose:
  1. 和指针类型强转性比，其实可以简单理解为是调用了构造
    因为c++有class的概念，涉及虚函数，继承等等抽象成本，不可以直接通过类型转换来实例化一个对象。
    而C语言没有Class的概念，所以其实也就是没有placement new的，直接使用类型转换就好了。

| 对比项        | Placement new       | 指针类型强转（如 `reinterpret_cast<T*>`） |
| ---------- | ------------------- | -------------------------------- |
| 是否调用构造函数   | ✅ **会调用构造函数**       | ❌ **不会调用构造函数**                 |
| 是否分配内存     | ❌ 不会（使用已有内存）        | ❌ 不会（使用已有内存）                |
| 是否改变实际内存内容 | 可能（构造函数里有初始化逻辑）     | ❌ 不会（只是改变类型解释方式）   |
| 是否安全       | ✅ 通常更安全，符合 C++ 对象模型 | ❌ 可能导致未定义行为（UB）            |
| 常见用途       | 显式构造对象在指定内存         | 类型擦除、低层 hack 或反序列化            |

*/

class MyClass {
  public:
    explicit MyClass(int value) : value(value) {
        std::cout << "Constructor called with value: " << value << std::endl;
    }
    ~MyClass() {
        std::cout << "Destructor called for value: " << value << std::endl;
    }
    void display() const { std::cout << "Value: " << value << std::endl; }

  private:
    int value;
};

int main() {
    void* memory = operator new(sizeof(MyClass));
    auto* obj = new (memory) MyClass(42);

    obj->display();
    obj->~MyClass();
    operator delete(memory);
    return 0;
}