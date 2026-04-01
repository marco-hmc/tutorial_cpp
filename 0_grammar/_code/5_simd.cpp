#include <xmmintrin.h>

#include <chrono>
#include <iostream>

namespace Add_simd {
    void add_simd(float *a, float *b, float *c, int size) {
        int i;
        for (i = 0; i <= size - 4; i += 4) {
            __m128 vec1 = _mm_loadu_ps(a + i);
            __m128 vec2 = _mm_loadu_ps(b + i);
            __m128 result = _mm_add_ps(vec1, vec2);
            _mm_storeu_ps(c + i, result);
        }

        // 处理剩余的元素
        for (; i < size; ++i) {
            c[i] = a[i] + b[i];
        }
    }

    void add_normal(float *a, float *b, float *c, int size) {
        for (int i = 0; i < size; ++i) {
            c[i] = a[i] + b[i];
        }
    }

    void task() {
        const int size = 1000000;
        float *a = new float[size];
        float *b = new float[size];
        float *c = new float[size];

        // 初始化 a 和 b
        for (int i = 0; i < size; ++i) {
            a[i] = i;
            b[i] = i;
        }

        // 测试 SIMD 版本的时间
        auto start = std::chrono::high_resolution_clock::now();
        // 0.0016336 sec
        add_simd(a, b, c, size);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        std::cout << "SIMD version took " << diff.count() << " seconds\n";

        // 测试普通版本的时间
        start = std::chrono::high_resolution_clock::now();
        // 0.003437 sec
        add_normal(a, b, c, size);
        end = std::chrono::high_resolution_clock::now();
        diff = end - start;
        std::cout << "Normal version took " << diff.count() << " seconds\n";

        delete[] a;
        delete[] b;
        delete[] c;
    }

}  // namespace Add_simd

namespace Mul_simd {

    void multiply_simd(float *a, float *b, float *c, int size) {
        int i;
        for (i = 0; i <= size - 4; i += 4) {
            __m128 vec1 = _mm_loadu_ps(a + i);
            __m128 vec2 = _mm_loadu_ps(b + i);
            __m128 result = _mm_mul_ps(vec1, vec2);
            _mm_storeu_ps(c + i, result);
        }

        // 处理剩余的元素
        for (; i < size; ++i) {
            c[i] = a[i] * b[i];
        }
    }

    void multiply_normal(float *a, float *b, float *c, int size) {
        for (int i = 0; i < size; ++i) {
            c[i] = a[i] * b[i];
        }
    }

    void task() {
        const int size = 10000;
        float *a = new float[size];
        float *b = new float[size];
        float *c = new float[size];

        // 初始化 a 和 b
        for (int i = 0; i < size; ++i) {
            a[i] = i;
            b[i] = i;
        }

        // 测试 SIMD 版本的时间
        auto start = std::chrono::high_resolution_clock::now();
        multiply_simd(a, b, c, size);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        std::cout << "SIMD version took " << diff.count() << " seconds\n";

        // 测试普通版本的时间
        start = std::chrono::high_resolution_clock::now();
        multiply_normal(a, b, c, size);
        end = std::chrono::high_resolution_clock::now();
        diff = end - start;
        std::cout << "Normal version took " << diff.count() << " seconds\n";

        delete[] a;
        delete[] b;
        delete[] c;
    }
}  // namespace Mul_simd

int main() {
    Add_simd::task();
    Mul_simd::task();
}