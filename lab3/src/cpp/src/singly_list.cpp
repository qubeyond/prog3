#include "singly_list.hpp"

SinglyList::~SinglyList() {
    clear();
}

void SinglyList::clear() noexcept {
    Node* current = m_head;
    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    m_head = m_tail = nullptr;
    m_size = 0;
}

SinglyList::SinglyList(const SinglyList& other) {
    Node* current = other.m_head;
    while (current) {
        push_tail(current->data);
        current = current->next;
    }
}

SinglyList& SinglyList::operator=(const SinglyList& other) {
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

SinglyList::SinglyList(SinglyList&& other) noexcept
    : m_head(other.m_head), m_tail(other.m_tail), m_size(other.m_size) {
    other.m_head = other.m_tail = nullptr;
    other.m_size = 0;
}

SinglyList& SinglyList::operator=(SinglyList&& other) noexcept {
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

void SinglyList::push_head(std::string_view value) {
    auto* newNode = new Node(value);
    if (!m_head) {
        m_head = m_tail = newNode;
    } else {
        newNode->next = m_head;
        m_head = newNode;
    }
    m_size++;
}

void SinglyList::push_tail(std::string_view value) {
    auto* newNode = new Node(value);
    if (!m_tail) {
        m_head = m_tail = newNode;
    } else {
        m_tail->next = newNode;
        m_tail = newNode;
    }
    m_size++;
}

bool SinglyList::del_head() {
    if (!m_head) return false;
    Node* temp = m_head;
    m_head = m_head->next;
    if (!m_head) m_tail = nullptr;
    delete temp;
    m_size--;
    return true;
}

bool SinglyList::del_tail() {
    if (!m_head) return false;
    if (m_head == m_tail) return del_head();

    Node* current = m_head;
    while (current->next != m_tail) current = current->next;

    delete m_tail;
    m_tail = current;
    m_tail->next = nullptr;
    m_size--;
    return true;
}

bool SinglyList::del_value(std::string_view value) {
    if (!m_head) return false;
    if (m_head->data == value) return del_head();

    Node* current = m_head;
    while (current->next && current->next->data != value) current = current->next;

    if (current->next) {
        Node* toDelete = current->next;
        if (toDelete == m_tail) m_tail = current;
        current->next = toDelete->next;
        delete toDelete;
        m_size--;
        return true;
    }
    return false;
}

bool SinglyList::find(std::string_view value) const {
    Node* current = m_head;
    while (current) {
        if (current->data == value) return true;
        current = current->next;
    }
    return false;
}

std::string SinglyList::get(size_t index) const {
    if (index >= m_size) return "";
    Node* current = m_head;
    for (size_t i = 0; i < index; ++i) current = current->next;
    return current->data;
}

std::string SinglyList::print() const {
    if (!m_head) return "[]";
    std::ostringstream oss;
    oss << "[";
    Node* current = m_head;
    while (current) {
        oss << current->data << (current->next ? " -> " : "");
        current = current->next;
    }
    oss << "]";
    return oss.str();
}

bool SinglyList::insert_before_value(std::string_view target, std::string_view value) {
    if (!m_head) return false;
    if (m_head->data == target) {
        push_head(value);
        return true;
    }

    Node* current = m_head;
    while (current->next && current->next->data != target) current = current->next;

    if (current->next) {
        auto* newNode = new Node(value);
        newNode->next = current->next;
        current->next = newNode;
        m_size++;
        return true;
    }
    return false;
}

bool SinglyList::insert_after_value(std::string_view target, std::string_view value) {
    Node* current = m_head;
    while (current) {
        if (current->data == target) {
            auto* newNode = new Node(value);
            newNode->next = current->next;
            current->next = newNode;
            if (current == m_tail) m_tail = newNode;
            m_size++;
            return true;
        }
        current = current->next;
    }
    return false;
}

bool SinglyList::insert_before_index(size_t index, std::string_view value) {
    if (index >= m_size) return false;
    if (index == 0) {
        push_head(value);
        return true;
    }

    Node* current = m_head;
    for (size_t i = 0; i < index - 1; ++i) current = current->next;

    auto* newNode = new Node(value);
    newNode->next = current->next;
    current->next = newNode;
    m_size++;
    return true;
}

bool SinglyList::insert_after_index(size_t index, std::string_view value) {
    if (index >= m_size) return false;
    Node* current = m_head;
    for (size_t i = 0; i < index; ++i) current = current->next;

    auto* newNode = new Node(value);
    newNode->next = current->next;
    current->next = newNode;
    if (current == m_tail) m_tail = newNode;
    m_size++;
    return true;
}

bool SinglyList::del_before_value(std::string_view target) {
    if (!m_head || !m_head->next) return false;
    if (m_head->next->data == target) return del_head();

    Node* current = m_head;
    while (current->next && current->next->next) {
        if (current->next->next->data == target) {
            Node* toDelete = current->next;
            current->next = toDelete->next;
            delete toDelete;
            m_size--;
            return true;
        }
        current = current->next;
    }
    return false;
}

bool SinglyList::del_after_value(std::string_view target) {
    Node* current = m_head;
    while (current) {
        if (current->data == target && current->next) {
            Node* toDelete = current->next;
            current->next = toDelete->next;
            if (toDelete == m_tail) m_tail = current;
            delete toDelete;
            m_size--;
            return true;
        }
        current = current->next;
    }
    return false;
}

bool SinglyList::del_before_index(size_t index) {
    if (index == 0 || index >= m_size) return false;
    if (index == 1) return del_head();

    Node* current = m_head;
    for (size_t i = 0; i < index - 2; ++i) current = current->next;

    Node* toDelete = current->next;
    current->next = toDelete->next;
    delete toDelete;
    m_size--;
    return true;
}

bool SinglyList::del_after_index(size_t index) {
    if (index >= m_size - 1) return false;
    Node* current = m_head;
    for (size_t i = 0; i < index; ++i) current = current->next;

    Node* toDelete = current->next;
    current->next = toDelete->next;
    if (toDelete == m_tail) m_tail = current;
    delete toDelete;
    m_size--;
    return true;
}