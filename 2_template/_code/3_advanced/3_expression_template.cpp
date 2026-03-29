#include <array>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

// ------------------------- 表达式模板基类 -------------------------
template <typename E>
class Expression {
  public:
    double operator[](size_t i) const {
        return static_cast<const E&>(*this)[i];
    }

    size_t size() const { return static_cast<const E&>(*this).size(); }
};

// ------------------------- std::array 实现 -------------------------
template <size_t N>
class VectorET_array : public Expression<VectorET_array<N>> {
  private:
    std::array<double, N> data;

  public:
    VectorET_array() = default;

    double& operator[](size_t i) { return data[i]; }
    double operator[](size_t i) const { return data[i]; }

    size_t size() const { return N; }

    template <typename E>
    VectorET_array& operator=(const Expression<E>& expr) {
        for (size_t i = 0; i < N; ++i) {
            data[i] = expr[i];
        }
        return *this;
    }
};

// ------------------------- std::vector 实现（表达式模板） -------------------------
class VectorET_vector : public Expression<VectorET_vector> {
  private:
    std::vector<double> data;

  public:
    VectorET_vector(size_t size) : data(size) {}

    double& operator[](size_t i) { return data[i]; }
    double operator[](size_t i) const { return data[i]; }

    size_t size() const { return data.size(); }

    template <typename E>
    VectorET_vector& operator=(const Expression<E>& expr) {
        for (size_t i = 0; i < data.size(); ++i) {
            data[i] = expr[i];
        }
        return *this;
    }
};

template <size_t N>
class VectorTraditional_array {
  private:
    std::array<double, N> data;

  public:
    VectorTraditional_array() = default;

    double& operator[](size_t i) { return data[i]; }
    const double& operator[](size_t i) const { return data[i]; }

    size_t size() const { return N; }

    VectorTraditional_array operator+(
        const VectorTraditional_array& other) const {
        VectorTraditional_array result;
        for (size_t i = 0; i < N; ++i) result[i] = data[i] + other[i];
        return result;
    }

    VectorTraditional_array operator-(
        const VectorTraditional_array& other) const {
        VectorTraditional_array result;
        for (size_t i = 0; i < N; ++i) result[i] = data[i] - other[i];
        return result;
    }

    VectorTraditional_array operator*(double scalar) const {
        VectorTraditional_array result;
        for (size_t i = 0; i < N; ++i) result[i] = data[i] * scalar;
        return result;
    }
};

// ------------------------- std::vector 实现（传统计算） -------------------------
class VectorTraditional {
  private:
    std::vector<double> data;

  public:
    VectorTraditional(size_t size) : data(size) {}

    double& operator[](size_t i) { return data[i]; }
    const double& operator[](size_t i) const { return data[i]; }

    size_t size() const { return data.size(); }

    VectorTraditional operator+(const VectorTraditional& other) const {
        VectorTraditional result(size());
        for (size_t i = 0; i < size(); ++i) result[i] = data[i] + other[i];
        return result;
    }

    VectorTraditional operator-(const VectorTraditional& other) const {
        VectorTraditional result(size());
        for (size_t i = 0; i < size(); ++i) result[i] = data[i] - other[i];
        return result;
    }

    VectorTraditional operator*(double scalar) const {
        VectorTraditional result(size());
        for (size_t i = 0; i < size(); ++i) result[i] = data[i] * scalar;
        return result;
    }
};

// ------------------------- 表达式模板运算 -------------------------
template <typename E1, typename E2>
class AddExpr : public Expression<AddExpr<E1, E2>> {
  private:
    const E1& a;
    const E2& b;

  public:
    AddExpr(const E1& a, const E2& b) : a(a), b(b) {}
    double operator[](size_t i) const { return a[i] + b[i]; }
    size_t size() const { return a.size(); }
};

template <typename E1, typename E2>
class SubExpr : public Expression<SubExpr<E1, E2>> {
  private:
    const E1& a;
    const E2& b;

  public:
    SubExpr(const E1& a, const E2& b) : a(a), b(b) {}
    double operator[](size_t i) const { return a[i] - b[i]; }
    size_t size() const { return a.size(); }
};

template <typename E>
class MulExpr : public Expression<MulExpr<E>> {
  private:
    const E& a;
    double scalar;

  public:
    MulExpr(const E& a, double scalar) : a(a), scalar(scalar) {}
    double operator[](size_t i) const { return a[i] * scalar; }
    size_t size() const { return a.size(); }
};

// 运算符重载
template <typename E1, typename E2>
AddExpr<E1, E2> operator+(const Expression<E1>& a, const Expression<E2>& b) {
    return AddExpr<E1, E2>(static_cast<const E1&>(a),
                           static_cast<const E2&>(b));
}

template <typename E1, typename E2>
SubExpr<E1, E2> operator-(const Expression<E1>& a, const Expression<E2>& b) {
    return SubExpr<E1, E2>(static_cast<const E1&>(a),
                           static_cast<const E2&>(b));
}

template <typename E>
MulExpr<E> operator*(const Expression<E>& a, double scalar) {
    return MulExpr<E>(static_cast<const E&>(a), scalar);
}

template <typename E>
MulExpr<E> operator*(double scalar, const Expression<E>& a) {
    return MulExpr<E>(static_cast<const E&>(a), scalar);
}

// ------------------------- 初始化函数 -------------------------
template <typename Vec>
void initVector(Vec& vec) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (size_t i = 0; i < vec.size(); ++i) {
        vec[i] = dis(gen);
    }
}

// ------------------------- 测试 array -------------------------
void runArrayTest() {
    constexpr size_t N = 10000;
    constexpr int iterations = 100;
    using Vec = VectorET_array<N>;

    Vec a, b, c, d, result;
    initVector(a);
    initVector(b);
    initVector(c);
    initVector(d);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        result = ((a + b) - (c * 2.0)) + (d * 3.0);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "std::array + 表达式模板: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";
}

// ------------------------- 测试 vector + 表达式模板 -------------------------
void runVectorTemplateTest() {
    constexpr size_t N = 10000;
    constexpr int iterations = 100;
    using Vec = VectorET_vector;

    Vec a(N), b(N), c(N), d(N), result(N);
    initVector(a);
    initVector(b);
    initVector(c);
    initVector(d);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        result = ((a + b) - (c * 2.0)) + (d * 3.0);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "std::vector + 表达式模板: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";
}

void runArrayTraditionalTest() {
    constexpr size_t N = 10000;
    constexpr int iterations = 100;
    using Vec = VectorTraditional_array<N>;

    Vec a, b, c, d, result;
    initVector(a);
    initVector(b);
    initVector(c);
    initVector(d);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        result = (a + b - (c * 2.0) + (d * 3.0));
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "std::array + 传统写法: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";
}

// ------------------------- 测试 vector + 传统写法 -------------------------
void runVectorTraditionalTest() {
    constexpr size_t N = 10000;
    constexpr int iterations = 100;
    using Vec = VectorTraditional;

    Vec a(N), b(N), c(N), d(N), result(N);
    initVector(a);
    initVector(b);
    initVector(c);
    initVector(d);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        result = (a + b - (c * 2.0) + (d * 3.0));
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "std::vector + 传统写法: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";
}

// ------------------------- main -------------------------
int main() {
    std::cout << "表达式模板 vs 传统写法 性能对比:\n";
    runArrayTest();
    runArrayTraditionalTest();
    runVectorTemplateTest();
    runVectorTraditionalTest();
    return 0;
}
