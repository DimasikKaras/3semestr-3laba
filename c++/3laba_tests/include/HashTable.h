#ifndef LABA3_OPENADRESSINGHASHTABLE_H
#define LABA3_OPENADRESSINGHASHTABLE_H

#include <string>
#include <vector>


constexpr unsigned long base = 2166136261;
constexpr unsigned long prime = 16777619;
const int primes[] = {5, 7, 11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421, 12853};

class OpenAddressingHashTable {
    struct OpenAddressingHashTableNode {
        std::string key;
        std::string data;
        bool isDelete;
        bool isNull;
        OpenAddressingHashTableNode(std::string& k, std::string& value);
        OpenAddressingHashTableNode();
    };

    OpenAddressingHashTableNode* table;
    size_t capacity;
    size_t size;
public:
    explicit OpenAddressingHashTable(int cap);
    ~OpenAddressingHashTable();

    [[nodiscard]] int hashFunction(const std::string& str) const;
    bool insert(const std::string& k, const std::string& value);
    bool rehash();
    bool deleteKey(const std::string& k);
    std::tuple<bool, std::string, std::string> search(const std::string& k) const;
    void print() const;

    std::vector<std::pair<std::string, std::string>> getAllElements() const;
};

class ChainHashTable {
    class ChainHashTableNode {
    public:
        std::string key;
        std::string data;
        ChainHashTableNode* next;

        ChainHashTableNode(const std::string& k, const std::string& value);
        ChainHashTableNode();

        void addHead(const std::string& k, const std::string& value);
        bool deleteKey(const std::string& k);
        ChainHashTableNode* searchKey(const std::string& k) const;
        void printList() const;
    };

    ChainHashTableNode** table;
    size_t capacity;
    size_t size;

public:
    explicit ChainHashTable(int cap);
    ~ChainHashTable();

    [[nodiscard]] int hashFunction(const std::string& str) const;
    bool insert(const std::string& k, const std::string& value);
    bool rehash();
    bool deleteKey(const std::string& k);
    std::tuple<bool, std::string, std::string> search(const std::string& k) const;
    void print() const;

    std::vector<std::pair<std::string, std::string>> getAllElements() const;
};

#endif //LABA3_OPENADRESSINGHASHTABLE_H