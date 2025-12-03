#include "executingCommands.h"
#include "dyArr.h"
#include "simplyList.h"
#include "doublyList.h"
#include "stack.h"
#include "queue.h"
#include "HashTable.h"
#include "avlTree.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cstdint>
#include <cstring>

using namespace std;

map<string, dyArr*> arrays;
map<string, SimplyList*> simplyLists;
map<string, DoublyList*> doublyLists;
map<string, Stack*> stacks;
map<string, Queue*> queues;
map<string, OpenAddressingHashTable*> openTables;
map<string, ChainHashTable*> chainTables;
map<string, AVLTree*> trees;

vector<string> commands;
string currentFilename;

bool useBinaryFormat = false;

string toUpperCase(const string& str) {
    string result;
    for (const unsigned char c : str) {
        result += toupper(c);
    }
    return result;
}

void saveToFile() {
    if (useBinaryFormat) {
        saveToFileBinary();
    } else {
        saveToFileText();
    }
}

void loadFromFile(const string& filename) {
    if (useBinaryFormat) {
        loadFromFileBinary(filename);
    } else {
        loadFromFileText(filename);
    }
}

void saveToFileText() {
    if (currentFilename.empty()) {
        cout << "Файл не указан, сохранение невозможно" << endl;
        return;
    }

    ofstream file(currentFilename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла для записи: " << currentFilename << endl;
        return;
    }

    for (const auto& arr : arrays) {
        file << "ARRAY " << arr.first;
        for (int i = 0; i < arr.second->getLength(); i++) {
            file << " " << arr.second->getElement(i);
        }
        file << endl;
    }

    for (const auto& list : simplyLists) {
        file << "SIMPLY " << list.first;
        SimplyListNode* current = list.second->getHead();
        while (current != nullptr) {
            file << " " << current->getData();
            current = current->getNext();
        }
        file << endl;
    }

    for (const auto& list : doublyLists) {
        file << "DOUBLE " << list.first;
        DoublyListNode* current = list.second->getHead();
        while (current != nullptr) {
            file << " " << current->getData();
            current = current->getNext();
        }
        file << endl;
    }

    for (const auto& stack : stacks) {
        file << "STACK " << stack.first;
        std::vector<std::string> stackElements;
        StackNode* current = stack.second->getTop();
        while (current != nullptr) {
            stackElements.push_back(current->getData());
            current = current->getNext();
        }
        for (auto it = stackElements.rbegin(); it != stackElements.rend(); ++it) {
            file << " " << *it;
        }
        file << endl;
    }

    for (const auto& queue : queues) {
        file << "QUEUE " << queue.first;
        QueueNode* current = queue.second->getFirst();
        while (current != nullptr) {
            file << " " << current->getData();
            current = current->getNext();
        }
        file << endl;
    }

    for (const auto& table : openTables) {
        file << "TABLEOPEN " << table.first;
        auto elements = table.second->getAllElements();
        for (const auto& element : elements) {
            file << " " << element.first << " " << element.second;
        }
        file << endl;
    }

    for (const auto& table : chainTables) {
        file << "TABLECHAIN " << table.first;
        auto elements = table.second->getAllElements();
        for (const auto& element : elements) {
            file << " " << element.first << " " << element.second;
        }
        file << endl;
    }

    for (const auto& tree : trees) {
        file << "TREE " << tree.first;
        auto elements = tree.second->getElements();
        for (int value : elements) {
            file << " " << value;
        }
        file << endl;
    }

    file.close();
    cout << "Все данные сохранены в файл: " << currentFilename << endl;
}

void loadFromFileText(const string& filename) {
    if (filename.empty()) return;

    ifstream file(filename);
    if (!file.is_open()) {
        return;
    }

    string line;
    while (getline(file, line)) {
        vector<string> param;
        stringstream ss(line);
        string tok;

        while (ss >> tok) {
            param.push_back(tok);
        }

        if (param.size() < 2) continue;

        string type = param[0];
        string name = param[1];

        if (type == "ARRAY") {
            if (arrays.find(name) != arrays.end()) {
                delete arrays[name];
            }
            arrays[name] = new dyArr(10);
            for (size_t i = 2; i < param.size(); i++) {
                arrays[name]->pushBack(param[i]);
            }
        }
        else if (type == "SIMPLY") {
            if (simplyLists.find(name) != simplyLists.end()) {
                delete simplyLists[name];
            }
            simplyLists[name] = new SimplyList();
            for (size_t i = 2; i < param.size(); i++) {
                simplyLists[name]->addBack(param[i]);
            }
        }
        else if (type == "DOUBLE") {
            if (doublyLists.find(name) != doublyLists.end()) {
                delete doublyLists[name];
            }
            doublyLists[name] = new DoublyList();
            for (size_t i = 2; i < param.size(); i++) {
                doublyLists[name]->addBack(param[i]);
            }
        }
        else if (type == "STACK") {
            if (stacks.find(name) != stacks.end()) {
                delete stacks[name];
            }
            stacks[name] = new Stack();
            for (size_t i = 2; i < param.size(); i++) {
                stacks[name]->push(param[i]);
            }
        }
        else if (type == "QUEUE") {
            if (queues.find(name) != queues.end()) {
                delete queues[name];
            }
            queues[name] = new Queue();
            for (size_t i = 2; i < param.size(); i++) {
                queues[name]->enqueue(param[i]);
            }
        }
        else if (type == "TABLEOPEN") {
            if (openTables.find(name) != openTables.end()) {
                delete openTables[name];
            }
            openTables[name] = new OpenAddressingHashTable(10);
            for (size_t i = 2; i < param.size(); i += 2) {
                if (i + 1 < param.size()) {
                    openTables[name]->insert(param[i], param[i + 1]);
                }
            }
        }
        else if (type == "TABLECHAIN") {
            if (chainTables.find(name) != chainTables.end()) {
                delete chainTables[name];
            }
            chainTables[name] = new ChainHashTable(10);
            for (size_t i = 2; i < param.size(); i += 2) {
                if (i + 1 < param.size()) {
                    chainTables[name]->insert(param[i], param[i + 1]);
                }
            }
        }
        else if (type == "TREE") {
            if (trees.find(name) != trees.end()) {
                delete trees[name];
            }
            trees[name] = new AVLTree();
            for (size_t i = 2; i < param.size(); i++) {
                try {
                    int value = stoi(param[i]);
                    trees[name]->insert(value);
                } catch (...) {
                    cout << "Ошибка преобразования числа для дерева " << name << ": " << param[i] << endl;
                }
            }
        }
    }
    file.close();
}


void saveToFileBinary() {
    if (currentFilename.empty()) {
        cout << "Файл не указан, сохранение невозможно" << endl;
        return;
    }

    ofstream file(currentFilename, ios::binary);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла для записи: " << currentFilename << endl;
        return;
    }

    constexpr char signature[] = "DS_SYSTEM_v1";
    file.write(signature, sizeof(signature));

    uint32_t arrayCount = arrays.size();
    file.write(reinterpret_cast<const char*>(&arrayCount), sizeof(arrayCount));

    uint32_t simplyListCount = simplyLists.size();
    file.write(reinterpret_cast<const char*>(&simplyListCount), sizeof(simplyListCount));

    uint32_t doublyListCount = doublyLists.size();
    file.write(reinterpret_cast<const char*>(&doublyListCount), sizeof(doublyListCount));

    uint32_t stackCount = stacks.size();
    file.write(reinterpret_cast<const char*>(&stackCount), sizeof(stackCount));

    uint32_t queueCount = queues.size();
    file.write(reinterpret_cast<const char*>(&queueCount), sizeof(queueCount));

    uint32_t openTableCount = openTables.size();
    file.write(reinterpret_cast<const char*>(&openTableCount), sizeof(openTableCount));

    uint32_t chainTableCount = chainTables.size();
    file.write(reinterpret_cast<const char*>(&chainTableCount), sizeof(chainTableCount));

    uint32_t treeCount = trees.size();
    file.write(reinterpret_cast<const char*>(&treeCount), sizeof(treeCount));

    for (const auto& arr : arrays) {
        uint32_t nameLength = arr.first.length();
        file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        file.write(arr.first.c_str(), nameLength);

        uint32_t arrLength = arr.second->getLength();
        file.write(reinterpret_cast<const char*>(&arrLength), sizeof(arrLength));

        for (int i = 0; i < arrLength; i++) {
            string element = arr.second->getElement(i);
            uint32_t elemLength = element.length();
            file.write(reinterpret_cast<const char*>(&elemLength), sizeof(elemLength));
            file.write(element.c_str(), elemLength);
        }
    }

    for (const auto& list : simplyLists) {
        uint32_t nameLength = list.first.length();
        file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        file.write(list.first.c_str(), nameLength);

        vector<string> elements;
        SimplyListNode* current = list.second->getHead();
        while (current != nullptr) {
            elements.push_back(current->getData());
            current = current->getNext();
        }

        uint32_t listLength = elements.size();
        file.write(reinterpret_cast<const char*>(&listLength), sizeof(listLength));

        for (const auto& element : elements) {
            uint32_t elemLength = element.length();
            file.write(reinterpret_cast<const char*>(&elemLength), sizeof(elemLength));
            file.write(element.c_str(), elemLength);
        }
    }

    for (const auto& list : doublyLists) {
        uint32_t nameLength = list.first.length();
        file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        file.write(list.first.c_str(), nameLength);

        vector<string> elements;
        DoublyListNode* current = list.second->getHead();
        while (current != nullptr) {
            elements.push_back(current->getData());
            current = current->getNext();
        }

        uint32_t listLength = elements.size();
        file.write(reinterpret_cast<const char*>(&listLength), sizeof(listLength));

        for (const auto& element : elements) {
            uint32_t elemLength = element.length();
            file.write(reinterpret_cast<const char*>(&elemLength), sizeof(elemLength));
            file.write(element.c_str(), elemLength);
        }
    }

    for (const auto& stack : stacks) {
        uint32_t nameLength = stack.first.length();
        file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        file.write(stack.first.c_str(), nameLength);

        vector<string> elements;
        StackNode* current = stack.second->getTop();
        while (current != nullptr) {
            elements.insert(elements.begin(), current->getData());
            current = current->getNext();
        }

        uint32_t stackLength = elements.size();
        file.write(reinterpret_cast<const char*>(&stackLength), sizeof(stackLength));

        for (const auto& element : elements) {
            uint32_t elemLength = element.length();
            file.write(reinterpret_cast<const char*>(&elemLength), sizeof(elemLength));
            file.write(element.c_str(), elemLength);
        }
    }

    for (const auto& queue : queues) {
        uint32_t nameLength = queue.first.length();
        file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        file.write(queue.first.c_str(), nameLength);

        vector<string> elements;
        QueueNode* current = queue.second->getFirst();
        while (current != nullptr) {
            elements.push_back(current->getData());
            current = current->getNext();
        }

        uint32_t queueLength = elements.size();
        file.write(reinterpret_cast<const char*>(&queueLength), sizeof(queueLength));

        for (const auto& element : elements) {
            uint32_t elemLength = element.length();
            file.write(reinterpret_cast<const char*>(&elemLength), sizeof(elemLength));
            file.write(element.c_str(), elemLength);
        }
    }

    for (const auto& table : openTables) {
        uint32_t nameLength = table.first.length();
        file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        file.write(table.first.c_str(), nameLength);

        auto elements = table.second->getAllElements();
        uint32_t tableSize = elements.size();
        file.write(reinterpret_cast<const char*>(&tableSize), sizeof(tableSize));

        for (const auto& element : elements) {
            uint32_t keyLength = element.first.length();
            file.write(reinterpret_cast<const char*>(&keyLength), sizeof(keyLength));
            file.write(element.first.c_str(), keyLength);

            uint32_t valueLength = element.second.length();
            file.write(reinterpret_cast<const char*>(&valueLength), sizeof(valueLength));
            file.write(element.second.c_str(), valueLength);
        }
    }

    for (const auto& table : chainTables) {
        uint32_t nameLength = table.first.length();
        file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        file.write(table.first.c_str(), nameLength);

        auto elements = table.second->getAllElements();
        uint32_t tableSize = elements.size();
        file.write(reinterpret_cast<const char*>(&tableSize), sizeof(tableSize));

        for (const auto& element : elements) {
            uint32_t keyLength = element.first.length();
            file.write(reinterpret_cast<const char*>(&keyLength), sizeof(keyLength));
            file.write(element.first.c_str(), keyLength);

            uint32_t valueLength = element.second.length();
            file.write(reinterpret_cast<const char*>(&valueLength), sizeof(valueLength));
            file.write(element.second.c_str(), valueLength);
        }
    }

    for (const auto& tree : trees) {
        uint32_t nameLength = tree.first.length();
        file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        file.write(tree.first.c_str(), nameLength);

        auto elements = tree.second->getElements();
        uint32_t treeSize = elements.size();
        file.write(reinterpret_cast<const char*>(&treeSize), sizeof(treeSize));

        for (int value : elements) {
            file.write(reinterpret_cast<const char*>(&value), sizeof(value));
        }
    }

    file.close();
    cout << "Все данные сохранены в бинарный файл: " << currentFilename << endl;
}

void loadFromFileBinary(const string& filename) {
    if (filename.empty()) return;

    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        return;
    }

    char signature[13];
    file.read(signature, sizeof(signature));
    if (strcmp(signature, "DS_SYSTEM_v1") != 0) {
        cout << "Неверный формат бинарного файла" << endl;
        return;
    }

    uint32_t arrayCount, simplyListCount, doublyListCount, stackCount, queueCount;
    uint32_t openTableCount, chainTableCount, treeCount;

    file.read(reinterpret_cast<char*>(&arrayCount), sizeof(arrayCount));
    file.read(reinterpret_cast<char*>(&simplyListCount), sizeof(simplyListCount));
    file.read(reinterpret_cast<char*>(&doublyListCount), sizeof(doublyListCount));
    file.read(reinterpret_cast<char*>(&stackCount), sizeof(stackCount));
    file.read(reinterpret_cast<char*>(&queueCount), sizeof(queueCount));
    file.read(reinterpret_cast<char*>(&openTableCount), sizeof(openTableCount));
    file.read(reinterpret_cast<char*>(&chainTableCount), sizeof(chainTableCount));
    file.read(reinterpret_cast<char*>(&treeCount), sizeof(treeCount));

    for (uint32_t i = 0; i < arrayCount; i++) {
        uint32_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        string name;
        name.resize(nameLength);
        file.read(&name[0], nameLength);

        uint32_t arrLength;
        file.read(reinterpret_cast<char*>(&arrLength), sizeof(arrLength));

        if (arrays.find(name) != arrays.end()) {
            delete arrays[name];
        }
        arrays[name] = new dyArr(arrLength + 10);

        for (uint32_t j = 0; j < arrLength; j++) {
            uint32_t elemLength;
            file.read(reinterpret_cast<char*>(&elemLength), sizeof(elemLength));
            string element;
            element.resize(elemLength);
            file.read(&element[0], elemLength);
            arrays[name]->pushBack(element);
        }
    }

    for (uint32_t i = 0; i < simplyListCount; i++) {
        uint32_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        string name;
        name.resize(nameLength);
        file.read(&name[0], nameLength);

        uint32_t listLength;
        file.read(reinterpret_cast<char*>(&listLength), sizeof(listLength));

        if (simplyLists.find(name) != simplyLists.end()) {
            delete simplyLists[name];
        }
        simplyLists[name] = new SimplyList();

        for (uint32_t j = 0; j < listLength; j++) {
            uint32_t elemLength;
            file.read(reinterpret_cast<char*>(&elemLength), sizeof(elemLength));
            string element;
            element.resize(elemLength);
            file.read(&element[0], elemLength);
            simplyLists[name]->addBack(element);
        }
    }

    for (uint32_t i = 0; i < doublyListCount; i++) {
        uint32_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        string name;
        name.resize(nameLength);
        file.read(&name[0], nameLength);

        uint32_t listLength;
        file.read(reinterpret_cast<char*>(&listLength), sizeof(listLength));

        if (doublyLists.find(name) != doublyLists.end()) {
            delete doublyLists[name];
        }
        doublyLists[name] = new DoublyList();

        for (uint32_t j = 0; j < listLength; j++) {
            uint32_t elemLength;
            file.read(reinterpret_cast<char*>(&elemLength), sizeof(elemLength));
            string element;
            element.resize(elemLength);
            file.read(&element[0], elemLength);
            doublyLists[name]->addBack(element);
        }
    }

    for (uint32_t i = 0; i < stackCount; i++) {
        uint32_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        string name;
        name.resize(nameLength);
        file.read(&name[0], nameLength);

        uint32_t stackLength;
        file.read(reinterpret_cast<char*>(&stackLength), sizeof(stackLength));

        if (stacks.find(name) != stacks.end()) {
            delete stacks[name];
        }
        stacks[name] = new Stack();

        for (uint32_t j = 0; j < stackLength; j++) {
            uint32_t elemLength;
            file.read(reinterpret_cast<char*>(&elemLength), sizeof(elemLength));
            string element;
            element.resize(elemLength);
            file.read(&element[0], elemLength);
            stacks[name]->push(element);
        }
    }

    for (uint32_t i = 0; i < queueCount; i++) {
        uint32_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        string name;
        name.resize(nameLength);
        file.read(&name[0], nameLength);

        uint32_t queueLength;
        file.read(reinterpret_cast<char*>(&queueLength), sizeof(queueLength));

        if (queues.find(name) != queues.end()) {
            delete queues[name];
        }
        queues[name] = new Queue();

        for (uint32_t j = 0; j < queueLength; j++) {
            uint32_t elemLength;
            file.read(reinterpret_cast<char*>(&elemLength), sizeof(elemLength));
            string element;
            element.resize(elemLength);
            file.read(&element[0], elemLength);
            queues[name]->enqueue(element);
        }
    }

    for (uint32_t i = 0; i < openTableCount; i++) {
        uint32_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        string name;
        name.resize(nameLength);
        file.read(&name[0], nameLength);

        uint32_t tableSize;
        file.read(reinterpret_cast<char*>(&tableSize), sizeof(tableSize));

        if (openTables.find(name) != openTables.end()) {
            delete openTables[name];
        }
        openTables[name] = new OpenAddressingHashTable(10);

        for (uint32_t j = 0; j < tableSize; j++) {
            uint32_t keyLength, valueLength;
            file.read(reinterpret_cast<char*>(&keyLength), sizeof(keyLength));
            string key;
            key.resize(keyLength);
            file.read(&key[0], keyLength);

            file.read(reinterpret_cast<char*>(&valueLength), sizeof(valueLength));
            string value;
            value.resize(valueLength);
            file.read(&value[0], valueLength);

            openTables[name]->insert(key, value);
        }
    }

    for (uint32_t i = 0; i < chainTableCount; i++) {
        uint32_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        string name;
        name.resize(nameLength);
        file.read(&name[0], nameLength);

        uint32_t tableSize;
        file.read(reinterpret_cast<char*>(&tableSize), sizeof(tableSize));

        if (chainTables.find(name) != chainTables.end()) {
            delete chainTables[name];
        }
        chainTables[name] = new ChainHashTable(10);

        for (uint32_t j = 0; j < tableSize; j++) {
            uint32_t keyLength, valueLength;
            file.read(reinterpret_cast<char*>(&keyLength), sizeof(keyLength));
            string key;
            key.resize(keyLength);
            file.read(&key[0], keyLength);

            file.read(reinterpret_cast<char*>(&valueLength), sizeof(valueLength));
            string value;
            value.resize(valueLength);
            file.read(&value[0], valueLength);

            chainTables[name]->insert(key, value);
        }
    }

    for (uint32_t i = 0; i < treeCount; i++) {
        uint32_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        string name;
        name.resize(nameLength);
        file.read(&name[0], nameLength);

        uint32_t treeSize;
        file.read(reinterpret_cast<char*>(&treeSize), sizeof(treeSize));

        if (trees.find(name) != trees.end()) {
            delete trees[name];
        }
        trees[name] = new AVLTree();

        for (uint32_t j = 0; j < treeSize; j++) {
            int value;
            file.read(reinterpret_cast<char*>(&value), sizeof(value));
            trees[name]->insert(value);
        }
    }

    file.close();
}



void FCREATE() {
    if (commands.size() < 3) throw runtime_error("недостаточно параметров для FCREATE");
    const string listName = commands[2];

    if (simplyLists.find(listName) != simplyLists.end()) {
        cout << "Односвязный список " << listName << " уже существует" << endl;
        return;
    }

    simplyLists[listName] = new SimplyList();
    cout << "Создан односвязный список: " << listName << endl;
    saveToFile();
}

void FPUSHHEAD() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для FPUSHHEAD");
    const string listName = commands[2];
    const string value = commands[3];

    if (simplyLists.find(listName) == simplyLists.end()) {
        throw runtime_error("список " + listName + " не найден");
    }

    simplyLists[listName]->addHead(value);
    cout << "Добавлено в начало списка " << listName << ": " << value << endl;
    saveToFile();
}

void FPUSHTAIL() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для FPUSHTAIL");
    const string listName = commands[2];
    const string value = commands[3];

    if (simplyLists.find(listName) == simplyLists.end()) {
        throw runtime_error("список " + listName + " не найден");
    }

    simplyLists[listName]->addBack(value);
    cout << "Добавлено в конец списка " << listName << ": " << value << endl;
    saveToFile();
}

void FPUSHAFTER() {
    if (commands.size() < 5) throw runtime_error("недостаточно параметров для FPUSHAFTER");
    const string listName = commands[2];
    const int index = stoi(commands[3]);
    const string value = commands[4];

    if (simplyLists.find(listName) == simplyLists.end()) {
        throw runtime_error("список " + listName + " не найден");
    }

    simplyLists[listName]->addAfterIndex(index, value);
    cout << "Добавлено после индекса " << index << " в список " << listName << ": " << value << endl;
    saveToFile();
}

void FPUSHBEFORE() {
    if (commands.size() < 5) throw runtime_error("недостаточно параметров для FPUSHBEFORE");
    const string listName = commands[2];
    const int index = stoi(commands[3]);
    const string value = commands[4];

    if (simplyLists.find(listName) == simplyLists.end()) {
        throw runtime_error("список " + listName + " не найден");
    }

    simplyLists[listName]->addBeforeIndex(index, value);
    cout << "Добавлено перед индексом " << index << " в список " << listName << ": " << value << endl;
    saveToFile();
}

void FDELHEAD() {
    if (commands.size() < 3) throw runtime_error("недостаточно параметров для FDELHEAD");
    const string listName = commands[2];

    if (simplyLists.find(listName) == simplyLists.end()) {
        throw runtime_error("список " + listName + " не найден");
    }

    simplyLists[listName]->dellHead();
    cout << "Удален первый элемент из списка " << listName << endl;
    saveToFile();
}

void FDELTAIL() {
    if (commands.size() < 3) throw runtime_error("недостаточно параметров для FDELTAIL");
    const string listName = commands[2];

    if (simplyLists.find(listName) == simplyLists.end()) {
        throw runtime_error("список " + listName + " не найден");
    }

    simplyLists[listName]->dellBack();
    cout << "Удален последний элемент из списка " << listName << endl;
    saveToFile();
}

void FDELAFTER() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для FDELAFTER");
    const string listName = commands[2];
    const int index = stoi(commands[3]);

    if (simplyLists.find(listName) == simplyLists.end()) {
        throw runtime_error("список " + listName + " не найден");
    }

    simplyLists[listName]->dellAfterIndex(index);
    cout << "Удален элемент после индекса " << index << " из списка " << listName << endl;
    saveToFile();
}

void FDELBEFORE() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для FDELBEFORE");
    const string listName = commands[2];
    const int index = stoi(commands[3]);

    if (simplyLists.find(listName) == simplyLists.end()) {
        throw runtime_error("список " + listName + " не найден");
    }
    if (index <= 0) {
        throw runtime_error("нельзя удалить элемент до индекса 0");
    }

    simplyLists[listName]->dellBeforeIndex(index);
    cout << "Удален элемент перед индексом " << index << " из списка " << listName << endl;
    saveToFile();
}

void FDELVALUE() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для FDELVALUE");
    const string listName = commands[2];
    const string value = commands[3];

    if (simplyLists.find(listName) != simplyLists.end()) {
        throw runtime_error("список " + listName + " не найден");
    }

    simplyLists[listName]->dellValue(value);
    cout << "Удален элемент со значением " << value << " из списка " << listName << endl;
    saveToFile();
}

void FGET() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для FGET");
    const string listName = commands[2];
    const string value = commands[3];

    if (simplyLists.find(listName) == simplyLists.end()) {
        throw runtime_error("список " + listName + " не найден");
    }

    const bool found = simplyLists[listName]->searchValue(value);
    cout << "Элемент " << value << " в списке " << listName << (found ? " найден" : " не найден") << endl;
}

void DCREATE() {
    if (commands.size() < 3) throw runtime_error("недостаточно параметров для DCREATE");
    const string listName = commands[2];

    if (doublyLists.find(listName) != doublyLists.end()) {
        cout << "Двусвязный список " << listName << " уже существует" << endl;
        return;
    }

    doublyLists[listName] = new DoublyList();
    cout << "Создан двусвязный список: " << listName << endl;
    saveToFile();
}

void DPUSHHEAD() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для DPUSHHEAD");
    const string listName = commands[2];
    const string value = commands[3];

    if (doublyLists.find(listName) == doublyLists.end()) {
        throw runtime_error("двусвязный список " + listName + " не найден");
    }

    doublyLists[listName]->addHead(value);
    cout << "Добавлено в начало двусвязного списка " << listName << ": " << value << endl;
    saveToFile();
}

void DPUSHTAIL() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для DPUSHTAIL");
    const string listName = commands[2];
    const string value = commands[3];

    if (doublyLists.find(listName) == doublyLists.end()) {
        throw runtime_error("двусвязный список " + listName + " не найден");
    }

    doublyLists[listName]->addBack(value);
    cout << "Добавлено в конец двусвязного списка " << listName << ": " << value << endl;
    saveToFile();
}

void DPUSHAFTER() {
    if (commands.size() < 5) throw runtime_error("недостаточно параметров для DPUSHAFTER");
    const string listName = commands[2];
    const int index = stoi(commands[3]);
    const string value = commands[4];

    if (doublyLists.find(listName) == doublyLists.end()) {
        throw runtime_error("двусвязный список " + listName + " не найден");
    }

    doublyLists[listName]->addAfterIndex(index, value);
    cout << "Добавлено после индекса " << index << " в двусвязный список " << listName << ": " << value << endl;
    saveToFile();
}

void DPUSHBEFORE() {
    if (commands.size() < 5) throw runtime_error("недостаточно параметров для DPUSHBEFORE");
    const string listName = commands[2];
    const int index = stoi(commands[3]);
    const string value = commands[4];

    if (doublyLists.find(listName) == doublyLists.end()) {
        throw runtime_error("двусвязный список " + listName + " не найден");
    }

    doublyLists[listName]->addBeforeIndex(index, value);
    cout << "Добавлено перед индексом " << index << " в двусвязный список " << listName << ": " << value << endl;
    saveToFile();
}

void DDELHEAD() {
    if (commands.size() < 3) throw runtime_error("недостаточно параметров для DDELHEAD");
    const string listName = commands[2];

    if (doublyLists.find(listName) == doublyLists.end()) {
        throw runtime_error("двусвязный список " + listName + " не найден");
    }

    doublyLists[listName]->dellHead();
    cout << "Удален первый элемент из двусвязного списка " << listName << endl;
    saveToFile();
}

void DDELTAIL() {
    if (commands.size() < 3) throw runtime_error("недостаточно параметров для DDELTAIL");
    const string listName = commands[2];

    if (doublyLists.find(listName) == doublyLists.end()) {
        throw runtime_error("двусвязный список " + listName + " не найден");
    }

    doublyLists[listName]->dellBack();
    cout << "Удален последний элемент из двусвязного списка " << listName << endl;
    saveToFile();
}

void DDELAFTER() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для DDELAFTER");
    const string listName = commands[2];
    const int index = stoi(commands[3]);

    if (doublyLists.find(listName) == doublyLists.end()) {
        throw runtime_error("двусвязный список " + listName + " не найден");
    }

    doublyLists[listName]->dellAfterIndex(index);
    cout << "Удален элемент после индекса " << index << " из двусвязного списка " << listName << endl;
    saveToFile();
}

void DDELBEFORE() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для DDELBEFORE");
    const string listName = commands[2];
    const int index = stoi(commands[3]);

    if (doublyLists.find(listName) == doublyLists.end()) {
        throw runtime_error("двусвязный список " + listName + " не найден");
    }
    if (index <= 0) {
        throw runtime_error("нельзя удалить элемент до индекса 0");
    }

    doublyLists[listName]->dellBeforeIndex(index);
    cout << "Удален элемент перед индексом " << index << " из двусвязного списка " << listName << endl;
    saveToFile();
}

void DDELVALUE() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для DDELVALUE");
    const string listName = commands[2];
    const string value = commands[3];

    if (doublyLists.find(listName) == doublyLists.end()) {
        throw runtime_error("двусвязный список " + listName + " не найден");
    }

    doublyLists[listName]->dellValue(value);
    cout << "Удален элемент со значением " << value << " из двусвязного списка " << listName << endl;
    saveToFile();
}

void DGET() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для DGET");
    const string listName = commands[2];
    const string value = commands[3];

    if (doublyLists.find(listName) == doublyLists.end()) {
        throw runtime_error("двусвязный список " + listName + " не найден");
    }

    const bool found = doublyLists[listName]->searchValue(value);
    cout << "Элемент " << value << " в двусвязном списке " << listName << (found ? " найден" : " не найден") << endl;
}

void SCREATE() {
    if (commands.size() < 3) throw runtime_error("недостаточно параметров для SCREATE");
    const string stackName = commands[2];

    if (stacks.find(stackName) != stacks.end()) {
        cout << "Стек " << stackName << " уже существует" << endl;
        return;
    }

    stacks[stackName] = new Stack();
    cout << "Создан стек: " << stackName << endl;
    saveToFile();
}

void SPUSH() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для SPUSH");
    const string stackName = commands[2];
    const string value = commands[3];

    if (stacks.find(stackName) == stacks.end()) {
        throw runtime_error("стек " + stackName + " не найден");
    }

    stacks[stackName]->push(value);
    cout << "Добавлено в стек " << stackName << ": " << value << endl;
    saveToFile();
}

void SPOP() {
    if (commands.size() < 3) throw runtime_error("недостаточно параметров для SPOP");
    const string stackName = commands[2];

    if (stacks.find(stackName) == stacks.end()) {
        throw runtime_error("стек " + stackName + " не найден");
    }

    const string value = stacks[stackName]->pop();
    cout << "Извлечено из стека " << stackName << ": " << value << endl;
    saveToFile();
}

void QCREATE() {
    if (commands.size() < 3) throw runtime_error("недостаточно параметров для QCREATE");
    const string queueName = commands[2];

    if (queues.find(queueName) != queues.end()) {
        cout << "Очередь " << queueName << " уже существует" << endl;
        return;
    }

    queues[queueName] = new Queue();
    cout << "Создана очередь: " << queueName << endl;
    saveToFile();
}

void QPUSH() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для QPUSH");
    const string queueName = commands[2];
    const string value = commands[3];

    if (queues.find(queueName) == queues.end()) {
        throw runtime_error("очередь " + queueName + " не найдена");
    }

    queues[queueName]->enqueue(value);
    cout << "Добавлено в очередь " << queueName << ": " << value << endl;
    saveToFile();
}

void QPOP() {
    if (commands.size() < 3) throw runtime_error("недостаточно параметров для QPOP");
    const string queueName = commands[2];

    if (queues.find(queueName) == queues.end()) {
        throw runtime_error("очередь " + queueName + " не найдена");
    }

    const string value = queues[queueName]->dequeue();
    cout << "Извлечено из очереди " << queueName << ": " << value << endl;
    saveToFile();
}


void MCREATE() {
    if (commands.size() < 3) throw runtime_error("недостаточно параметров для MCREATE");
    const string arrayName = commands[2];

    if (arrays.find(arrayName) != arrays.end()) {
        cout << "Массив " << arrayName << " уже существует" << endl;
        return;
    }

    arrays[arrayName] = new dyArr(10);
    cout << "Создан массив: " << arrayName << endl;
    saveToFile();
}

void MPUSH() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для MPUSH");
    const string arrayName = commands[2];
    const string value = commands[3];

    if (arrays.find(arrayName) == arrays.end()) {
        throw runtime_error("массив " + arrayName + " не найден");
    }

    arrays[arrayName]->pushBack(value);
    cout << "Добавлено в конец массива " << arrayName << ": " << value << endl;
    saveToFile();
}

void MPUSHINDEX() {
    if (commands.size() < 5) throw runtime_error("недостаточно параметров для MPUSHINDEX");
    const string arrayName = commands[2];
    const int index = stoi(commands[3]);
    const string value = commands[4];

    if (arrays.find(arrayName) == arrays.end()) {
        throw runtime_error("массив " + arrayName + " не найден");
    }

    arrays[arrayName]->pushInIndex(value, index);
    cout << "Добавлено по индексу " << index << " в массив " << arrayName << ": " << value << endl;
    saveToFile();
}

void MGET() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для MGET");
    const string arrayName = commands[2];
    const int index = stoi(commands[3]);

    if (arrays.find(arrayName) == arrays.end()) {
        throw runtime_error("массив " + arrayName + " не найден");
    }

    const string value = arrays[arrayName]->getElement(index);
    cout << "Элемент по индексу " << index << " в массиве " << arrayName << ": " << value << endl;
}

void MDELINDEX() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для MDELINDEX");
    const string arrayName = commands[2];
    const int index = stoi(commands[3]);

    if (arrays.find(arrayName) == arrays.end()) {
        throw runtime_error("массив " + arrayName + " не найден");
    }

    arrays[arrayName]->removeAtIndex(index);
    cout << "Удален элемент по индексу " << index << " из массива " << arrayName << endl;
    saveToFile();
}

void MREPLACE() {
    if (commands.size() < 5) throw runtime_error("недостаточно параметров для MREPLACE");
    const string arrayName = commands[2];
    const int index = stoi(commands[3]);
    const string value = commands[4];

    if (arrays.find(arrayName) == arrays.end()) {
        throw runtime_error("массив " + arrayName + " не найден");
    }

    arrays[arrayName]->replaceAtIndex(value, index);
    cout << "Заменен элемент по индексу " << index << " в массиве " << arrayName << " на: " << value << endl;
    saveToFile();
}

void MLEN() {
    if (commands.size() < 3) throw runtime_error("недостаточно параметров для MLEN");
    const string arrayName = commands[2];

    if (arrays.find(arrayName) == arrays.end()) {
        throw runtime_error("массив " + arrayName + " не найден");
    }

    const int length = arrays[arrayName]->getLength();
    cout << "Длина массива " << arrayName << ": " << length << endl;
}

void TCREATE() {
    if (commands.size() < 3) throw runtime_error("недостаточно параметров для TCREATE");
    const string treeName = commands[2];

    if (trees.find(treeName) != trees.end()) {
        cout << "AVL-дерево " << treeName << " уже существует" << endl;
        return;
    }

    trees[treeName] = new AVLTree();
    cout << "Создано AVL-дерево: " << treeName << endl;
    saveToFile();
}

void TPUSH() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для TPUSH");
    const string treeName = commands[2];
    const int value = stoi(commands[3]);

    if (trees.find(treeName) == trees.end()) {
        throw runtime_error("AVL-дерево " + treeName + " не найдено");
    }

    trees[treeName]->insert(value);
    cout << "Добавлено в AVL-дерево " << treeName << ": " << value << endl;
    saveToFile();
}

void TDEL() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для TDEL");
    const string treeName = commands[2];
    const int value = stoi(commands[3]);

    if (trees.find(treeName) == trees.end()) {
        throw runtime_error("AVL-дерево " + treeName + " не найдено");
    }

    trees[treeName]->remove(value);
    cout << "Удалено из AVL-дерева " << treeName << ": " << value << endl;
    saveToFile();
}

void TGET() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для TGET");
    const string treeName = commands[2];
    const int value = stoi(commands[3]);

    if (trees.find(treeName) == trees.end()) {
        throw runtime_error("AVL-дерево " + treeName + " не найдено");
    }

    const bool found = trees[treeName]->contains(value);
    cout << "Элемент " << value << " в AVL-дереве " << treeName << (found ? " найден" : " не найден") << endl;
}

void TABLEOPENCREATE() {
    if (commands.size() < 3) throw runtime_error("недостаточно параметров для TABLEOPENCREATE");
    const string tableName = commands[2];

    if (openTables.find(tableName) != openTables.end()) {
        cout << "Хэш-таблица " << tableName << " уже существует" << endl;
        return;
    }

    openTables[tableName] = new OpenAddressingHashTable(10);
    cout << "Создана хэш-таблица (открытая адресация): " << tableName << endl;
    saveToFile();
}

void TABLEOPENADD() {
    if (commands.size() < 5) throw runtime_error("недостаточно параметров для TABLEOPENADD");
    const string tableName = commands[2];
    const string key = commands[3];
    const string value = commands[4];

    if (openTables.find(tableName) == openTables.end()) {
        throw runtime_error("хэш-таблица " + tableName + " не найдена");
    }

    const bool success = openTables[tableName]->insert(key, value);
    if (success) {
        cout << "Добавлено в хэш-таблицу " << tableName << ": ключ=" << key << ", значение=" << value << endl;
    } else {
        cout << "Не удалось добавить в хэш-таблицу " << tableName << endl;
    }
    saveToFile();
}

void TABLEOPENDEL() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для TABLEOPENDEL");
    const string tableName = commands[2];
    const string key = commands[3];

    if (openTables.find(tableName) == openTables.end()) {
        throw runtime_error("хэш-таблица " + tableName + " не найдена");
    }

    const bool success = openTables[tableName]->deleteKey(key);
    if (success) {
        cout << "Удалено из хэш-таблицы " << tableName << ": ключ=" << key << endl;
    } else {
        cout << "Не удалось удалить из хэш-таблицы " << tableName << ": ключ=" << key << endl;
    }
    saveToFile();
}

void TABLEOPENGET() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для TABLEOPENGET");
    const string tableName = commands[2];
    const string key = commands[3];

    if (openTables.find(tableName) == openTables.end()) {
        throw runtime_error("хэш-таблица " + tableName + " не найдена");
    }

    const auto result = openTables[tableName]->search(key);
    if (get<0>(result)) {
        cout << "Найдено в хэш-таблице " << tableName << ": ключ=" << get<1>(result) << ", значение=" << get<2>(result) << endl;
    } else {
        cout << "Не найдено в хэш-таблице " << tableName << ": ключ=" << key << endl;
    }
}

void TABLECHAINCREATE() {
    if (commands.size() < 3) throw runtime_error("недостаточно параметров для TABLECHAINCREATE");
    const string tableName = commands[2];

    if (chainTables.find(tableName) != chainTables.end()) {
        cout << "Хэш-таблица " << tableName << " уже существует" << endl;
        return;
    }

    chainTables[tableName] = new ChainHashTable(10);
    cout << "Создана хэш-таблица (метод цепочек): " << tableName << endl;
    saveToFile();
}

void TABLECHAINADD() {
    if (commands.size() < 5) throw runtime_error("недостаточно параметров для TABLECHAINADD");
    const string tableName = commands[2];
    const string key = commands[3];
    const string value = commands[4];

    if (chainTables.find(tableName) == chainTables.end()) {
        throw runtime_error("хэш-таблица " + tableName + " не найдена");
    }

    const bool success = chainTables[tableName]->insert(key, value);
    if (success) {
        cout << "Добавлено в хэш-таблицу " << tableName << ": ключ=" << key << ", значение=" << value << endl;
    } else {
        cout << "Не удалось добавить в хэш-таблицу " << tableName << endl;
    }
    saveToFile();
}

void TABLECHAINDEL() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для TABLECHAINDEL");
    const string tableName = commands[2];
    const string key = commands[3];

    if (chainTables.find(tableName) == chainTables.end()) {
        throw runtime_error("хэш-таблица " + tableName + " не найдена");
    }

    const bool success = chainTables[tableName]->deleteKey(key);
    if (success) {
        cout << "Удалено из хэш-таблицы " << tableName << ": ключ=" << key << endl;
    } else {
        cout << "Не удалось удалить из хэш-таблицы " << tableName << ": ключ=" << key << endl;
    }
    saveToFile();
}

void TABLECHAINGET() {
    if (commands.size() < 4) throw runtime_error("недостаточно параметров для TABLECHAINGET");
    const string tableName = commands[2];
    const string key = commands[3];

    if (chainTables.find(tableName) == chainTables.end()) {
        throw runtime_error("хэш-таблица " + tableName + " не найдена");
    }

    const auto result = chainTables[tableName]->search(key);
    if (get<0>(result)) {
        cout << "Найдено в хэш-таблице " << tableName << ": ключ=" << get<1>(result) << ", значение=" << get<2>(result) << endl;
    } else {
        cout << "Не найдено в хэш-таблице " << tableName << ": ключ=" << key << endl;
    }
}

void PRINT() {
    if (commands.size() < 3) throw runtime_error("недостаточно параметров для PRINT");
    const string name = commands[2];

    if (arrays.find(name) != arrays.end()) {
        cout << "Массив " << name << ": ";
        arrays[name]->printDyArr();
    }
    else if (simplyLists.find(name) != simplyLists.end()) {
        cout << "Односвязный список " << name << ": ";
        simplyLists[name]->print();
    }
    else if (doublyLists.find(name) != doublyLists.end()) {
        cout << "Двусвязный список " << name << ": ";
        doublyLists[name]->print();
    }
    else if (stacks.find(name) != stacks.end()) {
        cout << "Стек " << name << ": ";
        stacks[name]->print();
    }
    else if (queues.find(name) != queues.end()) {
        cout << "Очередь " << name << ": ";
        queues[name]->print();
    }
    else if (openTables.find(name) != openTables.end()) {
        cout << "Хэш-таблица (открытая адресация) " << name << ":" << endl;
        openTables[name]->print();
    }
    else if (chainTables.find(name) != chainTables.end()) {
        cout << "Хэш-таблица (метод цепочек) " << name << ":" << endl;
        chainTables[name]->print();
    }
    else if (trees.find(name) != trees.end()) {
        cout << "AVL-дерево " << name << ":" << endl;
        trees[name]->print();
    }
    else {
        throw runtime_error("структура " + name + " не найдена");
    }
}

void compCommand() {
    if (commands.size() < 2) {
        cout << "Недостаточно команд для выполнения" << endl;
        return;
    }

    const string cmd = toUpperCase(commands[1]);

    try {
        if (cmd == "FCREATE") FCREATE();
        else if (cmd == "FPUSHHEAD") FPUSHHEAD();
        else if (cmd == "FPUSHTAIL") FPUSHTAIL();
        else if (cmd == "FPUSHAFTER") FPUSHAFTER();
        else if (cmd == "FPUSHBEFORE") FPUSHBEFORE();
        else if (cmd == "FDELHEAD") FDELHEAD();
        else if (cmd == "FDELTAIL") FDELTAIL();
        else if (cmd == "FDELAFTER") FDELAFTER();
        else if (cmd == "FDELBEFORE") FDELBEFORE();
        else if (cmd == "FDELVALUE") FDELVALUE();
        else if (cmd == "FGET") FGET();

        else if (cmd == "DCREATE") DCREATE();
        else if (cmd == "DPUSHHEAD") DPUSHHEAD();
        else if (cmd == "DPUSHTAIL") DPUSHTAIL();
        else if (cmd == "DPUSHAFTER") DPUSHAFTER();
        else if (cmd == "DPUSHBEFORE") DPUSHBEFORE();
        else if (cmd == "DDELHEAD") DDELHEAD();
        else if (cmd == "DDELTAIL") DDELTAIL();
        else if (cmd == "DDELAFTER") DDELAFTER();
        else if (cmd == "DDELBEFORE") DDELBEFORE();
        else if (cmd == "DDELVALUE") DDELVALUE();
        else if (cmd == "DGET") DGET();

        else if (cmd == "SCREATE") SCREATE();
        else if (cmd == "SPUSH") SPUSH();
        else if (cmd == "SPOP") SPOP();

        else if (cmd == "QCREATE") QCREATE();
        else if (cmd == "QPUSH") QPUSH();
        else if (cmd == "QPOP") QPOP();

        else if (cmd == "MCREATE") MCREATE();
        else if (cmd == "MPUSH") MPUSH();
        else if (cmd == "MPUSHINDEX") MPUSHINDEX();
        else if (cmd == "MGET") MGET();
        else if (cmd == "MDELINDEX") MDELINDEX();
        else if (cmd == "MREPLACE") MREPLACE();
        else if (cmd == "MLEN") MLEN();

        else if (cmd == "TCREATE") TCREATE();
        else if (cmd == "TPUSH") TPUSH();
        else if (cmd == "TDEL") TDEL();
        else if (cmd == "TGET") TGET();

        else if (cmd == "TABLEOPENCREATE") TABLEOPENCREATE();
        else if (cmd == "TABLEOPENADD") TABLEOPENADD();
        else if (cmd == "TABLEOPENDEL") TABLEOPENDEL();
        else if (cmd == "TABLEOPENGET") TABLEOPENGET();

        else if (cmd == "TABLECHAINCREATE") TABLECHAINCREATE();
        else if (cmd == "TABLECHAINADD") TABLECHAINADD();
        else if (cmd == "TABLECHAINDEL") TABLECHAINDEL();
        else if (cmd == "TABLECHAINGET") TABLECHAINGET();

        else if (cmd == "PRINT") PRINT();

        else cout << "Команда не найдена: " << cmd << endl;
    } catch (const exception& e) {
        cout << "Ошибка выполнения команды: " << e.what() << endl;
    }
}

void executeCommand(const string& command) {
    stringstream ss(command);
    string token;
    vector<string> tempCommands;

    while (ss >> token) {
        tempCommands.push_back(token);
    }

    if (tempCommands.empty()) return;

    commands = tempCommands;
    compCommand();
}

void rewrite(const pair<string, string>& arguments) {
    commands.clear();
    commands.push_back(arguments.first);
    currentFilename = arguments.first;

    stringstream ss(arguments.second);
    string token;
    while (ss >> token) {
        commands.push_back(token);
    }

    loadFromFile(commands[0]);


    compCommand();
}
