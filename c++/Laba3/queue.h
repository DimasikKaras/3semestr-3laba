#ifndef LABA3_QUEUE_H
#define LABA3_QUEUE_H

#include <string>

class QueueNode {
    std::string data;
    QueueNode* next;
public:
    explicit QueueNode(const std::string& value);

    std::string getData() const;
    QueueNode* getNext() const;
    void setNext(QueueNode* node);
};

class Queue {
    QueueNode* first;
    QueueNode* last;
public:
    Queue();
    ~Queue();

    QueueNode* getFirst() const { return first; }

    void enqueue(const std::string& value);
    std::string dequeue();
    void print() const;
};

#endif //LABA3_QUEUE_H