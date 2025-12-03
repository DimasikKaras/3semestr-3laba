#!/bin/bash
echo "=== ПОЛНЫЙ ЗАПУСК ПРОГРАММЫ С ПОКРЫТИЕМ GCOVR ==="

# Переходим в корень проекта
cd "$(dirname "$0")"
PROJECT_ROOT=$(pwd)
echo "Корневая директория: $PROJECT_ROOT"

rm -rf buidl
# Создаем build директорию если ее нет
if [ ! -d "build" ]; then
    echo "Создаем build директорию..."
    mkdir build
fi

cd build
echo "Текущая директория: $(pwd)"

# Шаг 1: Конфигурация CMake
echo "=== ШАГ 1: Конфигурация CMake ==="
cmake -DCMAKE_BUILD_TYPE=Coverage .. || { echo "Ошибка CMake!"; exit 1; }

# Шаг 2: Сборка проекта
echo "=== ШАГ 2: Сборка проекта ==="
make -j$(nproc) || { echo "Ошибка сборки!"; exit 1; }

# Шаг 3: Запуск тестов
echo "=== ШАГ 3: Запуск тестов ==="
./tests --gtest_output=xml:test_results.xml

# Сохраняем результат тестов
TEST_RESULT=$?
echo "Результат тестов: $TEST_RESULT"

# Шаг 4: Генерация отчетов покрытия gcovr
echo "=== ШАГ 4: Генерация отчетов покрытия GCOVR ==="

# Подробный HTML отчет
echo "--- Генерация детального HTML отчета ---"
gcovr -r .. --html --html-details -o coverage_report.html

# Сводный HTML отчет
echo "--- Генерация сводного HTML отчета ---"
gcovr -r .. --html -o coverage_summary.html

# XML отчет для CI
echo "--- Генерация XML отчета ---"
gcovr -r .. --xml -o coverage_report.xml

# Текстовый отчет в консоль
echo "--- Генерация текстового отчета ---"
gcovr -r .. --print-summary

# Шаг 5: Вывод информации
echo ""
echo "=== РЕЗУЛЬТАТЫ ==="
if [ $TEST_RESULT -eq 0 ]; then
    echo "✅ Все тесты прошли успешно!"
else
    echo "❌ Некоторые тесты не прошли"
fi

echo ""
echo "=== СГЕНЕРИРОВАННЫЕ ОТЧЕТЫ GCOVR ==="
echo "📊 Детальный HTML отчет:  file://$PROJECT_ROOT/build/coverage_report.html"
echo "📋 Сводный HTML отчет:    file://$PROJECT_ROOT/build/coverage_summary.html"
echo "📋 XML отчет (для CI):    $PROJECT_ROOT/build/coverage_report.xml"
echo "🧪 Результаты тестов:     $PROJECT_ROOT/build/test_results.xml"

echo ""
echo "=== ДЛЯ ПРОСМОТРА ОТЧЕТОВ ==="
echo "Откройте в браузере:"
echo "  xdg-open \"$PROJECT_ROOT/build/coverage_report.html\""

# Автоматически открываем отчет
if command -v xdg-open &> /dev/null; then
    echo "Автоматически открываю детальный отчет..."
    xdg-open "$PROJECT_ROOT/build/coverage_report.html"
elif command -v open &> /dev/null; then
    echo "Автоматически открываю детальный отчет..."
    open "$PROJECT_ROOT/build/coverage_report.html"
fi

exit $TEST_RESULT
