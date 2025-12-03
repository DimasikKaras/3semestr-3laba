#include "stack.h"

#include <stdexcept>
#include <iostream>

using namespace std;

StackNode::StackNode(const string &value) : data(value), next(nullptr) {}

string StackNode::getData() const {
    return data;
}

StackNode* StackNode::getNext() const {
    return next;
}

void StackNode::setNext(StackNode* node) {
    next = node;
}

Stack::Stack() : top(nullptr){}
Stack::~Stack() {
    while (top != nullptr) {
        const StackNode* current = top;
        top = top->getNext();
        delete current;
    }
}

void Stack::push(const string& value) {
    auto* newNode = new StackNode(value);
    newNode -> setNext(top);
    top = newNode;
}

std::string Stack::pop() {
    if (top == nullptr) {
        throw runtime_error("Стек пустой");
    }

    const StackNode* temp = top;
    string value = top->getData();
    top = top ->getNext();
    delete temp;
    return value;
}

void Stack::print() const {
    if (!top) {
        cout << "Пустой стек" << endl;
        return;
    }

    const StackNode* current = top;
    while (current != nullptr) {
        cout << current -> getData() << " ";
        current = current -> getNext();
    }
    cout << endl;
}




