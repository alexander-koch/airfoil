#ifndef __OBJECT_POOL_HPP__
#define __OBJECT_POOL_HPP__

#include <cstddef>
#include <vector>
#include <iostream>

template <typename T>
struct Node {
    Node* prev;
    Node* next;
    T value;
    Node(T val) {
        value = val;
        prev = next = NULL;
    }
};

template <typename T>
class ObjectPool {
public:
    ObjectPool();
    ObjectPool(std::vector<T> v);
    ~ObjectPool();
    void append(T val);
    void buildCircular();
    void revertCircular();
    bool empty();
    Node<T>* getFront();

    Node<T>* front;
private:
    Node<T>* back;
};

template <typename T>
ObjectPool<T>::ObjectPool() : front(NULL), back(NULL) {}

template <typename T>
ObjectPool<T>::ObjectPool(std::vector<T> v) : front(NULL), back(NULL) {
    for(unsigned i = 0; i < v.size(); i++) {
        append(v[i]);
    }
}

template <typename T>
ObjectPool<T>::~ObjectPool() {
    int count = 0;
    Node<T>* node = front;
    while(node != NULL) {
        Node<T>* tmp = node;
        node = tmp->next;
        delete tmp;
        count++;
    }
    front = NULL;
    back = NULL;
    std::cout << "Freed (" << count << ") objects" << std::endl;
}

template <typename T>
void ObjectPool<T>::append(T val) {
    Node<T>* node = new Node<T>(val);
    if(back == NULL) {
        front = node;
        back = node;
    } else {
        back->next = node;
        node->prev = back;
        back = node;
    }
}

template <typename T>
void ObjectPool<T>::buildCircular() {
    back->next = front;
    front->prev = back;
}

template <typename T>
void ObjectPool<T>::revertCircular() {
    Node<T>* node = front;
    while(node->next != front) {
        node = node->next;
    }
    node->next = NULL;
    front->prev = NULL;
}

template <typename T>
bool ObjectPool<T>::empty() {
    return back == NULL;
}

template <typename T>
Node<T>* ObjectPool<T>::getFront() {
    return front;
}

#endif
