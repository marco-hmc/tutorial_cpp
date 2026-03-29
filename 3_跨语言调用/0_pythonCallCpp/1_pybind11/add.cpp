#include <pybind11/pybind11.h>

int add(int a, int b) { return a + b; }

namespace py = pybind11;

PYBIND11_MODULE(add, m) {
    m.doc() = "pybind11 example plugin";  // 模块文档字符串
    m.def("add", &add, "A function which adds two numbers");  // 暴露add函数
}