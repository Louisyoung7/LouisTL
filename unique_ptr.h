#pragma once

#include <utility>

template <typename T>
class UniquePtr {
    T* ptr_;

   public:
    UniquePtr(T* ptr = nullptr) : ptr_(ptr) {}
    ~UniquePtr() {
        if (ptr_) { delete ptr_; }
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) { other.ptr_ = nullptr; }
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    T& operator*() const {
        if (ptr_) { return *ptr_; }
        return T();
    }
    T* operator->() const { return ptr_; }

    T* get() const { return ptr_; }

    void reset(T* ptr = nullptr) {
        if (ptr_) { delete ptr_; }
        ptr_ = ptr;
    }

    T* release() {
        T* temp = ptr_;
        ptr_ = nullptr;
        return temp;
    }
};

template <typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}