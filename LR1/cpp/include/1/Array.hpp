#pragma once

#include <iostream>
#include <functional>
#include <algorithm>
#include <stdexcept>

using namespace std;

template<typename T>
class Array {
public:
    explicit Array(int n = 4)
        : data(nullptr), length(0), capacity(0) 
    {
        if (n <= 0) throw out_of_range("Size must be > 0");

        data = new T[n];
        capacity = n;
    }

    ~Array() { delete[] data; }

    // Копирующий конструктор
    Array(const Array& other)
        : data(new T[other.capacity]), length(other.length), capacity(other.capacity)
    {
        for (int i = 0; i < length; ++i) data[i] = other.data[i];
    }

    // Перемещающий конструктор
    Array(Array&& other) noexcept
        : data(other.data), length(other.length), capacity(other.capacity)
    {
        other.data = nullptr;
        other.length = 0;
        other.capacity = 0;
    }

    // Копирующее присваивание
    Array& operator=(const Array& other) 
    {
        if (this == &other) return *this;

        T* new_data = new T[other.capacity];
        for (int i = 0; i < other.length; ++i) new_data[i] = other.data[i];

        delete[] data;

        data = new_data;
        length = other.length;
        capacity = other.capacity;

        return *this;
    }

    // Перемещающее присваивание
    Array& operator=(Array&& other) noexcept 
    {
        if (this == &other) return *this;

        delete[] data;

        data = other.data;
        length = other.length;
        capacity = other.capacity;
        other.data = nullptr;
        other.length = 0;
        other.capacity = 0;

        return *this;
    }

    int size() const noexcept 
    { 
        return length; 
    }

    void push(const T& value) 
    {
        if (length == capacity) resize(max(1, capacity * 2));
        data[length++] = value;
    }

    T get(int index) const 
    { 
        check_index(index); 
        return data[index]; 
    }

    void set(int index, const T& value) 
    { 
        check_index(index); 
        data[index] = value; 
    }

    T remove(int index) 
    {
        check_index(index);

        T val = data[index];
        for (int i = index; i < length - 1; ++i) data[i] = data[i + 1];

        --length;

        if (length > 0 && length < capacity / 4) resize(max(1, capacity / 2));
        return val;
    }

    void insert(int index, const T& value) 
    {
        check_index(index, true);

        if (length == capacity) resize(max(1, capacity * 2));

        for (int i = length - 1; i >= index; --i) data[i + 1] = data[i];

        data[index] = value;
        ++length;
    }

    friend ostream& operator<<(ostream& os, const Array& arr) 
    {
        os << "Array(capacity=" << arr.capacity << ", length=" << arr.length << ") : [";
        for (int i = 0; i < arr.length; ++i) {
            os << arr.data[i];
            if (i != arr.length - 1) os << ", ";
        }
        os << "]";

        return os;
    }

private:
    T* data;
    int length;
    int capacity;

    void resize(int new_cap) 
    {
        if (new_cap < 1) new_cap = 1;
        if (new_cap < length) new_cap = length;
        if (new_cap == capacity) return;

        T* temp = new T[new_cap];
        for (int i = 0; i < length; ++i) temp[i] = data[i];

        delete[] data;

        data = temp;
        capacity = new_cap;
    }

    void check_index(int index, bool allow_end = false) const 
    {
        if (allow_end) {
            if (index < 0 || index > length) throw out_of_range("Index out of range");
        } 
        else {
            if (index < 0 || index >= length) throw out_of_range("Index out of range");
        }
    }
};