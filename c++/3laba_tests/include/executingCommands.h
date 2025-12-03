#ifndef EXECUTING_COMMANDS_H
#define EXECUTING_COMMANDS_H

#include <string>
#include <map>
#include <vector>
#include <utility>

class dyArr;
class SimplyList;
class DoublyList;
class Stack;
class Queue;
class OpenAddressingHashTable;
class ChainHashTable;
class AVLTree;

extern std::map<std::string, dyArr*> arrays;
extern std::map<std::string, SimplyList*> simplyLists;
extern std::map<std::string, DoublyList*> doublyLists;
extern std::map<std::string, Stack*> stacks;
extern std::map<std::string, Queue*> queues;
extern std::map<std::string, OpenAddressingHashTable*> openTables;
extern std::map<std::string, ChainHashTable*> chainTables;
extern std::map<std::string, AVLTree*> trees;

extern std::vector<std::string> commands;
extern std::string currentFilename;

extern bool useBinaryFormat;

void saveToFile();
void loadFromFile(const std::string& filename);

void saveToFileText();
void loadFromFileText(const std::string& filename);

void saveToFileBinary();
void loadFromFileBinary(const std::string& filename);


void executeCommand(const std::string& command);
void compCommand();
void rewrite(const std::pair<std::string, std::string>& arguments);

void FCREATE();
void FPUSHHEAD();
void FPUSHTAIL();
void FPUSHAFTER();
void FPUSHBEFORE();
void FDELHEAD();
void FDELTAIL();
void FDELAFTER();
void FDELBEFORE();
void FDELVALUE();
void FGET();


void DCREATE();
void DPUSHHEAD();
void DPUSHTAIL();
void DPUSHAFTER();
void DPUSHBEFORE();
void DDELHEAD();
void DDELTAIL();
void DDELAFTER();
void DDELBEFORE();
void DDELVALUE();
void DGET();


void SCREATE();
void SPUSH();
void SPOP();


void QCREATE();
void QPUSH();
void QPOP();


void MCREATE();
void MPUSH();
void MPUSHINDEX();
void MGET();
void MDELINDEX();
void MREPLACE();
void MLEN();


void TCREATE();
void TPUSH();
void TDEL();
void TGET();


void TABLEOPENCREATE();
void TABLEOPENADD();
void TABLEOPENDEL();
void TABLEOPENGET();


void TABLECHAINCREATE();
void TABLECHAINADD();
void TABLECHAINDEL();
void TABLECHAINGET();


void PRINT();

#endif
