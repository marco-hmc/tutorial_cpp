#include <cassert>
#include <iostream>

namespace Arr2DPtr {
    void example1() {
        int actualArray[2][3] = {{1, 2, 3}, {4, 5, 6}};
        static_assert(sizeof(actualArray) == 24);  // 24

        int(*arr)[3] = actualArray;
        static_assert(sizeof(arr) == 8);     // 8
        static_assert(sizeof(*arr) == 12);   // 12
        assert(*arr + 1 == actualArray[1]);  // true
    }

}  // namespace Arr2DPtr

namespace Arr2DDecayToPrt {
    void func(int a[][3], int rows) {
        // rows is in need for avoiding memory access violation
        static_assert(sizeof(a) == 8);    // 8
        static_assert(sizeof(*a) == 12);  // 12
    }

    void example1() {
        int arr[2][3] = {{1, 2, 3}, {4, 5, 6}};
        func(arr, 2);
    }

}  // namespace Arr2DDecayToPrt

int main() {
    Arr2DPtr::example1();
    Arr2DDecayToPrt::example1();
    return 0;
}