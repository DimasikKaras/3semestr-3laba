# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(DataStructuresTests)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройки для покрытия кода
if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

# Поиск GoogleTest
find_package(GTest REQUIRED)

# Включение каталогов
include_directories(include)
include_directories(src)

# Список исходных файлов
set(SOURCES
    src/dyArr.cpp
    src/simplyList.cpp
    src/doublyList.cpp
    src/stack.cpp
    src/queue.cpp
    src/avlTree.cpp
    src/OpenAdressingHashTable.cpp
    src/chainHashTable.cpp
)

# Создание исполняемого файла тестов
add_executable(tests 
    tests/tests.cpp
    ${SOURCES}
)

# Связывание с GoogleTest
target_link_libraries(tests GTest::GTest GTest::Main pthread)

find_package(benchmark REQUIRED)

# Бенчмарки
add_executable(benchmarks
    benchmarks/benchmarks.cpp
    ${SOURCES}
)

target_link_libraries(benchmarks benchmark::benchmark pthread)

# Целевая задача для запуска бенчмарков
add_custom_target(run_benchmarks
    COMMAND ./benchmarks --benchmark_format=console
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS benchmarks
)
