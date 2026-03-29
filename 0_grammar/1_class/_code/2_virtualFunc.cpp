#include <iostream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"

class A {
  public:
    virtual void myvirfunc(int tempvalue) {
        std::cout << "A::myvirfunc, tempvalue = " << tempvalue << std::endl;
    }

    void myfunc2(int tempvalue) {
        std::cout << "A::myfunc2, tempvalue = " << tempvalue << std::endl;
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
    {  // 成员函数指针  -- vcall(vcall trunk)地址（虚函数）
        A *pvaobj = new A;
        pvaobj->myvirfunc(1);  // 直接调用虚函数

        void (A::*pmyvirfunc)(int tempvalue) = &A::myvirfunc;
        (pvaobj->*pmyvirfunc)(2);  // 通过成员函数指针调用虚函数
        printf("%p\n\n", (void *)&A::myvirfunc);  // 打印虚函数地址

        pmyvirfunc = &A::myfunc2;  // 赋值为真正的成员函数地址
        (pvaobj->*pmyvirfunc)(3);  // 通过成员函数指针调用普通成员函数
        delete pvaobj;
    }
    {
        B *pvbobj = new B;
        void (A::*pmyvirfunc)(int tempvalue) = &A::myvirfunc;
        (pvbobj->*pmyvirfunc)(4);  // 通过成员函数指针调用普通成员函数
        printf("%p\n\n", (void *)&B::myvirfunc);  // 打印虚函数地址

        void (B::*pmyvirfunc2)(int tempvalue) = &B::myvirfunc;
        (pvbobj->*pmyvirfunc2)(5);  // 通过成员函数指针调用普通成员函数
        printf("%p\n\n", (void *)&B::myvirfunc);  // 打印虚函数地址

        delete pvbobj;
    }
}

#pragma GCC diagnostic pop

int main() {
    func();
    return 0;
}