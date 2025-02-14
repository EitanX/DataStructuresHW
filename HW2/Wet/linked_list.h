#pragma once

#pragma once

template<class T>
class LinkedList {
    struct Node {
        T value;
        Node* next = nullptr;
        Node* prev = nullptr;

        explicit Node(const T& value);
        ~Node();
    };

public:
    class Iterator {
        friend class LinkedList;

    public:
        Iterator() = default;
        explicit Iterator(Node* node);

        Iterator& operator++();
        bool operator!=(const Iterator& other) const;
        T& operator*() const;

    private:
        Node* current = nullptr;
    };

private:
    Node* head = nullptr;
    Node* tail = nullptr;
    int size = 0;

public:
    ~LinkedList();

    Iterator insert(const T& value);
    void insertSymbolically(Iterator iterator);

    void remove(Iterator iterator);
    void removeSymbolically(Iterator iterator);

    int getSize() const;
    bool empty() const;

    Iterator begin() const;
    Iterator end() const;
};

// LinkedList

template<class T>
LinkedList<T>::~LinkedList() {
    delete this->head;
}

template<class T>
typename LinkedList<T>::Iterator LinkedList<T>::insert(const T& value) {
    Iterator iterator(new Node(value));
    this->insertSymbolically(iterator);
    return iterator;
}

template<class T>
void LinkedList<T>::insertSymbolically(Iterator iterator) {
    Node* node = iterator.current;
    if (this->tail != nullptr) {
        this->tail->next = node;
        node->prev = this->tail;
    } else {
        this->head = node;
    }
    this->tail = node;
    ++this->size;
}

template<class T>
void LinkedList<T>::remove(Iterator iterator) {
    this->removeSymbolically(iterator);
    delete iterator.current;
}

template<class T>
void LinkedList<T>::removeSymbolically(Iterator iterator) {
    Node* node = iterator.current;
    if (node == nullptr || head == nullptr) {
        return;
    }

    if (node->prev != nullptr) {
        node->prev->next = node->next;
    } else {
        head = node->next;
    }

    if (node->next != nullptr) {
        node->next->prev = node->prev;
    } else {
        tail = node->prev;
    }

    // Unlink the node completely
    node->prev = nullptr;
    node->next = nullptr;

    size--;
}

template<class T>
int LinkedList<T>::getSize() const {
    return this->size;
}

template<class T>
bool LinkedList<T>::empty() const {
    return this->size == 0;
}

template<class T>
typename LinkedList<T>::Iterator LinkedList<T>::begin() const {
    return Iterator(this->head);
}

template<class T>
typename LinkedList<T>::Iterator LinkedList<T>::end() const{
    return Iterator(nullptr);
}

// LinkedList::Node

template<class T>
LinkedList<T>::Node::Node(const T& value) : value(value)
{}

template<class T>
LinkedList<T>::Node::~Node() {
    delete this->next;
}

// LinkedList::Iterator

template<class T>
LinkedList<T>::Iterator::Iterator(Node *node) : current(node)
{}

template<class T>
typename LinkedList<T>::Iterator& LinkedList<T>::Iterator::operator++() {
    this->current = this->current->next;
    return *this;
}

template<class T>
bool LinkedList<T>::Iterator::operator!=(const Iterator& other) const {
    return this->current != other.current;
}

template<class T>
T& LinkedList<T>::Iterator::operator*() const {
    return this->current->value;
}
