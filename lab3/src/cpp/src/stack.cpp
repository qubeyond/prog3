#include "stack.hpp"

void Stack::push(std::string_view value) {
    m_list.push_head(value);
}

std::string Stack::pop() {
    if (m_list.getSize() == 0) {
        return {};
    }
    std::string value = m_list.get(0);
    m_list.del_head();
    return value;
}

std::string Stack::top() const {
    return m_list.get(0);
}

size_t Stack::size() const noexcept {
    return m_list.getSize();
}

bool Stack::empty() const noexcept {
    return m_list.getSize() == 0;
}

std::string Stack::print() const {
    if (empty()) {
        return "[] (top->bottom)";
    }
    return m_list.print() + " (top->bottom)";
}