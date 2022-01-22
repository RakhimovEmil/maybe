#pragma once

namespace control_03 {
    template<typename T>
    class maybe {
    public:
        maybe()               : hasValue(false) {}
        maybe(const maybe& x) : hasValue(true) { new(buf) T(x.value()); }
        maybe(const T& value) : hasValue(true) { new(buf) T(value); }
        maybe(maybe&& x)      : hasValue(x.hasValue) { new(buf) T(std::move(x.value())); }
        maybe(T&& value)      : hasValue(true) { new(buf) T(std::move(value)); }

        void reset() {
            if (hasValue) {
                value().T::~T();
                hasValue = false;
            }
        }

        void reset(const T& value) {
            reset();
            new(buf) T(value);
            hasValue = true;
        }

        T&       value()       { return static_cast<T&>(*reinterpret_cast<T*>(buf)); }
        const T& value() const { return static_cast<const T&>(*reinterpret_cast<const T*>(buf)); }

        bool has_value() const { return hasValue; }

        maybe& operator=(const maybe& x) {
            if (this == &x) return *this;
            if (x.hasValue) {
                reset(x.value());
            } else {
                reset();
            }
            return *this;
        }

        maybe& operator=(const T& value) {
            reset(value);
            return *this;
        }

        maybe& operator=(maybe&& x)  noexcept {
            if (this == &x) return *this;
            if (x.hasValue) {
                if (hasValue) {
                    value() = std::move(x.value());
                } else {
                    hasValue = true;
                    new(buf) T(std::move(x.value()));
                }
            }
            return *this;
        }

        maybe& operator=(T&& x) {
            reset();
            hasValue = true;
            new(buf) T(std::move(x));
            return *this;
        }

        explicit operator bool() const { return hasValue; }

        template<typename... Args>
        void emplace(Args&&... args) {
            reset();
            new(buf) T(std::forward<Args>(args)...);
            hasValue = true;
        }

        ~maybe() { if (hasValue) value().T::~T(); }

    private:
        bool hasValue;
        alignas(T) u_char buf[sizeof(T)]{};
    };


    template<typename T>
    class maybe<T*> {
    public:
        maybe() : ptr(nullptr) {}

        maybe& operator=(std::nullptr_t) { впо
            ptr = nullptr;
            return *this;
        }

        void reset()     { ptr = nullptr;}
        void reset(T* x) { ptr = x; }

        T*       value()       { return ptr;}
        const T* value() const { return ptr;}

        bool has_value() const { return value() != nullptr;}

        explicit operator bool() const { return has_value(); }

    private:
        T* ptr;
    };
}