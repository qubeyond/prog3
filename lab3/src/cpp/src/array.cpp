#include "array.hpp"

Array::Array(size_t initial_capacity)
    : data(std::make_unique<std::string[]>(initial_capacity)),
      m_size(0),
      m_capacity(initial_capacity) {
}

Array::Array(const Array& other)
    : data(std::make_unique<std::string[]>(other.m_capacity)),
      m_size(other.m_size),
      m_capacity(other.m_capacity) {
    for (size_t i = 0; i < m_size; ++i) data[i] = other.data[i];
}

Array& Array::operator=(const Array& other) {
    if (this != &other) {
        auto new_data = std::make_unique<std::string[]>(other.m_capacity);
        for (size_t i = 0; i < other.m_size; ++i) new_data[i] = other.data[i];
        data = std::move(new_data);
        m_size = other.m_size;
        m_capacity = other.m_capacity;
    }
    return *this;
}

void Array::resize(size_t new_capacity) {
    auto new_data = std::make_unique<std::string[]>(new_capacity);
    for (size_t i = 0; i < m_size; ++i) new_data[i] = std::move(data[i]);
    data = std::move(new_data);
    m_capacity = new_capacity;
}

void Array::push(std::string_view value) {
    if (m_size >= m_capacity) resize(m_capacity == 0 ? 1 : m_capacity * 2);
    data[m_size++] = std::string(value);
}

void Array::insert(size_t index, std::string_view value) {
    if (index > m_size) throw std::out_of_range("Index out of bounds");
    if (m_size >= m_capacity) resize(m_capacity == 0 ? 1 : m_capacity * 2);
    for (size_t i = m_size; i > index; --i) data[i] = std::move(data[i - 1]);
    data[index] = std::string(value);
    m_size++;
}

const std::string& Array::get(size_t index) const {
    if (index >= m_size) throw std::out_of_range("Index out of bounds");
    return data[index];
}

void Array::set(size_t index, std::string_view value) {
    if (index >= m_size) throw std::out_of_range("Index out of bounds");
    data[index] = std::string(value);
}

bool Array::remove(size_t index) {
    if (index >= m_size) return false;
    for (size_t i = index; i < m_size - 1; ++i) data[i] = std::move(data[i + 1]);
    m_size--;
    if (m_size > 0 && m_size < m_capacity / 4 && m_capacity > 4) resize(m_capacity / 2);
    return true;
}

std::string Array::print() const {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < m_size; ++i) {
        oss << data[i] << (i < m_size - 1 ? ", " : "");
    }
    oss << "]";
    return oss.str();
}