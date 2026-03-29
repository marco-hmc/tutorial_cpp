#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>

namespace RawPtr {

    class Any {
      private:
        void* data;
        const std::type_info& type;
        void (*deleter)(void*);
        void* (*creator)(const void*);

      public:
        template <typename T>
        Any(const T& value)
            : data(new T(value)),
              type(typeid(T)),
              deleter([](void* ptr) { delete static_cast<T*>(ptr); }),
              creator([](const void* ptr) -> void* {
                  return new T(*static_cast<const T*>(ptr));
              }) {}

        ~Any() {
            if (data) deleter(data);
        }

        Any(const Any& other)
            : data(other.data ? other.creator(other.data) : nullptr),
              type(other.type),
              deleter(other.deleter),
              creator(other.creator) {}

        template <typename T>
        bool is() const {
            return typeid(T) == type;
        }

        template <typename T>
        T get() const {
            if (!is<T>()) {
                throw std::bad_cast();
            }
            return *static_cast<T*>(data);
        }
    };

    void task() {
        Any value(42);
        std::cout << "Value: " << value.get<int>() << std::endl;

        Any text(std::string("Hello"));
        std::cout << "Text: " << text.get<std::string>() << std::endl;

        try {
            text.get<int>();
        } catch (const std::bad_cast& e) {
            std::cerr << "Bad cast: " << e.what() << std::endl;
        }
    }
}  // namespace RawPtr

namespace ModelConcept {
#include <iostream>
#include <memory>
#include <typeinfo>

    class Any {
      private:
        struct Concept {
            virtual ~Concept() = default;
            virtual const std::type_info& type() const = 0;
            virtual std::unique_ptr<Concept> clone() const = 0;
        };

        template <typename T>
        struct Model : Concept {
            T value;

            Model(const T& val) : value(val) {}

            const std::type_info& type() const override { return typeid(T); }

            std::unique_ptr<Concept> clone() const override {
                return std::make_unique<Model<T>>(value);
            }
        };

        std::unique_ptr<Concept> concept_;

      public:
        Any() = default;

        template <typename T>
        Any(const T& value) : concept_(std::make_unique<Model<T>>(value)) {}

        Any(const Any& other)
            : concept_(other.concept_ ? other.concept_->clone() : nullptr) {}

        Any(Any&& other) noexcept = default;

        Any& operator=(const Any& other) {
            if (this != &other) {
                concept_ = other.concept_ ? other.concept_->clone() : nullptr;
            }
            return *this;
        }

        Any& operator=(Any&& other) noexcept = default;

        template <typename T>
        bool is() const {
            return concept_ && concept_->type() == typeid(T);
        }

        template <typename T>
        T& get() {
            if (!is<T>()) throw std::bad_cast();
            return static_cast<Model<T>*>(concept_.get())->value;
        }

        template <typename T>
        const T& get() const {
            if (!is<T>()) throw std::bad_cast();
            return static_cast<const Model<T>*>(concept_.get())->value;
        }

        bool has_value() const { return static_cast<bool>(concept_); }

        void reset() { concept_.reset(); }

        const std::type_info& type() const {
            return concept_ ? concept_->type() : typeid(void);
        }
    };

    void task() {
        Any a = 123;
        std::cout << "a = " << a.get<int>() << std::endl;

        a = std::string("Hello, Concept!");
        if (a.is<std::string>()) {
            std::cout << "a = " << a.get<std::string>() << std::endl;
        }

        Any b = a;
        std::cout << "b = " << b.get<std::string>() << std::endl;

        a = 3.14159;
        std::cout << "a = " << a.get<double>() << std::endl;

        try {
            std::cout << a.get<std::string>() << std::endl;
        } catch (const std::bad_cast& e) {
            std::cerr << "Caught bad_cast: " << e.what() << std::endl;
        }
    }

}  // namespace ModelConcept

int main() { return 0; }