#include "doublyList.h"
#include <iostream>
#include <stdexcept>

using namespace std;

DoublyListNode::DoublyListNode(const std::string& value)
    : data(value), next(nullptr), prev(nullptr) {}

std::string DoublyListNode::getData() const { return data; }
DoublyListNode* DoublyListNode::getNext() const { return next; }
DoublyListNode* DoublyListNode::getPrev() const { return prev; }
void DoublyListNode::setNext(DoublyListNode* node) { next = node; }
void DoublyListNode::setPrev(DoublyListNode* node) { prev = node; }

DoublyList::DoublyList() : head(nullptr), tail(nullptr) {}

DoublyList::~DoublyList() {
    while (head != nullptr) {
        const DoublyListNode* temp = head;
        head = head->getNext();
        delete temp;
    }
    tail = nullptr;
}

void DoublyList::addHead(const std::string& value) {
    if (value.empty()) throw std::runtime_error("Значение пустое");

    auto* newNode = new DoublyListNode(value);
    newNode->setNext(head);

    if (head != nullptr) {
        head->setPrev(newNode);
    } else {
        tail = newNode;
    }
    head = newNode;
}

void DoublyList::addBack(const std::string& value) {
    if (value.empty()) throw std::runtime_error("Значение пустое");

    auto* newNode = new DoublyListNode(value);
    newNode->setPrev(tail);

    if (tail != nullptr) {
        tail->setNext(newNode);
    } else {
        head = newNode;
    }
    tail = newNode;
}

void DoublyList::addAfterIndex(const int index, const std::string& value) {
    if (index < 0) throw std::runtime_error("Индекс меньше 0");
    if (value.empty()) throw std::runtime_error("Значение пустое");

    if (head == nullptr) {
        if (index == 0) {
            addHead(value);
            return;
        }
        throw std::runtime_error("Список пуст, индекс недоступен");
    }

    DoublyListNode* current = head;
    for (int i = 0; i < index && current != nullptr; ++i) {
        current = current->getNext();
    }

    if (current == nullptr) throw std::runtime_error("Индекс превышает размер списка");

    auto* newNode = new DoublyListNode(value);
    newNode->setNext(current->getNext());
    newNode->setPrev(current);

    if (current->getNext() != nullptr) {
        current->getNext()->setPrev(newNode);
    } else {
        tail = newNode;
    }
    current->setNext(newNode);
}

void DoublyList::addBeforeIndex(const int index, const std::string &value) {
    if (index < 0) throw std::runtime_error("Индекс меньше 0");
    if (value.empty()) throw std::runtime_error("Значение пустое");

    if (head == nullptr) {
        if (index == 0) {
            addHead(value);
            return;
        }
        throw std::runtime_error("Список пуст, индекс недоступен");
    }

    if (index == 0) {
        addHead(value);
        return;
    }

    DoublyListNode* current = head;
    for (int i = 0; i < index - 1 && current != nullptr; ++i) {
        current = current->getNext();
    }

    if (current == nullptr) throw std::runtime_error("Индекс превышает размер списка");

    auto* newNode = new DoublyListNode(value);
    newNode->setNext(current->getNext());
    newNode->setPrev(current);

    if (current->getNext() != nullptr) {
        current->getNext()->setPrev(newNode);
    } else {
        tail = newNode;
    }
    current->setNext(newNode);
}

void DoublyList::dellHead() {
    if (head == nullptr) throw std::runtime_error("Список пустой");

    const DoublyListNode* temp = head;
    head = head->getNext();
    if (head != nullptr) {
        head->setPrev(nullptr);
    } else {
        tail = nullptr;
    }
    delete temp;
}

void DoublyList::dellBack() {
    if (tail == nullptr) throw std::runtime_error("Список пустой");

    const DoublyListNode* temp = tail;
    tail = tail->getPrev();
    if (tail != nullptr) {
        tail->setNext(nullptr);
    } else {
        head = nullptr;
    }
    delete temp;
}

void DoublyList::dellAfterIndex(const int index) {
    if (head == nullptr) throw std::runtime_error("Список пустой");
    if (index < 0) throw std::runtime_error("Индекс меньше 0");

    DoublyListNode* current = head;
    for (int i = 0; i < index && current != nullptr; ++i) {
        current = current->getNext();
    }

    if (current == nullptr || current->getNext() == nullptr)
        throw std::runtime_error("Нет элемента после индекса");

    const DoublyListNode* toDelete = current->getNext();
    current->setNext(toDelete->getNext());
    if (toDelete->getNext() != nullptr) {
        toDelete->getNext()->setPrev(current);
    } else {
        tail = current;
    }
    delete toDelete;
}

void DoublyList::dellBeforeIndex(const int index) {
    if (head == nullptr) throw std::runtime_error("Список пустой");
    if (index <= 0) throw std::runtime_error("Нельзя удалить перед индексом 0");
    if (index == 1) {
        dellHead();
        return;
    }

    DoublyListNode* current = head;
    for (int i = 0; i < index - 2 && current != nullptr; ++i) {
        current = current->getNext();
    }

    if (current == nullptr || current->getNext() == nullptr || current->getNext()->getNext() == nullptr)
        throw std::runtime_error("Нет элемента перед индексом");

    const DoublyListNode* toDelete = current->getNext();
    current->setNext(toDelete->getNext());
    toDelete->getNext()->setPrev(current);
    delete toDelete;
}

void DoublyList::dellValue(const std::string& value) {
    if (head == nullptr) return;
    if (value.empty()) throw std::runtime_error("Значение пустое");

    if (head->getData() == value) {
        dellHead();
        return;
    }

    const DoublyListNode* current = head;
    while (current != nullptr && current->getData() != value) {
        current = current->getNext();
    }

    if (current == nullptr) return;

    if (current == tail) {
        dellBack();
        return;
    }

    current->getPrev()->setNext(current->getNext());
    current->getNext()->setPrev(current->getPrev());
    delete current;
}

bool DoublyList::searchValue(const std::string& value) const {
    if (value.empty()) throw std::runtime_error("Значение пустое");

    const DoublyListNode* current = head;
    while (current != nullptr) {
        if (current->getData() == value) return true;
        current = current->getNext();
    }
    return false;
}

void DoublyList::print() const {
    if (head == nullptr) {
        cout << "(Список пустой)" << endl;
        return;
    }
    const DoublyListNode* current = head;
    while (current != nullptr) {
        cout << current->getData();
        if (current->getNext() != nullptr) cout << " <-> ";
        current = current->getNext();
    }
    cout << endl;
}

void DoublyList::printReverse() const {
    if (tail == nullptr) {
        cout << "(Список пустой)" << endl;
        return;
    }
    const DoublyListNode* current = tail;
    while (current != nullptr) {
        cout << current->getData();
        if (current->getPrev() != nullptr) cout << " <-> ";
        current = current->getPrev();
    }
    cout << endl;
}