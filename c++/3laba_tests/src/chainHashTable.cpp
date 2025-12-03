#include "HashTable.h"
#include <iostream>
#include <tuple>
#include <vector>

using namespace std;
ChainHashTable::ChainHashTableNode::ChainHashTableNode()
    : next(nullptr) {}

ChainHashTable::ChainHashTableNode::ChainHashTableNode(const std::string& k, const std::string& value)
    : key(k), data(value), next(nullptr) {}

void ChainHashTable::ChainHashTableNode::addHead(const std::string& k, const std::string& value) {
    auto* newNode = new ChainHashTableNode(k, value);
    newNode->next = this->next;
    this->next = newNode;
}

bool ChainHashTable::ChainHashTableNode::deleteKey(const std::string& k) {
    ChainHashTableNode* current = this->next;
    auto prev = this;

    while (current != nullptr) {
        if (current->key == k) {
            prev->next = current->next;
            delete current;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

ChainHashTable::ChainHashTableNode* ChainHashTable::ChainHashTableNode::searchKey(const std::string& k) const {
    ChainHashTableNode* current = this->next;

    while (current != nullptr) {
        if (current->key == k) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

void ChainHashTable::ChainHashTableNode::printList() const {
    const ChainHashTableNode* current = this->next;
    while (current != nullptr) {
        std::cout << "(" << current->key << ", " << current->data << ") -> ";
        current = current->next;
    }
    std::cout << "nullptr";
}

ChainHashTable::ChainHashTable(const int cap) : capacity(cap), size(0) {
    table = new ChainHashTableNode*[capacity];
    for (size_t i = 0; i < capacity; i++) {
        table[i] = new ChainHashTableNode();
    }
}

ChainHashTable::~ChainHashTable() {
    for (size_t i = 0; i < capacity; i++) {
        const ChainHashTableNode* current = table[i];
        while (current != nullptr) {
            const ChainHashTableNode* next = current->next;
            delete current;
            current = next;
        }
    }
    delete[] table;
}

int ChainHashTable::hashFunction(const std::string &str) const {
    unsigned long hash = base;
    for (const auto& c : str) {
        hash = (hash * prime) ^ static_cast<unsigned char>(c);
    }
    return hash % capacity;
}

bool ChainHashTable::insert(const std::string& k, const std::string& value) {
    if (static_cast<double>(size) / capacity >= 0.75) {
        if (!rehash()) return false;
    }

    const int index = hashFunction(k);

    ChainHashTableNode* existing = table[index]->searchKey(k);
    if (existing != nullptr) {
        existing->data = value;
        return true;
    }

    table[index]->addHead(k, value);
    size++;
    return true;
}

std::tuple<bool, std::string, std::string> ChainHashTable::search(const std::string& k) const {
    const int index = hashFunction(k);
    ChainHashTableNode* node = table[index]->searchKey(k);

    if (node != nullptr) {
        return std::make_tuple(true, node->key, node->data);
    }

    return std::make_tuple(false, "", "");
}

bool ChainHashTable::deleteKey(const std::string& k) {
    const int index = hashFunction(k);
    if (table[index]->deleteKey(k)) {
        size--;
        return true;
    }
    return false;
}


bool ChainHashTable::rehash() {
    vector<pair<string, string>> elements;

    for (size_t i = 0; i < capacity; i++) {
        ChainHashTableNode* current = table[i]->next;
        while (current != nullptr) {
            elements.emplace_back(current->key, current->data);
            current = current->next;
        }
    }

    ChainHashTableNode** oldTable = table;
    const size_t oldCapacity = capacity;

    size_t newCap = 0;
    for (const int prime1 : primes) {
        if (prime1 > static_cast<int>(capacity)) {
            newCap = prime1;
            break;
        }
    }

    if (newCap == 0) {
        newCap = oldCapacity * 2;
    }

    table = new ChainHashTableNode*[newCap];
    capacity = newCap;
    size = 0;

    for (size_t i = 0; i < newCap; i++) {
        table[i] = new ChainHashTableNode();
    }

    bool success = true;
    for (auto& el : elements) {
        if (!insert(el.first, el.second)) {
            success = false;
            break;
        }
    }

    if (oldTable != nullptr) {
        for (size_t i = 0; i < oldCapacity; i++) {
            const ChainHashTableNode* current = oldTable[i];
            while (current != nullptr) {
                const ChainHashTableNode* next = current->next;
                delete current;
                current = next;
            }
        }
        delete[] oldTable;
    }

    return success;
}

void ChainHashTable::print() const {
    if (table == nullptr) {
        cout << "[NULL]" << endl;
        return;
    };
    cout << "Хэш функция с методом цепочек" << endl;
    cout << "Размер: " << size << "/" << capacity << endl;
    for (int i = 0; i < capacity; i++) {
        if (table[i] == nullptr) {
            cout << "[" << i << "] [NULL]" << endl;
        } else {
            cout << "[" << i << "] ";
            table[i]->printList();
            cout << endl;
        }
    }
}

std::vector<std::pair<std::string, std::string>> ChainHashTable::getAllElements() const {
    std::vector<std::pair<std::string, std::string>> elements;
    for (size_t i = 0; i < capacity; i++) {
        ChainHashTableNode* current = table[i]->next;
        while (current != nullptr) {
            elements.emplace_back(current->key, current->data);
            current = current->next;
        }
    }
    return elements;
}
