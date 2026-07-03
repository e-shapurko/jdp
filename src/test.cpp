#include <cstdint>
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include "jserializer.h"

// КАК СОБРАТЬ для тестов
// Создать папку build 
// если есть, перейти в неё
// **** настроить путь к MinGW в CMakeList_exe.txt (Можно переименовать для удобства в CMakeList.txt)
// сделать make вот так  cmake -G "MinGW Makefiles" ..
// сделать mingw32-make получится экзешничег
// запустить ./JsonManager.exe

int main() {
    try {
        JsonSerializer serializer;

        // Тест 1: Парсинг JSON файла
        std::cout << "=== Тест 1: Парсинг JSON файла ===" << std::endl;
        JObject root = serializer.parseFile("Calibration_TAI.json");
        
        // Проверка порядка полей в корневом объекте
        std::cout << "\nПоля корневого объекта (порядок должен сохраниться):" << std::endl;
        auto rootFields = root.getAvailableFields();
        for (const auto& field : rootFields) {
            std::cout << "  - " << field << std::endl;
        }
        
        // Проверка конкретных значений
        std::cout << "\npassRza: " << root.getString("passRza") << std::endl;
        
        // Тест 2: Работа с вложенными объектами
        std::cout << "\n=== Тест 2: Работа с вложенными объектами ===" << std::endl;
        JObject& etaItr08 = root.getObject("ETAITR08");
        std::cout << "Поля ETAITR08:" << std::endl;
        auto etaFields = etaItr08.getAvailableFields();
        for (const auto& field : etaFields) {
            std::cout << "  - " << field << std::endl;
        }
        
        // Тест 3: Работа с таблицами (realizations)
        std::cout << "\n=== Тест 3: Работа с таблицами ===" << std::endl;
        JTable& realizationsTable = etaItr08.getTable("realizations");
        
        std::cout << "Колонки таблицы realizations:" << std::endl;
        auto columnNames = realizationsTable.columnNames();
        for (const auto& colName : columnNames) {
            std::cout << "  - " << colName << " (тип: " << static_cast<int>(realizationsTable.columnType(colName)) << ")" << std::endl;
        }
        
        std::cout << "\nСодержимое таблицы realizations:" << std::endl;
        size_t rowNum = 0;
        for (const auto& row : realizationsTable) {
            std::cout << "Строка " << rowNum++ << ": ";

            for (const auto& entry : row) {
                std::cout << entry.key << "=";
                switch (entry.value->typeIndex()) {
                    case IJNode::INT: std::cout << entry.value->getInt(); break;
                    case IJNode::FLOAT: std::cout << entry.value->getFloat(); break;
                    case IJNode::STRING: std::cout << "\"" << entry.value->getString() << "\""; break;
                    case IJNode::BOOL: std::cout << (entry.value->getBool() ? "true" : "false"); break;
                    default: std::cout << "[тип " << static_cast<int>(entry.value->typeIndex()) << "]";
                }
                std::cout << ", ";
            }
            std::cout << std::endl;
        }
        
        // Тест 4: Глубокий доступ к данным
        std::cout << "\n=== Тест 4: Глубокий доступ к данным ===" << std::endl;
        JObject& tai44 = etaItr08.getObject("tai44");
        JObject& tai44_U = tai44.getObject("tai44_U");
        
        std::cout << "tai44.caption: " << tai44.getString("caption") << std::endl;
        std::cout << "tai44_U.caption: " << tai44_U.getString("caption") << std::endl;
        std::cout << "tai44_U.cycles: " << tai44_U.getInt("cycles") << std::endl;
        std::cout << "tai44_U.suppFreq: " << tai44_U.getFloat("suppFreq") << std::endl;
        
        // Тест 5: Работа с таблицей каналов
        std::cout << "\n=== Тест 5: Работа с таблицей каналов ===" << std::endl;
        JTable& channelsTable = tai44_U.getTable("channels");
        
        std::cout << "Колонки таблицы channels:" << std::endl;
        auto channelColumns = channelsTable.columnNames();
        for (const auto& colName : channelColumns) {
            std::cout << "  - " << colName << std::endl;
        }
        
        std::cout << "\nПервые 3 строки таблицы channels:" << std::endl;
        for (size_t i = 0; i < std::min(size_t(3), channelsTable.size()); ++i) {
            std::cout << "Строка " << i << ": ";
            for (const auto& colName : channelColumns) {
                auto* value = channelsTable.at(colName, i);
                std::cout << colName << "=";
                switch (value->typeIndex()) {
                    case IJNode::INT: std::cout << value->getInt(); break;
                    case IJNode::FLOAT: std::cout << value->getFloat(); break;
                    case IJNode::STRING: std::cout << "\"" << value->getString() << "\""; break;
                    case IJNode::BOOL: std::cout << (value->getBool() ? "true" : "false"); break;
                    default: std::cout << "[тип " << static_cast<int>(value->typeIndex()) << "]";
                }
                std::cout << ", ";
            }
            std::cout << std::endl;
        }
        
        // Тест 6: Проверка порядка полей в глубоко вложенных объектах
        std::cout << "\n=== Тест 6: Проверка порядка полей ===" << std::endl;
        JObject& shunt1 = tai44_U.getObject("shunt_1");
        std::cout << "Поля shunt_1:" << std::endl;
        auto shuntFields = shunt1.getAvailableFields();
        for (const auto& field : shuntFields) {
            std::cout << "  - " << field << std::endl;
        }
        
        // Тест 7: Модификация данных
        std::cout << "\n=== Тест 7: Модификация данных ===" << std::endl;
        if (channelsTable.size() > 0) {
            auto* firstRowChNumber = channelsTable.at("chNumber", 0);
            if (firstRowChNumber->typeIndex() == IJNode::INT) {
                int oldValue = firstRowChNumber->getInt();
                firstRowChNumber->setInt(999); // Изменяем значение
                std::cout << "Изменен chNumber с " << oldValue << " на 999" << std::endl;
            }
        }
        
        // Тест 8: Сохранение в JSON
        std::cout << "\n=== Тест 8: Сохранение в JSON ===" << std::endl;
        Json modifiedJson = root.toJson();
        std::string newFilePath = "Calibration_TAI_modified.json";
        std::ofstream outFile(newFilePath);
        outFile << modifiedJson.dump(4); 
        outFile.close();
        std::cout << "Модифицированный JSON сохранен в: " << newFilePath << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        std::cout << "\nНажмите Enter для выхода...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return 1;
    }
    
    std::cout << "\nНажмите Enter для выхода...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return 0;
}