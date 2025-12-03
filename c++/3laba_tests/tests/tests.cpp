#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <algorithm>
#include "avlTree.h"
#include "HashTable.h"
#include "doublyList.h"
#include "dyArr.h"
#include "queue.h"
#include "simplyList.h"
#include "stack.h"

using namespace std;

TEST(AVLTreeTest, ConstructorDestructor) {
    AVLTree* tree = new AVLTree();
    delete tree;
}

TEST(AVLTreeTest, InsertContainsRemove) {
    AVLTree tree;
    
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    
    EXPECT_TRUE(tree.contains(50));
    EXPECT_TRUE(tree.contains(20));
    EXPECT_FALSE(tree.contains(100));
    
    tree.remove(20);
    tree.remove(30);
    tree.remove(50);
    
    EXPECT_FALSE(tree.contains(20));
    EXPECT_FALSE(tree.contains(30));
    EXPECT_FALSE(tree.contains(50));
    EXPECT_TRUE(tree.contains(40));
    EXPECT_TRUE(tree.contains(70));
}

TEST(AVLTreeTest, RightRotation) {
    AVLTree tree;
    
    // LL case - правая ротация
    tree.insert(30);
    tree.insert(20);
    tree.insert(10);
    
    vector<int> elements = tree.getElements();
    EXPECT_EQ(elements.size(), 3);
    EXPECT_TRUE(is_sorted(elements.begin(), elements.end()));
}

TEST(AVLTreeTest, LeftRotation) {
    AVLTree tree;
    
    // RR case - левая ротация
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    
    vector<int> elements = tree.getElements();
    EXPECT_EQ(elements.size(), 3);
    EXPECT_TRUE(is_sorted(elements.begin(), elements.end()));
}

TEST(AVLTreeTest, LeftRightRotation) {
    AVLTree tree;
    
    // LR case - лево-правая ротация
    tree.insert(30);
    tree.insert(10);
    tree.insert(20);
    
    vector<int> elements = tree.getElements();
    EXPECT_EQ(elements.size(), 3);
    EXPECT_TRUE(is_sorted(elements.begin(), elements.end()));
}

TEST(AVLTreeTest, GetElementsPrint) {
    AVLTree tree;
    
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    
    vector<int> elements = tree.getElements();
    EXPECT_EQ(elements.size(), 5);
    EXPECT_TRUE(is_sorted(elements.begin(), elements.end()));
    
    EXPECT_NO_THROW(tree.print());
}

TEST(AVLTreeTest, EdgeCases) {
    AVLTree tree;
    
    EXPECT_FALSE(tree.contains(42));
    EXPECT_NO_THROW(tree.remove(42));
    
    tree.insert(42);
    tree.insert(42); // duplicate
    
    vector<int> elements = tree.getElements();
    EXPECT_EQ(elements.size(), 1);
    
    tree.remove(42);
    EXPECT_TRUE(tree.getElements().empty());
}

//==============Тест метода цепочек================
TEST(ChainHashTable, TestsOne){
	ChainHashTable table(2);
	table.print();
	EXPECT_TRUE(table.insert("Alice", "89532123"));
	EXPECT_TRUE(table.insert("Alice", "23213122"));
	EXPECT_TRUE(table.insert("BOB", "22"));
	EXPECT_TRUE(table.insert("ALEX", "32"));
	EXPECT_TRUE(table.deleteKey("Alice"));
	table.print();
	auto el = table.search("BOB");
	EXPECT_TRUE(get<0>(el));
	el = table.search("Petr");
	EXPECT_FALSE(get<0>(el));
	
	auto allEl = table.getAllElements();
	
	EXPECT_TRUE((allEl[0].first == "BOB") || (allEl[0].first) == "ALEX");

}
//=========Двусвязный список====


TEST(DoublyListTest, testDoublyListOne){
    DoublyList list;
    
    list.addHead("Anna");
    EXPECT_THROW(list.addHead(""), runtime_error);
    
    list.dellHead();
    EXPECT_THROW(list.dellHead(), runtime_error);
    
    list.addBack("Bob");
    list.addBack("Alice");
    EXPECT_THROW(list.addBack(""), runtime_error);
    list.dellBack();
    
    list.addBack("Anna");
    list.addBack("Alex");
    list.addBack("Linux");
    list.print();
    list.dellBack();    
    
    // Исправляем ожидания для dellValue
    EXPECT_THROW(list.dellValue(""), runtime_error);
    EXPECT_NO_THROW(list.dellValue("Bob"));
    EXPECT_NO_THROW(list.dellValue("Anna"));
    EXPECT_NO_THROW(list.dellValue("Alex"));
    list.print();
}

TEST(DoublyListTest, testDoublyListTwo){
    DoublyList list;
    
    // Тестируем исключения на пустом списке
    EXPECT_THROW(list.addAfterIndex(-10, "Anna"), runtime_error);
    EXPECT_THROW(list.addAfterIndex(10, ""), runtime_error);
    EXPECT_THROW(list.addAfterIndex(10, "Anna"), runtime_error);
    
    // Добавляем первый элемент
    EXPECT_NO_THROW(list.addAfterIndex(0, "Anna"));
    
    // Добавляем еще элементы
    EXPECT_NO_THROW(list.addAfterIndex(0, "Bob")); // После индекса 0
    EXPECT_NO_THROW(list.addAfterIndex(1, "Alice"));
    
    // Тестируем неверные индексы для удаления
    EXPECT_THROW(list.dellAfterIndex(-10), runtime_error);
    EXPECT_THROW(list.dellAfterIndex(10), runtime_error); // Добавил проверку большого индекса
    
    // Удаляем существующий элемент
    EXPECT_NO_THROW(list.dellAfterIndex(1));
    
    // Тестируем dellBeforeIndex
    EXPECT_THROW(list.dellBeforeIndex(-10), runtime_error);
    EXPECT_THROW(list.dellBeforeIndex(0), runtime_error);
    
    // Добавляем элементы для дальнейшего тестирования
    list.addBack("Tom");
    list.addBack("John");
    list.printReverse();
    
    // Теперь можно безопасно удалить
    EXPECT_NO_THROW(list.dellBeforeIndex(2));
    
    // Добавляем перед индексом
    EXPECT_NO_THROW(list.addBeforeIndex(1, "Alice"));
    
    // Тестируем второй список отдельно
    DoublyList lists;
    lists.printReverse();
    EXPECT_NO_THROW(lists.addBeforeIndex(0, "Alice"));
}

TEST(Doublylist, testDoublylistThird){
	DoublyList list;
	list.addBack("Anna");
	list.addBack("Alex");
	list.addBack("Linux");
	
	EXPECT_TRUE(list.searchValue("Anna"));
	EXPECT_FALSE(list.searchValue("Alice"));
	
	auto w = list.getHead();
}

TEST(DoublyListTest, DellValueEdgeCases){
    DoublyList list;
    
    // Создаем список: A <-> B <-> C
    list.addBack("A");
    list.addBack("B"); 
    list.addBack("C");
    
    // Удаляем из середины (покрывает строки 213-215)
    EXPECT_NO_THROW(list.dellValue("B"));
    
    // Удаляем хвост (покрывает строки 208-211)
    EXPECT_NO_THROW(list.dellValue("C"));
    
    // Удаляем голову (покрывает другое условие)
    EXPECT_NO_THROW(list.dellValue("A"));
    
    // Проверяем что список пустой
    EXPECT_THROW(list.dellHead(), runtime_error);
}

//================Динамический массив============

TEST(DyArrTest, ConstructorDestructor) {
    dyArr* arr = new dyArr(5);
    EXPECT_EQ(arr->getLength(), 0);
    delete arr;
}

TEST(DyArrTest, BasicOperations) {
    dyArr arr(3);
    
    arr.pushBack("A");
    arr.pushBack("B");
    arr.pushBack("C");
    
    EXPECT_EQ(arr.getLength(), 3);
    EXPECT_EQ(arr.getElement(0), "A");
    EXPECT_EQ(arr.getElement(1), "B");
    EXPECT_EQ(arr.getElement(2), "C");
}

TEST(DyArrTest, AutoResize) {
    dyArr arr(2);
    
    arr.pushBack("A");
    arr.pushBack("B");
    arr.pushBack("C"); // Trigger resize
    
    EXPECT_EQ(arr.getLength(), 3);
    EXPECT_EQ(arr.getElement(2), "C");
}

TEST(DyArrTest, InsertAndRemove) {
    dyArr arr(5);
    
    arr.pushBack("A");
    arr.pushBack("C");
    arr.pushInIndex("B", 1);
    
    EXPECT_EQ(arr.getElement(1), "B");
    
    arr.removeAtIndex(1);
    EXPECT_EQ(arr.getElement(1), "C");
    
    arr.replaceAtIndex("X", 0);
    EXPECT_EQ(arr.getElement(0), "X");
}

TEST(DyArrTest, CapacityExpansionOnPushInIndex) {
    dyArr arr(2);
    arr.pushBack("A");
    arr.pushBack("B");
    
    // Этот вызов должен вызвать расширение
    arr.pushInIndex("C", 1); // Вставка в середину
    
    EXPECT_EQ(arr.getElement(0), "A");
    EXPECT_EQ(arr.getElement(1), "C");
    EXPECT_EQ(arr.getElement(2), "B");
}


TEST(DyArrTest, ComplexScenario) {
    dyArr arr(2);
    
    for (int i = 0; i < 5; i++) {
        arr.pushBack("elem" + to_string(i));
    }
    
    arr.pushInIndex("middle", 2);
    arr.removeAtIndex(3);
    arr.replaceAtIndex("updated", 1);
    
    EXPECT_EQ(arr.getLength(), 5);
    EXPECT_NO_THROW(arr.printDyArr());
}


//================Метод открытой адресации============

TEST(OpenAdressingHashTable, qwer){
	EXPECT_THROW(OpenAddressingHashTable table(-1), runtime_error);
	OpenAddressingHashTable table(2);
	
	EXPECT_TRUE(table.insert("Alice", "89532123"));
	EXPECT_TRUE(table.insert("Alice", "23213122"));
	EXPECT_TRUE(table.insert("BOB", "22"));
	EXPECT_TRUE(table.insert("ALEX", "32"));
	EXPECT_TRUE(table.deleteKey("Alice"));
	table.print();
	auto el = table.search("BOB");
	EXPECT_TRUE(get<0>(el));
	el = table.search("Petr");
	EXPECT_FALSE(get<0>(el));
	
	auto allEl = table.getAllElements();
	
	EXPECT_TRUE((allEl[0].first == "BOB") || (allEl[0].first) == "ALEX");
}

//================Очередь============

TEST(Queue, q){
	Queue queue;
	queue.enqueue("A1");
	queue.enqueue("A2");
	queue.enqueue("Б2");
	
	queue.print();
	
	EXPECT_NO_THROW(queue.dequeue());
	EXPECT_NO_THROW(queue.dequeue());
	EXPECT_NO_THROW(queue.dequeue());
	
	queue.print();
	
	EXPECT_THROW(queue.dequeue(), runtime_error);
}

//================= Односвязный список==================

TEST(SimplyList, testSimlyOne){
    SimplyList list;
    
	list.addHead("Anna");
	EXPECT_THROW(list.addHead(""), runtime_error);
	
	list.dellHead();
	EXPECT_THROW(list.dellHead(), runtime_error);
	
	list.addBack("Bob");
	list.addBack("Alice");
	EXPECT_THROW(list.addBack(""), runtime_error);
	list.dellBack();
	
	list.addBack("Anna");
	list.addBack("Alex");
	list.addBack("Linux");
	list.print();
	list.dellBack();	
	
	EXPECT_THROW(list.dellValue(""), runtime_error);
	EXPECT_THROW(list.dellValue("Max"), runtime_error);
	EXPECT_NO_THROW(list.dellValue("Bob"));
	EXPECT_NO_THROW(list.dellValue("Anna"));
	EXPECT_NO_THROW(list.dellValue("Alex"));
	list.print();
	
}

TEST(SimplyList, testSimlyTwo){
	SimplyList list;
	EXPECT_THROW(list.addAfterIndex(-10, "Anna"), runtime_error);
	EXPECT_THROW(list.addAfterIndex(10, ""), runtime_error);
	
	EXPECT_THROW(list.addAfterIndex(10, "Anna"), runtime_error);
	EXPECT_NO_THROW(list.addAfterIndex(0, "Anna"));
	EXPECT_NO_THROW(list.addAfterIndex(0, "Bob"));
	EXPECT_THROW(list.addAfterIndex(10, "Alice"), runtime_error);
	EXPECT_NO_THROW(list.addAfterIndex(1, "Alice"));
	
	EXPECT_THROW(list.dellAfterIndex(-10), runtime_error);
	EXPECT_NO_THROW(list.dellAfterIndex(1));	
	
	EXPECT_THROW(list.dellBeforeIndex(-10), runtime_error);
	EXPECT_THROW(list.dellBeforeIndex(0), runtime_error);
	EXPECT_NO_THROW(list.dellBeforeIndex(1));
	list.addBack("Anna");
	EXPECT_NO_THROW(list.dellBeforeIndex(2));
	
	EXPECT_NO_THROW(list.addBeforeIndex(1, "Alice"));
	
	list.~SimplyList();
	
	SimplyList lists;
	
	EXPECT_NO_THROW(lists.addBeforeIndex(0, "Alice"));
	
}


TEST(SimplyList, testSimlyThird){
	SimplyList list;
	list.addBack("Anna");
	list.addBack("Alex");
	list.addBack("Linux");
	
	EXPECT_TRUE(list.searchValue("Anna"));
	EXPECT_FALSE(list.searchValue("Alice"));
	
	auto w = list.getHead();
}

//================СТЕК====================

TEST(Stack, testStack){
	Stack stack;

	EXPECT_THROW(stack.pop(), runtime_error);
	stack.print();
	
	stack.push("1");
	stack.push("2");
	stack.push("3");
	stack.push("4");
	
	stack.print();
	
	stack.pop();
	stack.pop();
	stack.pop();
	stack.pop();
}

// Главная функция
int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
