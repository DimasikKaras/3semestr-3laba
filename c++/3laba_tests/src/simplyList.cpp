#include "simplyList.h"
#include <iostream>
using namespace std;

SimplyListNode::SimplyListNode(const string &value): data(value), next(nullptr){}

string SimplyListNode::getData() const {
    return data;
}

SimplyListNode *SimplyListNode::getNext() const {
    return next;
}

void SimplyListNode::setNext(SimplyListNode *node) {
    next = node;
}

SimplyList::SimplyList(): head(nullptr), tail(nullptr){}
SimplyList::~SimplyList() {
    while (head != nullptr) {
        const SimplyListNode* temp = head;
        head = head->getNext();
        delete temp;
    }
}

void SimplyList::addHead(const string &value) {
    if (value.empty()) {
        throw runtime_error("Значение пустое");
    }
    const auto newNode = new SimplyListNode(value);
    newNode->setNext(head);
    head = newNode;
    if (!tail) tail = newNode;
}

void SimplyList::addBack(const string &value) {
    if (value.empty()) {
        throw runtime_error("Значение пустое");
    }

    const auto newNode = new SimplyListNode(value);
    if (!tail) head = tail = newNode;
    else {
        tail->setNext(newNode);
        tail = newNode;
    }
}

void SimplyList::addAfterIndex(int index, const string &value) {
    if (index < 0) throw runtime_error("Индекс меньше 0");
    if (value.empty()) throw runtime_error("Значение пустое");

    if (head == nullptr) {
        if (index == 0) {
            head = new SimplyListNode(value);
            tail = head;
            return;
        } else {
            throw runtime_error("Список пуст, индекс недоступен");
        }
    }

    auto current = head;
    for (int i = 0; i < index && current != nullptr; i++) {
        current = current->getNext();
    }

    if (current == nullptr) {
        throw runtime_error("Индекс превышает размер списка");
    }

    const auto newNode = new SimplyListNode(value);
    newNode->setNext(current->getNext());
    current->setNext(newNode);

    if (newNode->getNext() == nullptr) {
        tail = newNode;
    }
}

void SimplyList::addBeforeIndex(int index, const string &value) {
    if (index < 0) throw runtime_error("Индекс меньше 0");
    if (value.empty()) throw runtime_error("Значение пустое");

    if (head == nullptr) {
        if (index == 0) {
            addHead(value);
            return;
        } else {
            throw runtime_error("Список пуст, индекс недоступен");
        }
    }

    if (index == 0) {
        addHead(value);
        return;
    }

    auto current = head;
    for (int i = 0; i < index - 1 && current != nullptr; i++) {
        current = current->getNext();
    }

    if (current == nullptr) {
        throw runtime_error("Индекс превышает размер списка");
    }

    const auto newNode = new SimplyListNode(value);
    newNode->setNext(current->getNext());
    current->setNext(newNode);

    if (newNode->getNext() == nullptr) {
        tail = newNode;
    }
}

void SimplyList::dellHead() {
    if (head == nullptr) throw runtime_error("Список пустой");

    const auto temp = head;
    head = head->getNext();

    if (temp == tail) {
        tail = nullptr;
    }

    delete temp;
}

void SimplyList::dellBack() {
    if (head == nullptr) throw runtime_error("Список пустой");

    if (head == tail) {
        delete head;
        head = tail = nullptr;
        return;
    }

    auto current = head;
    while (current ->getNext() != tail) current = current ->getNext();

    delete tail;
    tail = current;
    tail -> setNext(nullptr);
}

void SimplyList::dellAfterIndex(int index) {
    if (head == nullptr) throw runtime_error("Список пустой");
    if (index < 0) throw runtime_error("Индекс меньше 0");

    auto current = head;
    for (int i = 0; i < index && current != nullptr; i++) {
        current = current ->getNext();
    }

    if (current == nullptr || current->getNext() == nullptr) throw runtime_error("Индекс превышает размер списка");

    const auto temp = current->getNext();
    current->setNext(temp->getNext());

    if (temp == tail) {
        tail = current;
    }
    delete temp;
}

void SimplyList::dellBeforeIndex(int index) {
    if (head == nullptr) throw runtime_error("Список пустой");
    if (index < 0) throw runtime_error("Индекс меньше 0");
    if (index == 0) throw runtime_error("Нельзя удалить элемент перед индексом 0");

    if (index == 1) {
        dellHead();
        return;
    }


    auto current = head;
    for (int i = 0; i < index - 2 && current != nullptr; i++) {
        current = current ->getNext();
    }

    if (current == nullptr || current->getNext() == nullptr) throw runtime_error("Индекс превышает размер списка");

    const auto temp = current->getNext();
    current->setNext(temp->getNext());

    if (temp == tail) {
        tail = current;
    }
    delete temp;
}

void SimplyList::dellValue(const string &value) {
    if (head == nullptr) throw runtime_error("Список пустой");
    if (value.empty()) throw runtime_error("Значение пустое");

    if (head->getData() == value) {
        const auto temp = head;
        head = head->getNext();
        if (head == nullptr) tail = nullptr;
        delete temp;
        return;
    }

    auto* current = head;
    while (current->getNext() != nullptr && current->getNext()->getData() != value) {
        current = current->getNext();
    }

    if (current ->getNext() == nullptr) throw runtime_error(value + " отсутствует в списке");

    const auto temp = current->getNext();
    current->setNext(temp->getNext());

    if (temp == tail) {
        tail = current;
    }

    delete temp;
}

bool SimplyList::searchValue(const string &value) const {
    if (head == nullptr) throw runtime_error("Список пустой");
    if (value.empty()) throw runtime_error("Значение пустое");

    auto current = head;
    while (current != nullptr) {
        if (current->getData() == value) return true;
        current = current->getNext();
    }
    return false;
}

void SimplyList::print() const {
    if (head == nullptr) {
        cout << "(Список пустой)" << endl;
        return;
    }
    auto current = head;
    while (current != nullptr) {
        cout << current->getData();
        if (current->getNext() != nullptr) {
            cout << " -> ";
        }
        current = current->getNext();
    }
    cout << endl;
}

SimplyListNode *SimplyList::getHead() const {
    return head;
}
