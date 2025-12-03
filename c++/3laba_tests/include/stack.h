#ifndef LABA3_STACK_H
#define LABA3_STACK_H

#include <string>

class StackNode {
    std::string data;
    StackNode* next;
public:
    explicit StackNode(const std::string &value);

    std::string getData() const;
    StackNode* getNext() const;
    void setNext(StackNode* node);
};

class Stack {
    StackNode* top;
public:
    Stack();
    ~Stack();

    StackNode* getTop() const { return top; }

    void push(const std::string& value);
    std::string pop();
    void print() const;
};

#endif //LABA3_STACK_H