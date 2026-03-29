#include <iostream>
#include <string>
#include <vector>

namespace quiz_1 {
    template <class T>
    void f(T &i) {
        std::cout << 1;
    }

    template <>
    void f(const int &i) {
        std::cout << 2;
    }

    void task() {
        int i = 42;
        f(i);
    }

}  // namespace quiz_1

namespace quiz_2 {

    void f(const std::string &) { std::cout << 1; }

    void f(const void *) { std::cout << 2; }

    void task() {
        f("foo");
        const char *bar = "bar";
        f(bar);
    }
}  // namespace quiz_2

namespace quiz_3 {

    class A {
      public:
        void f() { std::cout << "A"; }
    };

    class B : public A {
      public:
        void f() { std::cout << "B"; }
    };

    void g(A &a) { a.f(); }

    int main() {
        B b;
        g(b);
    }
}  // namespace quiz_3

namespace quiz_4 {

    struct A {
        A() { std::cout << "A"; }
        A(const A &a) { std::cout << "B"; }
        virtual void f() { std::cout << "C"; }
    };

    int main() {
        A a[2];
        for (auto x : a) {
            x.f();
        }
    }
}  // namespace quiz_4

namespace quiz_5 {

    struct A {
        A() { foo(); }
        virtual ~A() { foo(); }
        virtual void foo() { std::cout << "1"; }
        void bar() { foo(); }
    };

    struct B : public A {
        virtual void foo() { std::cout << "2"; }
    };

    int main() {
        B b;
        b.bar();
    }
}  // namespace quiz_5

namespace quiz_6 {
    struct X {
        X() { std::cout << "X"; }
    };

    int main() { X x(); }
}  // namespace quiz_6

namespace quiz_7 {

    struct X {
        X() { std::cout << "X"; }
    };

    struct Y {
        Y(const X &x) { std::cout << "Y"; }
        void f() { std::cout << "f"; }
    };

    int main() {
        Y y(X());
        // y.f();
    }
}  // namespace quiz_7

namespace quiz_8 {

    struct X {
        X() { std::cout << "a"; }
        X(const X &x) { std::cout << "b"; }
        const X &operator=(const X &x) {
            std::cout << "c";
            return *this;
        }
    };

    int main() {
        X x;
        X y(x);
        X z = y;
        z = x;
    }
}  // namespace quiz_8

namespace quiz_9 {

    struct GeneralException {
        virtual void print() { std::cout << "G"; }
    };

    struct SpecialException : public GeneralException {
        void print() override { std::cout << "S"; }
    };

    void f() { throw SpecialException(); }

    int main() {
        try {
            f();
        } catch (GeneralException e) {
            e.print();
        }
    }

}  // namespace quiz_9

namespace quiz_10 {

    int main() {
        std::vector<int> v1(1, 2);
        std::vector<int> v2{1, 2};
        std::cout << v1.size() << v2.size();
    }
}  // namespace quiz_10

namespace quiz_11 {

    int main() {
        int a = 0;
        decltype((a)) b = a;
        b++;
        std::cout << a << b;
    }
}  // namespace quiz_11

namespace quiz_12 {
    struct Dollars {
        Dollars(int v) : cents(v * 100) {}
        int cents;
    };
    void pay(Dollars d) { std::cout << "Paid " << d.cents << " cents\n"; }

    struct ExplicitDollars {
        explicit ExplicitDollars(int v) : cents(v * 100) {}
        int cents;
    };
    void pay_explicit(ExplicitDollars d) {
        std::cout << "Paid " << d.cents << " cents\n";
    }

    void demo_implicit() { pay(5); }
    void demo_explicit() {
        // pay_explicit({5}); // compile error
        pay_explicit(ExplicitDollars(5));
    }
}  // namespace quiz_12

int main() {}