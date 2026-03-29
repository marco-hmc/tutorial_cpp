#include <bitset>
#include <cassert>
#include <deque>
#include <iostream>
#include <string>
#include <vector>

namespace ParamIsTemplate {

    template <typename T, template <typename...> class Container>
    void f(const Container<T>& /*unused*/) {}

    void test() { f(std::vector<int>{}); }

}  // namespace ParamIsTemplate

namespace ParamIsTemplate_complexVersion {

    template <typename T,
              template <typename Elem, typename = std::allocator<Elem>>
              class Container = std::deque>
    class Stack {
      public:
        using reference = T&;
        using const_reference = const T&;

        template <typename, template <typename, typename> class>
        friend class Stack;

        template <typename U,
                  template <typename Elem2, typename = std::allocator<Elem2>>
                  class Container2>
        Stack<T, Container>& operator=(const Stack<U, Container2>& rhs);

        void push(const T& x);
        void pop();
        reference top();
        const_reference top() const;
        std::size_t size() const;
        bool empty() const;

      private:
        Container<T> container_;
    };

    template <typename T, template <typename, typename> class Container>
    template <typename U, template <typename, typename> class Container2>
    inline Stack<T, Container>& Stack<T, Container>::operator=(
        const Stack<U, Container2>& rhs) {
        container_.assign(rhs.container_.begin(), rhs.container_.end());
        return *this;
    }

    template <typename T, template <typename, typename> class Container>
    inline void Stack<T, Container>::push(const T& x) {
        container_.emplace_back(x);
    }

    template <typename T, template <typename, typename> class Container>
    inline void Stack<T, Container>::pop() {
        assert(!empty());
        container_.pop_back();
    }

    template <typename T, template <typename, typename> class Container>
    inline typename Stack<T, Container>::reference Stack<T, Container>::top() {
        assert(!empty());
        return container_.back();
    }

    template <typename T, template <typename, typename> class Container>
    inline typename Stack<T, Container>::const_reference
    Stack<T, Container>::top() const {
        assert(!empty());
        return container_.back();
    }

    template <typename T, template <typename, typename> class Container>
    inline std::size_t Stack<T, Container>::size() const {
        return container_.size();
    }

    template <typename T, template <typename, typename> class Container>
    inline bool Stack<T, Container>::empty() const {
        return container_.empty();
    }

    void test() {
        Stack<std::string, std::deque> s;
        s.push("hello");
        s.push("world");
        assert(s.size() == 2);
        assert(s.top() == "world");
        s.pop();
        assert(s.size() == 1);
        assert(s.top() == "hello");
        s.pop();
        assert(s.empty());

        std::cout << "All tests passed!" << std::endl;
    }

}  // namespace ParamIsTemplate_complexVersion

namespace StringLinkErrorTpl {

    template <const char* s>
    class S {
      public:
        void print() { std::cout << s << std::endl; }
    };

    const char str[] = "abc";  // external linked object
    void syntax_literal_string_ok_example() {
        S<str> i;
        i.print();
    }

    void syntax_literal_string_fail_example() {
        const char str[] = "abc";  // running linked object
        // S<str> i;
        // i.print();
    }

    void task() {
        syntax_literal_string_ok_example();
        syntax_literal_string_fail_example();
    }
}  // namespace StringLinkErrorTpl

namespace UndefTypeLinkErrorTpl {
    template <typename T>
    class ClassB {
        T* a;
    };

    template <typename T>
    class ClassC {
        T a;
    };

    struct StructA;  // declare

    void syntax_undefined_type_fail_example() {
        // ClassC<StructA> e;  // error：becoz StructA is not defined
    }

    void syntax_undefined_type_ok_example() {
        ClassB<StructA*> d;  // ok：StructA is used as ptr
        struct StructA {};  // define StructA
        ClassC<StructA> f;  // ok：StructA is defined
    }

    void task() {
        syntax_undefined_type_fail_example();
        syntax_undefined_type_ok_example();
    }
}  // namespace UndefTypeLinkErrorTpl

namespace PtrLinkErrorTpl {
    template <int* buf>
    struct ArrPtr {};

    template <int (*fun)()>
    struct FuncPtr {};

    void task() {
        static int buffer[5] = {1, 2, 3, 4, 5};
        ArrPtr<buffer> arrPtr;

        // int buffer2[5] = {1, 2, 3, 4, 5};  // not ok
        // ArrPtr<buffer2> arrPtr2;

        auto func = []() -> int { return 42; };
        FuncPtr<func> funcPtr;
    }
}  // namespace PtrLinkErrorTpl

namespace ParamIsMemberFuncCase {

    struct A {
        void f(int*) const noexcept {}
    };

    template <typename RT, typename T, typename... Args>
    void f(RT (T::*)(Args...) const) {}

    void task() {
        f(&A::f);  // RT = void，T = A，Args = int*
    }
}  // namespace ParamIsMemberFuncCase

namespace TemplateArgsIsArr {

    template <typename T, typename U>
    constexpr bool less(const T& a, const U& b) {
        return a < b;
    }

    template <typename T, std::size_t M, std::size_t N>
    constexpr bool less(T (&a)[M], T (&b)[N]) {
        for (std::size_t i = 0; i < M && i < N; ++i) {
            if (a[i] < b[i]) {
                return true;
            }
            if (b[i] < a[i]) {
                return false;
            }
        }
        return M < N;
    }

    static_assert(less(0, 42));
    static_assert(!less("down", "demo"));
    static_assert(less("demo", "down"));
}  // namespace TemplateArgsIsArr

int main() {
    {
        ParamIsTemplate::test();
        ParamIsMemberFuncCase::task();
    }

    {
        StringLinkErrorTpl::task();
        UndefTypeLinkErrorTpl::task();
        PtrLinkErrorTpl::task();
    }

    ParamIsTemplate_complexVersion::test();
    return 0;
}