#ifndef LABA3_SIMPLYLIST_H
#define LABA3_SIMPLYLIST_H

#include <string>

class SimplyListNode {
    std::string data;
    SimplyListNode* next;
public:
    explicit SimplyListNode(const std::string& value);

    std::string getData() const;
    SimplyListNode* getNext() const;
    void setNext(SimplyListNode* node);
};

class SimplyList {
    SimplyListNode* head;
    SimplyListNode* tail;
public:
    SimplyList();
    ~SimplyList();

    SimplyListNode *getHead() const;

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

};

#endif //LABA3_SIMPLYLIST_H