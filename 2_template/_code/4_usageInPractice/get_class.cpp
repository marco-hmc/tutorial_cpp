#include <cassert>

namespace {

    template <typename>
    struct get_class;

    template <typename ClassType, typename MemberType>
    struct get_class<MemberType ClassType::*> {
        using type = ClassType;
    };

    template <typename T>
    using get_class_t = typename get_class<T>::type;

    template <auto ClassMember>
    class Wrapper {
      public:
        Wrapper(get_class_t<decltype(ClassMember)>& obj) : obj_(obj) {}

        void increase() { ++(obj_.*ClassMember); }

      private:
        get_class_t<decltype(ClassMember)>& obj_;
    };

    struct A {
        int i = 0;
    };

}  // namespace

int main() {
    A a;
    Wrapper<&A::i>{a}.increase();
    assert(a.i == 1);
}
