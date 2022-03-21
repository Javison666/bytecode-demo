#pragma once

// bytecode.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
#define  _CRT_SECURE_NO_WARNINGS
#define STATIC_ASSERT(test) static_assert(test, #test)

#if !defined(DEBUG) && V8_HAS_ATTRIBUTE_ALWAYS_INLINE
# define V8_INLINE inline __attribute__((always_inline))
#elif !defined(DEBUG) && V8_HAS___FORCEINLINE
# define V8_INLINE __forceinline
#else
# define V8_INLINE inline
#endif

// The DCHECK macro is equivalent to CHECK except that it only
// generates code in debug builds.
#ifdef DEBUG
#define DCHECK_EQ(lhs, rhs) DCHECK_OP(EQ, ==, lhs, rhs)
#define DCHECK_NE(lhs, rhs) DCHECK_OP(NE, !=, lhs, rhs)
#define DCHECK_GT(lhs, rhs) DCHECK_OP(GT, >, lhs, rhs)
#define DCHECK_GE(lhs, rhs) DCHECK_OP(GE, >=, lhs, rhs)
#define DCHECK_LT(lhs, rhs) DCHECK_OP(LT, <, lhs, rhs)
#define DCHECK_LE(lhs, rhs) DCHECK_OP(LE, <=, lhs, rhs)
#define DCHECK_NULL(val) DCHECK((val) == nullptr)
#define DCHECK_NOT_NULL(val) DCHECK((val) != nullptr)
#define DCHECK_IMPLIES(lhs, rhs) \
  DCHECK_WITH_MSG(!(lhs) || (rhs), #lhs " implies " #rhs)
#else
#define DCHECK(condition)      ((void) 0)
#define DCHECK_EQ(v1, v2)      ((void) 0)
#define DCHECK_NE(v1, v2)      ((void) 0)
#define DCHECK_GT(v1, v2)      ((void) 0)
#define DCHECK_GE(v1, v2)      ((void) 0)
#define DCHECK_LT(v1, v2)      ((void) 0)
#define DCHECK_LE(v1, v2)      ((void) 0)
#define DCHECK_NULL(val)       ((void) 0)
#define DCHECK_NOT_NULL(val)   ((void) 0)
#define DCHECK_IMPLIES(v1, v2) ((void) 0)
#endif

#include <iostream>;
#include "deps/v8/src/utils/memcopy.h";


size_t hash_combine(size_t seed, size_t value);

template <typename T>
V8_INLINE size_t hash_value_unsigned(T v) {
    switch (sizeof(T)) {
    case 4: {
        // "32 bit Mix Functions"
        v = ~v + (v << 15);  // v = (v << 15) - v - 1;
        v = v ^ (v >> 12);
        v = v + (v << 2);
        v = v ^ (v >> 4);
        v = v * 2057;  // v = (v + (v << 3)) + (v << 11);
        v = v ^ (v >> 16);
        return static_cast<size_t>(v);
    }
    case 8: {
        switch (sizeof(size_t)) {
        case 4: {
            // "64 bit to 32 bit Hash Functions"
            v = ~v + (v << 18);  // v = (v << 18) - v - 1;
            v = v ^ (v >> 31);
            v = v * 21;  // v = (v + (v << 2)) + (v << 4);
            v = v ^ (v >> 11);
            v = v + (v << 6);
            v = v ^ (v >> 22);
            return static_cast<size_t>(v);
        }
        case 8: {
            // "64 bit Mix Functions"
            v = ~v + (v << 21);  // v = (v << 21) - v - 1;
            v = v ^ (v >> 24);
            v = (v + (v << 3)) + (v << 8);  // v * 265
            v = v ^ (v >> 14);
            v = (v + (v << 2)) + (v << 4);  // v * 21
            v = v ^ (v >> 28);
            v = v + (v << 31);
            return static_cast<size_t>(v);
        }
        }
    }
    }
    //UNREACHABLE();
}

template <typename T>
class Vector {
public:
    using value_type = T;
    using iterator = T*;
    using const_iterator = const T*;

    constexpr Vector() : start_(nullptr), length_(0) {}

    constexpr Vector(T* data, size_t length) : start_(data), length_(length) {
        DCHECK(length == 0 || data != nullptr);
    }

    static Vector<T> New(size_t length) {
        return Vector<T>(new T[length], length);
    }

    // Returns a vector using the same backing storage as this one,
    // spanning from and including 'from', to but not including 'to'.
    Vector<T> SubVector(size_t from, size_t to) const {
        DCHECK_LE(from, to);
        DCHECK_LE(to, length_);
        return Vector<T>(begin() + from, to - from);
    }

    // Returns the length of the vector. Only use this if you really need an
    // integer return value. Use {size()} otherwise.
    int length() const {
        DCHECK_GE(std::numeric_limits<int>::max(), length_);
        return static_cast<int>(length_);
    }

    // Returns the length of the vector as a size_t.
    constexpr size_t size() const { return length_; }

    // Returns whether or not the vector is empty.
    constexpr bool empty() const { return length_ == 0; }

    // Access individual vector elements - checks bounds in debug mode.
    T& operator[](size_t index) const {
        DCHECK_LT(index, length_);
        return start_[index];
    }

    const T& at(size_t index) const { return operator[](index); }

    T& first() { return start_[0]; }

    T& last() {
        DCHECK_LT(0, length_);
        return start_[length_ - 1];
    }

    // Returns a pointer to the start of the data in the vector.
    constexpr T* begin() const { return start_; }

    // For consistency with other containers, do also provide a {data} accessor.
    constexpr T* data() const { return start_; }

    // Returns a pointer past the end of the data in the vector.
    constexpr T* end() const { return start_ + length_; }

    // Returns a clone of this vector with a new backing store.
    Vector<T> Clone() const {
        T* result = new T[length_];
        for (size_t i = 0; i < length_; i++) result[i] = start_[i];
        return Vector<T>(result, length_);
    }

    void Truncate(size_t length) {
        DCHECK(length <= length_);
        length_ = length;
    }

    // Releases the array underlying this vector. Once disposed the
    // vector is empty.
    void Dispose() {
        delete[] start_;
        start_ = nullptr;
        length_ = 0;
    }

    Vector<T> operator+(size_t offset) {
        DCHECK_LE(offset, length_);
        return Vector<T>(start_ + offset, length_ - offset);
    }

    Vector<T> operator+=(size_t offset) {
        DCHECK_LE(offset, length_);
        start_ += offset;
        length_ -= offset;
        return *this;
    }

    // Implicit conversion from Vector<T> to Vector<const T>.
    operator Vector<const T>() const { return { start_, length_ }; }

    template <typename S>
    static Vector<T> cast(Vector<S> input) {
        // Casting is potentially dangerous, so be really restrictive here. This
        // might be lifted once we have use cases for that.
        STATIC_ASSERT(std::is_pod<S>::value);
        STATIC_ASSERT(std::is_pod<T>::value);
        DCHECK_EQ(0, (input.size() * sizeof(S)) % sizeof(T));
        DCHECK_EQ(0, reinterpret_cast<uintptr_t>(input.begin()) % alignof(T));
        return Vector<T>(reinterpret_cast<T*>(input.begin()),
            input.size() * sizeof(S) / sizeof(T));
    }

    bool operator==(const Vector<const T> other) const {
        return std::equal(begin(), end(), other.begin(), other.end());
    }

    bool operator!=(const Vector<const T> other) const {
        return !operator==(other);
    }

private:
    T* start_;
    size_t length_;
};

template <typename T>
T* NewArray(size_t size) {
    T* result = new (std::nothrow) T[size];
    //if (result == nullptr) {
    //    V8::GetCurrentPlatform()->OnCriticalMemoryPressure();
    //    result = new (std::nothrow) T[size];
    //    if (result == nullptr) FatalProcessOutOfMemory(nullptr, "NewArray");
    //}
    return result;
}