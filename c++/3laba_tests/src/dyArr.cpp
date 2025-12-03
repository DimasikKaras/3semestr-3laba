#include "dyArr.h"

#include <iostream>
#include <stdexcept>

using namespace std;

dyArr::dyArr(const int cap) : capacity(cap), length(0) {data = new string[capacity];}

dyArr::~dyArr() {delete[] data;}

void dyArr::pushBack(const std::string &value) {
    if (length >= capacity) {
        const int newCapacity = capacity * 2;
        auto* newData = new string[newCapacity];
        for (int i = 0; i < length; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }
    data[length] = value;
    length++;
}

void dyArr::pushInIndex(const std::string &value, const int index) {
    if (index < 0 || index > length) throw runtime_error("ошибка индекса");
    if (length >= capacity) {
        const int newCapacity = capacity * 2;
        auto* newData = new string[newCapacity];
        for (int i = 0; i < length; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

    for (int i = length; i > index; i--) {
        data[i] = data[i-1];
    }

    data[index] = value;
    length++;
}

void dyArr::removeAtIndex(const int index) {
    if (index < 0 || index > length) throw runtime_error("ошибка индекса");
    for (int i = index; i < length; i++) {
        data[i] = data[i+1];
    }
    length--;
}

void dyArr::replaceAtIndex(const std::string &value, const int index) const {
    if (index < 0 || index > length) throw runtime_error("ошибка индекса");
    data[index] = value;
}

void dyArr::printDyArr() const {
    if (length == 0) {
        cout << "Пустой массив" << endl;
        return;
    }
    cout << "[";
    for (int i = 0; i < length; i++) {
        cout << data[i];
        if (i < length - 1) {
            cout << ", ";
        }
    }
    cout << "]" << endl;
}

std::string dyArr::getElement(const int index) const {
    if (index < 0 || index > length) throw runtime_error("ошибка индекса");
    return data[index];
}

int dyArr::getLength() const { return length;}





