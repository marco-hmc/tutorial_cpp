#include <iostream>

/*
    目的是区分String::[]的读用法和写用法，通过隐式转换和代理类巧妙实现。
    可以用于延迟计算、控制访问权限、记录日志等场景。

*/

class String {
  public:
    class CharProxy {
      public:
        CharProxy(String& str, int index)
            : theString_(str), charIndex_(index) {}

        CharProxy& operator=(const CharProxy& rhs) {
            if (this != &rhs) {
                theString_.setChar(charIndex_,
                                   rhs.theString_.getChar(rhs.charIndex_));
            }
            return *this;
        }

        CharProxy& operator=(char c) {
            theString_.setChar(charIndex_, c);
            return *this;
        }

        operator char() const { return theString_.getChar(charIndex_); }

      private:
        String& theString_;
        int charIndex_;
    };

    const CharProxy operator[](int index) const {
        return CharProxy(const_cast<String&>(*this), index);
    }

    CharProxy operator[](int index) { return CharProxy(*this, index); }

    void setChar(int index, char c) { value_[index] = c; }

    char getChar(int index) const { return value_[index]; }

  private:
    std::string value_;
};

int main() {
    String str;
    str.setChar(0, 'H');
    str.setChar(1, 'e');
    str.setChar(2, 'l');
    str.setChar(3, 'l');
    str.setChar(4, 'o');
    std::cout << str[0] << str[1] << str[2] << str[3] << str[4]
              << std::endl;  // 输出: Hello

    str[0] = 'h';
    std::cout << str[0] << str[1] << str[2] << str[3] << str[4]
              << std::endl;  // 输出: hello
    return 0;
}
