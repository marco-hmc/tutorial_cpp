#include <cassert>
#include <iostream>

namespace ArrDecayToPtr {
    void paramAsPtr(int *arr, int size) {
        for (int i = 0; i < size; i++) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
    }

    void paramAsArr(int arr[], int size) {
        // when decaying to ptr happens, it behaves exactly the same as ptr.
        int arrNoDecay[5] = {1, 2, 3, 4, 5};
        assert(sizeof(arr) == 8 && sizeof(arrNoDecay) == 20);
        for (int i = 0; i < size; i++) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
    }

    void example1() {
        int myArray[5] = {1, 2, 3, 4, 5};
        paramAsPtr(myArray, 5);
        paramAsArr(myArray, 5);
    }

}  // namespace ArrDecayToPtr

int main() {
    ArrDecayToPtr::example1();

    return 0;
}