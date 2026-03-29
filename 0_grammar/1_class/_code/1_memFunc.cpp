#include <iostream>

class A {
  public:
    void myfunc1(int tempvalue1) {
        std::cout << "tempvalue1 = " << tempvalue1 << std::endl;
    }

    void myfunc2(int tempvalue2) {
        std::cout << "tempvalue2 = " << tempvalue2 << std::endl;
    }

    static void mysfunc(int tempvalue) {
        std::cout << "A::mysfunc() 静态成员函数 -- tempvalue = " << tempvalue
                  << std::endl;
    }
};

void func() {
    /*
        对于普通成员函数需要取出地址，然后基于做地址偏移就可以获得成员函数地址，像普通函数一样使用了。
    */
    A mya;
    void (A::*pmypoint)(int tempvalue) = &A::myfunc1;
    (mya.*pmypoint)(1);

    pmypoint = &A::myfunc2;
    (mya.*pmypoint)(2);

    A *pmya = new A();
    (pmya->*pmypoint)(3);

    void (*pmyspoint)(int tempvalue) = &A::mysfunc;
    pmyspoint(4);

    delete pmya;
}

int main() {
    func();
    return 0;
}