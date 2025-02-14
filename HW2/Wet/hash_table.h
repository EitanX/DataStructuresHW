#pragma once

#include "linked_list.h"

template<class T>
class HashTable {
    struct KeyValuePair {
        int key = 0;
        T* value = nullptr;

        KeyValuePair(const int key, T* value) : key(key), value(value) {}
    };
    using KeyValueList = LinkedList<KeyValuePair>;

    const int minimumArraySize = 8;

    int numElements = 0;
    int arraySize = minimumArraySize;
    KeyValueList* data = new KeyValueList[this->arraySize]();

public:
    ~HashTable();

    T* get(int key) const;
    void insert(int key, T* value);
    void update(int key, T* value);
    void remove(int key);

private:
    int hash(int key) const;
    void resize(int newSize);
};

// HashTable

template <class T>
HashTable<T>::~HashTable() {
    for (int i = 0; i < arraySize; ++i) {
        for (auto& kv : this->data[i]) {
            delete kv.value;
        }
    }
    delete[] data;
}

template <class T>
T* HashTable<T>::get(int key) const {
    for (auto kv : this->data[this->hash(key)]) {
        if (kv.key == key) {
            return kv.value;
        }
    }
    return nullptr;
}

template <class T>
void HashTable<T>::insert(int key, T* value) {
    if (this->arraySize == this->numElements) {
        this->resize(this->arraySize * 2);
    }

    this->data[this->hash(key)].insert(KeyValuePair(key, value));
    ++this->numElements;
}

template <class T>
void HashTable<T>::update(int key, T* value) {
    for (auto& kv : this->data[this->hash(key)]) {
        if (kv.key == key) {
            kv.value = value;
            return;
        }
    }
}

template <class T>
void HashTable<T>::remove(int key) {
    if (this->arraySize == this->numElements / 4) {
        this->resize(this->arraySize / 2);
    }

    auto& list = this->data[this->hash(key)];
    auto it = list.begin();
    for (; it != list.end(); ++it) {
        if ((*it).key == key) {
            break;
        }
    }
    list.remove(it);
    --this->numElements;
}

template <class T>
int HashTable<T>::hash(int key) const {
    return key % this->arraySize;
}

template <class T>
void HashTable<T>::resize(const int newSize) {
    if (newSize < minimumArraySize) {
        return;
    }

    const int oldSize = this->arraySize;

    auto oldData = this->data;
    auto newData = new KeyValueList[newSize]();

    // Do this now, so that the hash function will use the new value
    this->arraySize = newSize;

    for (int i = 0; i < oldSize; i++) {
        for (auto& kv : oldData[i]) {
            newData[this->hash(kv.key)].insert(kv);
        }
    }

    delete[] oldData;
    this->data = newData;
}
