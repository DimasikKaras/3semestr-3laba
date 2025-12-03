#ifndef LABA3_DYARR_H
#define LABA3_DYARR_H

#include <string>

class dyArr {
    std::string* data;
    int capacity;
    int length;

public:
    explicit dyArr(int cap);
    ~dyArr();

    void pushBack(const std::string& value);
    void pushInIndex(const std::string& value, int index);
    std::string getElement(int index) const;
    void removeAtIndex(int index);
    void replaceAtIndex(const std::string& value, int index) const;
    int getLength() const;
    void printDyArr() const;

};

#endif //LABA3_DYARR_H