#ifndef LABA3_DOUBLYLIST_H
#define LABA3_DOUBLYLIST_H

#include <string>

class DoublyListNode {
    std::string data;
    DoublyListNode* next;
    DoublyListNode* prev;
public:
    explicit DoublyListNode(const std::string& value);

    std::string getData() const;
    DoublyListNode* getNext() const;
    DoublyListNode* getPrev() const;
    void setNext(DoublyListNode* node);
    void setPrev(DoublyListNode* node);
};

class DoublyList {
    DoublyListNode* head;
    DoublyListNode* tail;
public:
    DoublyList();
    ~DoublyList();

    DoublyListNode* getHead() const { return head; }

    void addHead(const std::string& value);
    void addBack(const std::string& value);
    void addAfterIndex(int index, const std::string& value);
    void addBeforeIndex(int index, const std::string& value);

    void dellHead();
    void dellBack();
    void dellAfterIndex(int index);
    void dellBeforeIndex(int index);
    void dellValue(const std::string& value);

    bool searchValue(const std::string& value) const;
    void print() const;
    void printReverse() const;
};

#endif // LABA3_DOUBLYLIST_H