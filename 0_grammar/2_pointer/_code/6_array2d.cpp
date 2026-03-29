#include <iostream>

template <class T>
class Array2D {
  public:
    Array2D(int dim1, int dim2) : dim1_(dim1), dim2_(dim2) {
        data_ = new T*[dim1];
        for (int i = 0; i < dim1; ++i) {
            data_[i] = new T[dim2];
        }
    }

    ~Array2D() {
        for (int i = 0; i < dim1_; ++i) {
            delete[] data_[i];
        }
        delete[] data_;
    }

    class Array1D {
      public:
        Array1D(T* array) : array_(array) {}

        T& operator[](int index) { return array_[index]; }

        const T& operator[](int index) const { return array_[index]; }

      private:
        T* array_;
    };

    Array1D operator[](int index) { return Array1D(data_[index]); }

    const Array1D operator[](int index) const { return Array1D(data_[index]); }

  private:
    int dim1_, dim2_;
    T** data_;
};

int main() {
    Array2D<int> data(10, 20);
    data[3][6] = 42;
    std::cout << data[3][6] << std::endl;  // 输出: 42
    return 0;
}