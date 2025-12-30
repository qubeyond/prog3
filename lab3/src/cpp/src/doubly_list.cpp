#include "doubly_list.hpp"

DoublyList::~DoublyList() {
    clear();
}

void DoublyList::clear() noexcept {
    Node* current = m_head;
    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    m_head = m_tail = nullptr;
    m_size = 0;
}

DoublyList::DoublyList(const DoublyList& other) {
    Node* current = other.m_head;
    while (current) {
        push_tail(current->data);
        current = current->next;
    }
}

DoublyList& DoublyList::operator=(const DoublyList& other) {
    if (this != &other) {
        clear();
        Node* current = other.m_head;
        while (current) {
            push_tail(current->data);
            current = current->next;
        }
    }
    return *this;
}

DoublyList::DoublyList(DoublyList&& other) noexcept
    : m_head(other.m_head), m_tail(other.m_tail), m_size(other.m_size) {
    other.m_head = other.m_tail = nullptr;
    other.m_size = 0;
}

DoublyList& DoublyList::operator=(DoublyList&& other) noexcept {
    if (this != &other) {
        clear();
        m_head = other.m_head;
        m_tail = other.m_tail;
        m_size = other.m_size;
        other.m_head = other.m_tail = nullptr;
        other.m_size = 0;
    }
    return *this;
}

void DoublyList::push_head(std::string_view value) {
    auto* newNode = new Node(value);
    if (!m_head) {
        m_head = m_tail = newNode;
    } else {
        newNode->next = m_head;
        m_head->prev = newNode;
        m_head = newNode;
    }
    m_size++;
}

void DoublyList::push_tail(std::string_view value) {
    auto* newNode = new Node(value);
    if (!m_tail) {
        m_head = m_tail = newNode;
    } else {
        newNode->prev = m_tail;
        m_tail->next = newNode;
        m_tail = newNode;
    }
    m_size++;
}

bool DoublyList::del_head() {
    if (!m_head) return false;
    Node* temp = m_head;
    m_head = m_head->next;
    if (m_head)
        m_head->prev = nullptr;
    else
        m_tail = nullptr;
    delete temp;
    m_size--;
    return true;
}

bool DoublyList::del_tail() {
    if (!m_tail) return false;
    Node* temp = m_tail;
    m_tail = m_tail->prev;
    if (m_tail)
        m_tail->next = nullptr;
    else
        m_head = nullptr;
    delete temp;
    m_size--;
    return true;
}

bool DoublyList::del_value(std::string_view value) {
    Node* current = m_head;
    while (current) {
        if (current->data == value) {
            if (m_size == 1) {
                delete current;
                m_head = m_tail = nullptr;
                m_size = 0;
                return true;
            }
            if (current == m_head) return del_head();
            if (current == m_tail) return del_tail();
            current->prev->next = current->next;
            current->next->prev = current->prev;
            delete current;
            m_size--;
            return true;
        }
        current = current->next;
    }
    return false;
}

bool DoublyList::find(std::string_view value) const {
    Node* current = m_head;
    while (current) {
        if (current->data == value) return true;
        current = current->next;
    }
    return false;
}

std::string DoublyList::get(size_t index) const {
    if (index >= m_size) return "";
    Node* current = m_head;
    if (index > m_size / 2) {
        current = m_tail;
        for (size_t i = m_size - 1; i > index; --i) current = current->prev;
    } else {
        for (size_t i = 0; i < index; ++i) current = current->next;
    }
    return current->data;
}

std::string DoublyList::print() const {
    if (!m_head) return "[]";
    std::ostringstream oss;
    oss << "[";
    Node* current = m_head;
    while (current) {
        oss << current->data << (current->next ? " <-> " : "");
        current = current->next;
    }
    oss << "]";
    return oss.str();
}

bool DoublyList::insert_before_value(std::string_view target, std::string_view value) {
    Node* current = m_head;
    while (current) {
        if (current->data == target) {
            if (current == m_head) {
                push_head(value);
                return true;
            }
            auto* newNode = new Node(value);
            newNode->next = current;
            newNode->prev = current->prev;
            current->prev->next = newNode;
            current->prev = newNode;
            m_size++;
            return true;
        }
        current = current->next;
    }
    return false;
}

bool DoublyList::insert_after_value(std::string_view target, std::string_view value) {
    Node* current = m_head;
    while (current) {
        if (current->data == target) {
            if (current == m_tail) {
                push_tail(value);
                return true;
            }
            auto* newNode = new Node(value);
            newNode->prev = current;
            newNode->next = current->next;
            current->next->prev = newNode;
            current->next = newNode;
            m_size++;
            return true;
        }
        current = current->next;
    }
    return false;
}

bool DoublyList::insert_before_index(size_t index, std::string_view value) {
    if (index >= m_size) return false;
    if (index == 0) {
        push_head(value);
        return true;
    }
    Node* current = m_head;
    for (size_t i = 0; i < index; ++i) current = current->next;
    auto* newNode = new Node(value);
    newNode->next = current;
    newNode->prev = current->prev;
    current->prev->next = newNode;
    current->prev = newNode;
    m_size++;
    return true;
}

bool DoublyList::insert_after_index(size_t index, std::string_view value) {
    if (index >= m_size) return false;
    if (index == m_size - 1) {
        push_tail(value);
        return true;
    }
    Node* current = m_head;
    for (size_t i = 0; i < index; ++i) current = current->next;
    auto* newNode = new Node(value);
    newNode->prev = current;
    newNode->next = current->next;
    current->next->prev = newNode;
    current->next = newNode;
    m_size++;
    return true;
}

bool DoublyList::del_before_value(std::string_view target) {
    Node* current = m_head;
    while (current) {
        if (current->data == target) {
            if (!current->prev) return false;
            Node* toDelete = current->prev;
            if (toDelete == m_head) return del_head();
            toDelete->prev->next = current;
            current->prev = toDelete->prev;
            delete toDelete;
            m_size--;
            return true;
        }
        current = current->next;
    }
    return false;
}

bool DoublyList::del_after_value(std::string_view target) {
    Node* current = m_head;
    while (current) {
        if (current->data == target) {
            if (!current->next) return false;
            Node* toDelete = current->next;
            if (toDelete == m_tail) return del_tail();
            current->next = toDelete->next;
            toDelete->next->prev = current;
            delete toDelete;
            m_size--;
            return true;
        }
        current = current->next;
    }
    return false;
}

bool DoublyList::del_before_index(size_t index) {
    if (m_size == 0 || index == 0 || index >= m_size) return false;
    if (index == 1) return del_head();
    Node* current = m_head;
    for (size_t i = 0; i < index; ++i) current = current->next;
    Node* toDelete = current->prev;
    toDelete->prev->next = current;
    current->prev = toDelete->prev;
    delete toDelete;
    m_size--;
    return true;
}

bool DoublyList::del_after_index(size_t index) {
    if (m_size == 0 || index >= m_size - 1) return false;
    if (index == m_size - 2) return del_tail();
    Node* current = m_head;
    for (size_t i = 0; i < index; ++i) current = current->next;
    Node* toDelete = current->next;
    current->next = toDelete->next;
    toDelete->next->prev = current;
    delete toDelete;
    m_size--;
    return true;
}

void DoublyList::set(size_t index, std::string_view value) {
    if (index >= m_size) throw std::out_of_range("Index out of bounds");
    Node* current = m_head;
    if (index > m_size / 2) {
        current = m_tail;
        for (size_t i = m_size - 1; i > index; --i) current = current->prev;
    } else {
        for (size_t i = 0; i < index; ++i) current = current->next;
    }
    current->data = std::string(value);
}