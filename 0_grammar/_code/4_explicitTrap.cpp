#include <iostream>
#include <string>

class MyString {
  private:
    std::string data;

  public:
    MyString(const std::string& str) : data(str) {}

    operator const char*() const { return data.c_str(); }
};

void func(const std::string& s1, const std::string& s2, bool flag) {
    std::cout << "Called func(const std::string&, const std::string&, bool)"
              << std::endl;
}

void func(const std::string& s1, const std::string& s2) {
    std::cout << "Called func(const std::string&, const std::string&)"
              << std::endl;
}

void func(const std::string& s, bool flag) {
    std::cout << "Called func(const std::string&, bool)" << std::endl;
}

int main() {
    MyString str1("apple");
    MyString str2("banana");
    if (str1 < str2) {
        std::cout << "str1 is less than str2" << std::endl;
    }
    func("a", "b");

    return 0;
}