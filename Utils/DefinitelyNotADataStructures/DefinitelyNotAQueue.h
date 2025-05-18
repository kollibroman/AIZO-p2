#ifndef DEFINITELYNOTAQUEUE_H
#define DEFINITELYNOTAQUEUE_H

template<typename T>
class DefinitelyNotAQueue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* front_;
    Node* rear_;
    size_t size_;

public:
    DefinitelyNotAQueue() : front_(nullptr), rear_(nullptr), size_(0) {}

    ~DefinitelyNotAQueue() {
        while (!empty()) {
            pop();
        }
    }

    // Copy constructor
    DefinitelyNotAQueue(const DefinitelyNotAQueue& other) : front_(nullptr), rear_(nullptr), size_(0) {
        Node* current = other.front_;
        while (current != nullptr) {
            push(current->data);
            current = current->next;
        }
    }

    // Move constructor
    DefinitelyNotAQueue(DefinitelyNotAQueue&& other) noexcept
        : front_(other.front_), rear_(other.rear_), size_(other.size_) {
        other.front_ = nullptr;
        other.rear_ = nullptr;
        other.size_ = 0;
    }

    // Copy assignment
    DefinitelyNotAQueue& operator=(const DefinitelyNotAQueue& other) {
        if (this != &other) {
            while (!empty()) {
                pop();
            }
            Node* current = other.front_;
            while (current != nullptr) {
                push(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    // Move assignment
    DefinitelyNotAQueue& operator=(DefinitelyNotAQueue&& other) noexcept {
        if (this != &other) {
            while (!empty()) {
                pop();
            }
            front_ = other.front_;
            rear_ = other.rear_;
            size_ = other.size_;
            other.front_ = nullptr;
            other.rear_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    void push(const T& value) {
        Node* newNode = new Node(value);
        if (empty()) {
            front_ = rear_ = newNode;
        } else {
            rear_->next = newNode;
            rear_ = newNode;
        }
        size_++;
    }

    void pop() {
        if (!empty()) {
            Node* temp = front_;
            front_ = front_->next;
            delete temp;
            size_--;
            if (empty()) {
                rear_ = nullptr;
            }
        }
    }

    T& front() { return front_->data; }
    const T& front() const { return front_->data; }

    bool empty() const { return size_ == 0; }
    size_t size() const { return size_; }
};


#endif //DEFINITELYNOTAQUEUE_H
