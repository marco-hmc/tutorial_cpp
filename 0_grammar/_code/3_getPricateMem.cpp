#include <cassert>
#include <string>

namespace PrivateMemGetUtils {
    template <auto>
    struct A;

    template <typename T, typename Class, T Class::*Member>
    struct A<Member> {
        friend T& get(Class& c) { return c.*Member; }
    };
}  // namespace PrivateMemGetUtils

class Data {
  public:
    std::string value() const { return value_; }

  private:
    std::string value_ = "Rich";
};

template struct PrivateMemGetUtils::A<&Data::value_>;
std::string& PrivateMemGetUtils::get(Data& /*c*/);

int main() {
    Data data;
    assert(data.value() == "Rich");
    PrivateMemGetUtils::get(data) = "Money";
    assert(data.value() == "Money");
}