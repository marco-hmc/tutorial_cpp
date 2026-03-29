#include <iostream>

class A {
  public:
    virtual void myvirfunc(int tempvalue) {
        std::cout << "A::myvirfunc, tempvalue = " << tempvalue << std::endl;
    }
    virtual ~A() = default;
};

class B : public A {
  public:
    void myvirfunc(int tempvalue) override {
        std::cout << "B::myvirfunc, tempvalue = " << tempvalue << std::endl;
    }
};

void func() {
    B *pmyb = new B();  // pmyb：对象指针
    void (B::*pmyvirfunc)(int tempvalue) = &A::myvirfunc;  // 成员函数指针
    (pmyb->*pmyvirfunc)(
        190);  // pmyvirfunc虽然赋的是A的myvirfunc的值，但调用的仍然是B的myvirfunc，因为&A::myvirfunc实际上存储的是vcall（一个虚函数表中的偏移值，在A和B中是相同的）

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"
    printf("%p\n", (void *)&A::myvirfunc);  // vcall地址 和下个vcall地址不一样
    printf("%p\n", (void *)&B::myvirfunc);
#pragma GCC diagnostic pop
    delete pmyb;
}

int main() {
    func();
    return 0;
}