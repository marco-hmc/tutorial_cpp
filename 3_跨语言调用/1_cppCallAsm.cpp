#include <iostream>

void example() {
    int a = 10;
    int b = 20;
    int result;

    asm("movl %1, %%eax;"  // 将变量 a 的值移动到 eax 寄存器
        "addl %2, %%eax;"  // 将变量 b 的值加到 eax 寄存器
        "movl %%eax, %0;"  // 将 eax 寄存器的值移动到 result 变量
        : "=r"(result)     // 输出操作数
        : "r"(a), "r"(b)   // 输入操作数
        : "%eax"           // 被破坏的寄存器
    );

    std::cout << "Result: " << result << std::endl;
}

int main() {
    example();
    return 0;
}