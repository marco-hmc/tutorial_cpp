// 学习处理模板化基类内的名称
// 在使用模板类时，模板化的基类和派生类之间有时会出现名称遮蔽的问题。
// 尤其是在模板继承中，派生类可能会尝试访问基类的成员函数，但由于基类的函数被模板参数所依赖，编译器可能无法正确识别。

#include <iostream>
#include <string>

class MsgInfo {};

class CompanyA {
  public:
    void sendCleartext(const std::string& msg) {
        std::cout << "CompanyA sending cleartext: " << msg << std::endl;
    }
};

class CompanyB {
    // 类定义...
};

// 解决方法 1：使用 `this` 访问基类成员
namespace solution2 {
    template <typename Company>
    class MsgSender {
      public:
        void sendClear(const MsgInfo& info) {
            std::string msg = "Message";
            Company c;
            c.sendCleartext(msg);  // 使用Company类型的sendCleartext
        }
    };

    template <typename Company>
    class LoggingMsgSender : public MsgSender<Company> {
      public:
        void sendClearMsg(const MsgInfo& info) {
            // 记录日志
            this->sendClear(info);  // 使用 this-> 访问基类的 sendClear
        }
    };
}  // namespace solution2

// 解决方法 2：使用 `using` 引入基类的成员
namespace solution3 {
    template <typename Company>
    class MsgSender {
      public:
        void sendClear(const MsgInfo& info) {
            std::string msg = "Message";
            Company c;
            c.sendCleartext(msg);  // 使用Company类型的sendCleartext
        }
    };

    template <typename Company>
    class LoggingMsgSender : public MsgSender<Company> {
      public:
        using MsgSender<Company>::sendClear;  // 显式引入基类的 sendClear 函数

        void sendClearMsg(const MsgInfo& info) {
            // 记录日志
            sendClear(info);  // 现在可以调用基类的 sendClear
        }
    };
}  // namespace solution3

// 解决方法 3：显式指定基类成员的访问
namespace solution4 {
    template <typename Company>
    class MsgSender {
      public:
        void sendClear(const MsgInfo& info) {
            std::string msg = "Message";
            Company c;
            c.sendCleartext(msg);  // 使用Company类型的sendCleartext
        }
    };

    template <typename Company>
    class LoggingMsgSender : public MsgSender<Company> {
      public:
        void sendClearMsg(const MsgInfo& info) {
            // 记录日志
            MsgSender<Company>::sendClear(info);  // 显式指定基类的成员
        }
    };
}  // namespace solution4

int main() {
    MsgInfo info;
    // 测试解决方法 1
    solution2::LoggingMsgSender<CompanyA> sender2;
    sender2.sendClearMsg(info);

    // 测试解决方法 2
    solution3::LoggingMsgSender<CompanyA> sender3;
    sender3.sendClearMsg(info);

    // 测试解决方法 3
    solution4::LoggingMsgSender<CompanyA> sender4;
    sender4.sendClearMsg(info);

    return 0;
}