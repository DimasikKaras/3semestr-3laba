#include <iostream>
#include <string>
#include <utility>

#include "menu.h"
#include "executingCommands.h"

using namespace std;

pair<string, string> arguments;
bool useBinaryFormat = false;

int main(const int argc, char* argv[]) {
    try {
        if (argc < 2) {
            throw runtime_error("недостаточно аргументов.");
        }

        string filename, query;

        for (int i = 1; i < argc; i++) {
            string arg = argv[i];

            if (arg == "--file" || arg == "-f") {
                if (i + 1 < argc) {
                    filename = argv[++i];
                } else {
                    throw runtime_error("после ключа " + arg + " должно быть указано имя файла.");
                }
            }
            else if (arg == "--query" || arg == "-q") {
                if (i + 1 < argc) {
                    query = argv[++i];
                } else {
                    throw runtime_error("после ключа " + arg + " должна быть указана команда.");
                }
            }
            else if (arg == "--binary" || arg == "-b") {
                useBinaryFormat = true;
            }
            else if (arg == "--help" || arg == "-h") {
                help();
                return 0;
            }
            else {
                throw runtime_error ("неизвестный ключ:" + arg + ".");
            }
        }
        if (filename.empty()) throw runtime_error("нет названия файла.");
        arguments.first = filename;
        if (query.empty()) throw runtime_error("нет команды.");
        arguments.second = query;
        rewrite(arguments);

    } catch (const exception& e) {
        cout << "Ошибка: " << e.what() << " " << "Для использования введите: ./main -h или ./main --help" << endl;
        return 1;
    }
}