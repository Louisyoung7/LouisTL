#pragma once

#include <cstddef>

template <typename T>
class SharedPtr {
    T* ptr_;
    size_t* refCount_;

   public:
    explicit SharedPtr(T* ptr = nullptr) : ptr_(ptr), refCount_(new size_t(1)) {}
    ~SharedPtr() {
        if (--(*refCount_) == 0) {
            delete ptr_;
            delete refCount_;
        }
    }

    SharedPtr(const SharedPtr& other) : ptr_(other.ptr_), refCount_(other.refCount_) { ++(*refCount_); }
    SharedPtr& operator=(const SharedPtr& other) {
        // 避免自赋值
        if (this != &other) {
            // 释放旧的资源
            if (--(*refCount_) == 0) {
                delete ptr_;
                delete refCount_;
            }
            // 赋值新的资源
            ptr_ = other.ptr_;
            refCount_ = other.refCount_;
            ++(*refCount_);
        }
        return *this;
    }

    T& operator*() const {
        if (ptr_) { return *ptr_; }
        return T();
    }
    T* operator->() const { return ptr_; }

    size_t use_count() const { return *refCount_; }
    void reset(T* ptr = nullptr) {
        if (ptr == ptr_) { return; }
        if (ptr_) {
            delete ptr_;
            delete refCount_;
        }
        ptr_ = ptr;
        refCount_ = (ptr ? new size_t(1) : nullptr);
    }
};