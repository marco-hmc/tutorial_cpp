#include <cstdint>
#include <iostream>

/*
结论：
1. 对象的首地址存的是虚函数表指针，虚函数表指针指向的是虚函数表，虚函数表里保存的是虚函数指针，虚函数指针指向虚函数。
   1. 虚函数指针vptr 指向 虚函数表vtbl，虚函数表里保存的是虚函数地址
   2. 虚函数表 存储在可执行目标文件的 只读数据段（.rodata）、虚函数存储在代码段（.text）。
2. 通过对象指针得到对象的首地址，通过强制类型转换得到虚函数表指针，取指针内容得到虚函数表。定义函数指针类型，使用虚函数表内容(虚函数地址)调用对象。
*/

class Base {
   public:
     virtual void f() { std::cout << "Base::f()" << std::endl; }
     virtual void g() { std::cout << "Base::g()" << std::endl; }
     virtual void h() { std::cout << "Base::h()" << std::endl; }
};

class Derive : public Base {
    virtual void g() override { std::cout << "Derive::g()" << std::endl; }
};

int main() {
    static_assert(sizeof(Base) == 8);
    static_assert(sizeof(Derive) == 8);

    auto *d = new Derive();
    auto *pVptr = reinterpret_cast<int64_t *>(d);
    auto *vptr = reinterpret_cast<int64_t *>(*pVptr);

    for (int i = 0; i <= 4; i++) {
        printf("vptr[%d] = 0x:%p\n", i, vptr[i]);
    }

    typedef void (*Func)();
    Func f = (Func)vptr[0];
    Func g = (Func)vptr[1];
    Func h = (Func)vptr[2];
    /*Func i = (Func)vptr[3];
	Func j = (Func)vptr[4];*/

    f();
    g();
    h();
    //i();

    Base *dpar = new Base();
    long *pvptrpar = (long *)dpar;
    long *vptrpar = (long *)(*pvptrpar);

    for (int i = 0; i <= 4; i++) {
        printf("vptr Base[%d] = 0x:%p\n", i, vptrpar[i]);
    }

    Func fpar = (Func)vptrpar[0];
    Func gpar = (Func)vptrpar[1];
    Func hpar = (Func)vptrpar[2];

    std::cout << "--------------------" << std::endl;
    fpar();
    gpar();
    hpar();
    return 1;
}