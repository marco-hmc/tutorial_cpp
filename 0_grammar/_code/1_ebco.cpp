namespace {

    template <typename Base, typename Member>
    class Pair : private Base {
      public:
        Pair(const Base& b, const Member& m) : Base(b), member_(m) {}

        const Base& first() const { return (const Base&)*this; }

        Base& first() { return (Base&)*this; }

        const Member& second() const { return this->member_; }

        Member& second() { return this->member_; }

      private:
        Member member_;
    };

    template <typename T>
    struct Unoptimizable {
        T info;
        void* storage;
    };

    template <typename T>
    struct Optimizable {
        Pair<T, void*> info_and_storage;
    };

}  // namespace

struct A {};

static_assert(sizeof(Unoptimizable<A>) == 2 * sizeof(void*));
static_assert(sizeof(Optimizable<A>) == sizeof(void*));

int main() {}