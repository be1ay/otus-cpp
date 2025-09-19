#pragma once
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <utility>
#include <cstddef>
#include <new>

// =====================
// Общее состояние пула
// =====================
struct SharedState {
    void* base = nullptr;             // начало пула
    std::size_t capacity_bytes = 0;   // всего байт
    std::size_t used_bytes = 0;       // занято байт
};

// =====================
// FixedPoolAllocator аллокатор с общим пулом
// =====================
template <typename T, std::size_t N>
class FixedPoolAllocator {
public:
    using value_type = T;

    FixedPoolAllocator() {
        state_ = std::make_shared<SharedState>();
        state_->capacity_bytes = N * sizeof(T);
        state_->used_bytes = 0;
        void* raw = ::operator new(state_->capacity_bytes, std::align_val_t(alignof(T)));
        state_->base = raw;
        holder_ = std::shared_ptr<void>(raw, [](void* p) {
            ::operator delete(p, std::align_val_t(alignof(T)));
        });
    }

    T* allocate(std::size_t n) {
        std::size_t bytes_needed = n * sizeof(T);
        if (state_->used_bytes + bytes_needed > state_->capacity_bytes)
            throw std::bad_alloc();
        char* p = static_cast<char*>(state_->base) + state_->used_bytes;
        state_->used_bytes += bytes_needed;
        return reinterpret_cast<T*>(p);
    }

    void deallocate(T*, std::size_t) noexcept { /* не требуется */ }

    template <typename U> 
    struct rebind { using other = FixedPoolAllocator<U, N>; };

    template <typename U>
    FixedPoolAllocator(const FixedPoolAllocator<U, N>& other) noexcept
        : state_(other.state_), holder_(other.holder_) {}

private:
    std::shared_ptr<SharedState> state_;
    std::shared_ptr<void> holder_;
};


// =====================
// MyContainer односвязный список
// =====================
template <typename T, typename Alloc = std::allocator<T>>
class MyContainer {
private:
    struct Node {
        T value;
        Node* next;
    };

    using NodeAlloc = typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;
    using NodeTraits = std::allocator_traits<NodeAlloc>;

    Node* head = nullptr;
    NodeAlloc alloc;
    std::size_t count = 0;

public:
    MyContainer() = default;
    ~MyContainer() { 
        clear(); 
    }
    MyContainer(const MyContainer&) = delete;
    MyContainer& operator=(const MyContainer&) = delete;


    void push_back(const T& val) {
        Node* newNode = NodeTraits::allocate(alloc, 1);
        NodeTraits::construct(alloc, newNode, Node{val, nullptr});

        if (!head) {
            head = newNode;
        } else {
            Node* cur = head;
            while (cur->next) cur = cur->next;
            cur->next = newNode;
        }
        ++count;
    }

    void clear() {
        Node* cur = head;
        while (cur) {
            Node* next = cur->next;
            NodeTraits::destroy(alloc, cur);
            NodeTraits::deallocate(alloc, cur, 1);
            cur = next;
        }
        head = nullptr;
        count = 0;
    }

    struct iterator {
        Node* ptr;
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator(Node* p) : ptr(p) {}
        T& operator*() const { return ptr->value; }
        iterator& operator++() { ptr = ptr->next; return *this; }
        bool operator!=(const iterator& other) const { return ptr != other.ptr; }
    };

    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }

    std::size_t size() const { return count; }
    bool empty() const { return count == 0; }
};