#include "queue.hpp"

void Queue::push(std::string_view value) {
    m_list.push_tail(value);
}

std::string Queue::pop() {
    if (m_list.getSize() == 0) {
        return {};
    }
    std::string value = m_list.get(0);
    m_list.del_head();
    return value;
}

std::string Queue::front() const {
    return m_list.get(0);
}

size_t Queue::size() const noexcept {
    return m_list.getSize();
}

bool Queue::empty() const noexcept {
    return m_list.getSize() == 0;
}

std::string Queue::print() const {
    if (empty()) {
        return "[] (front->back)";
    }
    // Используем твой метод print() из SinglyList
    return m_list.print() + " (front->back)";
}