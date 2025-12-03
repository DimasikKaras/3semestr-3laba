#include <stdexcept>

#include "HashTable.h"

#include <utility>
#include <iostream>
#include <tuple>
#include <vector>

using namespace std;


OpenAddressingHashTable::OpenAddressingHashTableNode::OpenAddressingHashTableNode():
        isDelete(false), isNull(true) {}

OpenAddressingHashTable::OpenAddressingHashTable(const int cap): capacity(cap), size(0) {
        if (cap <= 0) throw runtime_error("Вместимость должна быть больше 0");
        table = new OpenAddressingHashTableNode[cap];
        for (size_t i = 0; i < cap; i++) {
                table[i] = OpenAddressingHashTableNode();
        }
}

OpenAddressingHashTable::~OpenAddressingHashTable() {
        delete[] table;
}

int OpenAddressingHashTable::hashFunction(const string &str) const {
    unsigned long hash = base;
    for (const auto& c : str) {
        hash = (hash * prime) ^ static_cast<unsigned char>(c);
    }
    return hash % capacity;
}

bool OpenAddressingHashTable::rehash() {
        vector<pair<string, string>> element;

        for (int i = 0; i < capacity; i++) {
                if (!table[i].isNull && !table[i].isDelete) {
                        element.emplace_back(table[i].key, table[i].data);
                }
        }
        
        const OpenAddressingHashTableNode* oldTable = table;
        const int oldCapacity = capacity;
        
        int newCap = 0;
        for (const int prime1 : primes) {
                if (prime1 > capacity) {
                        newCap = prime1;
                        break;
                }
        }

        if (newCap == 0) {
                newCap = oldCapacity * 2;
        }
        
        table = new OpenAddressingHashTableNode[newCap];
        capacity = newCap;
        size = 0;
        
        for (int i = 0; i < newCap; i++) {
                table[i].isNull = true;
                table[i].isDelete = false;
        }
        
        bool success = true;
        for (auto& el : element) {
                if (!insert(el.first, el.second)) {
                        success = false;
                        break;
                }
        }
        
        delete[] oldTable;
        return success;
}


bool OpenAddressingHashTable::insert(const string &k, const string &value) {
        if (k.empty() || value.empty()) throw runtime_error("ключ и значение не могут быть пустыми");

        int h = hashFunction(k);
        for (size_t i = 0; i < capacity; i++) {
                const size_t index = (h + i) % capacity;

                if (!table[index].isDelete && !table[index].isNull && table[index].key == k) {
                        table[index].data = value;
                        return true;
                }
        }

        if (static_cast<double>(size) / capacity >= 0.75) {
                if (!rehash()) return false;
                h = hashFunction(k);
        }

        for (size_t i = 0; i < capacity; i++) {
                const size_t index = (h + i) % capacity;
                if (table[index].isDelete || table[index].isNull) {
                        table[index].key = k;
                        table[index].data = value;
                        table[index].isNull = table[index].isDelete = false;
                        size++;
                        return true;
                }
        }
        return false;
}


bool OpenAddressingHashTable::deleteKey(const string& k) {
        if (k.empty()) throw runtime_error("ключ не может быть пустым");
        const int h = hashFunction(k);
        for (size_t i = 0; i < capacity; i++) {
                const int index = (h + i) % capacity;
                if (!table[index].isDelete && !table[index].isNull && table[index].key == k) {
                        table[index].isDelete = true;
                        size--;
                        return true;
                }
        }
        return false;
}

tuple<bool, string, string> OpenAddressingHashTable::search(const string &k) const {
        if (k.empty()) throw runtime_error("ключ не может быть пустым");
        const int h = hashFunction(k);
        for (size_t i = 0; i < capacity ;i++) {
                const int index = (h + i) % capacity;
                if (table[index].isDelete || table[index].isNull) continue;
                if (!table[index].isDelete && !table[index].isNull && table[index].key == k) {
                        return make_tuple(true, table[index].key, table[index].data);
                }
        }
        return make_tuple(false, k, "");
}

void OpenAddressingHashTable::print() const {
        cout << "Хэш функция с методом открытой адресации" << endl;
        cout << "Размер: " << size << "/" << capacity << endl;
        for (int i = 0; i < capacity; i++) {
                if (table[i].isDelete) {
                        cout << "[" << i << "] [DELETE] " << table[i].key << ">     ->    " << "<" << table[i].data <<
                                        ">" << endl;
                } else if (table[i].isNull) {
                        cout << "[" << i << "] [NULL]" << endl;
                } else {
                        cout << "[" << i << "] <" << table[i].key << ">     ->    " << "<" << table[i].data << ">" <<
                                        endl;
                }
        }
}


vector<pair<string, string>> OpenAddressingHashTable::getAllElements() const {
        vector<pair<string, string>> elements;
        for (int i = 0; i < capacity; i++) {
                if (!table[i].isNull && !table[i].isDelete) {
                        elements.emplace_back(table[i].key, table[i].data);
                }
        }
        return elements;
}







