#include "queue.h"


#include <stdexcept>
#include <iostream>

using namespace std;

QueueNode::QueueNode(const string& value) : data(value), next(nullptr) {}

string QueueNode::getData() const {
    return data;
}

QueueNode* QueueNode::getNext() const{
    return next;
}

void QueueNode::setNext(QueueNode *node) {
    next = node;
}

Queue::Queue(): first(nullptr), last(nullptr) {}

Queue::~Queue() {
    while (first != nullptr) {
        const QueueNode* temp = first;
        first = first -> getNext();
        delete temp;
    }
}

void Queue::enqueue(const string &value) {
    auto* newNode = new QueueNode(value);
    if (last == nullptr) {
        first = newNode;
        last = newNode;
    } else {
        last->setNext(newNode);
        last = newNode;
    }
}

string Queue::dequeue() {
    if (!first) throw runtime_error("Очередь пустая");
    const QueueNode* current = first;
    first = current->getNext();
    string value = current->getData();

    if (first == nullptr) {
        last = nullptr;
    }

    delete current;
    return value;
}

void Queue::print() const {
    if (!first) {
        cout << "(Пустая очередь)" << endl;
        return;
    }
    const QueueNode* current = first;
    while (current != nullptr) {
        cout << current->getData() << " ";
        current = current ->getNext();
    }
    cout << endl;
}
