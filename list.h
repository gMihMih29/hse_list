#pragma once

#include <cstdint>
#include <memory>
#include <utility>

namespace ListSolution {

template <typename T>
class List {
public:
    class ListNode {
    public:
        friend class List;
        friend class Iterator;

    public:
        ListNode() : value_ptr_(nullptr), next_(this), prev_(this) {
        }

        explicit ListNode(const T& value) : value_ptr_(std::make_unique<T>(value)), next_(nullptr), prev_(nullptr) {
        }

        explicit ListNode(T&& value) : value_ptr_(std::make_unique<T>(std::move(value))), next_(nullptr), prev_(nullptr) {
        }

        T& GetValue() {
            return *value_ptr_;
        }

    private:
        std::unique_ptr<T> value_ptr_;
        ListNode* next_;
        ListNode* prev_;
    };

    class Iterator {
    public:
        explicit Iterator(ListNode* ptr) : ptr_(ptr) {}

        Iterator& operator++() {
            ptr_ = ptr_->next_;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp(ptr_);
            ++(*this);
            return tmp;
        }

        Iterator& operator--() {
            ptr_ = ptr_->prev_;
            return *this;
        }

        Iterator operator--(int) {
            Iterator tmp(ptr_);
            --(*this);
            return tmp;
        }

        T& operator*() const { return ptr_->GetValue(); }

        T* operator->() const { return ptr_->value_ptr_; }

        bool operator==(const Iterator& rhs) const {
            return ptr_->value_ptr_ == rhs.ptr_->value_ptr_;
        }

        bool operator!=(const Iterator& rhs) const { return !(*this == rhs); }

    private:
        ListNode* ptr_;
    };

public:
    List() : linkage_(new ListNode()), size_(0) {
    }

    List(const List& other) : List() {
        for (const T& value : other) {
            PushBack(value);
        }
    }

    List(List&& other) : List() {
        std::swap(linkage_, other.linkage_);
    }

    ~List() {
        while (!IsEmpty()) {
            PopBack();
        }
        delete linkage_;
    }

    List& operator=(const List& rhv) {
        List tmp(rhv);
        SwapWith(tmp);
        return *this;
    }

    List& operator=(List&& rhv) {
        List tmp(std::move(rhv));
        SwapWith(tmp);
        return *this;
    }

    bool IsEmpty() const {
        return linkage_->prev_ == linkage_ && linkage_->next_ == linkage_;
    }

    std::size_t Size() const {
        return size_;
    }

    void PushBack(const T& elem) {
        ListNode* new_node = new ListNode(elem);
        LinkAfter(new_node, linkage_->prev_);
    }

    void PushBack(T&& elem) {
        ListNode* new_node = new ListNode(std::move(elem));
        LinkAfter(new_node, linkage_->prev_);
    }

    void PushFront(const T& elem) {
        ListNode* new_node = new ListNode(elem);
        LinkAfter(new_node, linkage_);
    }

    void PushFront(T&& elem) {
        ListNode* new_node = new ListNode(std::move(elem));
        LinkAfter(new_node, linkage_);
    }

    T& Front() {
        return linkage_->next_->GetValue();
    }

    const T& Front() const {
        return linkage_->next_->GetValue();
    }

    T& Back() {
        return linkage_->prev_->GetValue();
    }

    const T& Back() const {
        return linkage_->prev_->GetValue();
    }

    void PopBack() {
        UnLink(linkage_->prev_);
    }

    void PopFront() {
        UnLink(linkage_->next_);
    }

    void SwapWith(List& other){
        std::swap(linkage_, other.linkage_);
    }

    Iterator Begin() {
        return Iterator(linkage_->next_);
    }

    Iterator End() {
        return Iterator(linkage_);
    }

    Iterator Begin() const {
        return Iterator(linkage_->next_);
    }

    Iterator End() const {
        return Iterator(linkage_);
    }

    Iterator begin() {  // NOLINT
        return Begin();
    }

    Iterator end() {  // NOLINT
        return End();
    }

    Iterator begin() const {  // NOLINT
        return Begin();
    }

    Iterator end() const {  // NOLINT
        return End();
    }

private:
    ListNode* linkage_;
    std::size_t size_;

    void UnLink(ListNode* node) {
        if (node == linkage_) {
            return;
        }
        ListNode* prev = node->prev_;
        ListNode* next = node->next_;
        prev->next_ = next;
        next->prev_ = prev;
        node->prev_ = nullptr;
        node->next_ = nullptr;
        delete node;
    }

    void LinkAfter(ListNode* target, ListNode* after) {
        after->next_->prev_ = target;
        target->next_ = after->next_;
        after->next_ = target;
        target->prev_ = after;
        ++size_;
    }
};

template <typename T>
typename List<T>::Iterator begin(List<T>& list) {  // NOLINT
    return list.Begin();
}

template <typename T>
typename List<T>::Iterator end(List<T>& list) {  // NOLINT
    return list.End();
}
}  // namespace ListSolution
