#include "jserializer.h"


JObject JsonSerializer::parseString(const std::string& jsonStr)
{
    auto j = Json::parse(jsonStr);
    JObject result;
    parseToObject(j, result);
    return result;
}

JObject JsonSerializer::parseFile(const std::string& jsonPath)
{
    std::ifstream file(jsonPath);
    if (!file.is_open()) {
        throw std::runtime_error("File not found: " + jsonPath);
    }
    
    Json j;
    file >> j;
    
    JObject result;
    parseToObject(j, result);
    return result;
}

void JsonSerializer::parseToObject(const Json& doc, JObject& mapNode) {
    for (auto& [key, value] : doc.items()) {
        if (value.is_object()) {
            auto node = std::make_unique<JObjectNode>(key);
            parseToObject(value, node->getObject());
            mapNode.emplace(key, std::move(node));
        }
        else if (value.is_array()) {
            bool isArrayOfObjects = !value.empty() && value[0].is_object();
            if (isArrayOfObjects) {
                auto tableNode = std::make_unique<JTableNode>(key);
                parseToTable(value, tableNode->getTable());
                mapNode.emplace(key, std::move(tableNode));
            }
        }
        else if (value.is_number_integer()) {
            mapNode.emplace(key, std::make_unique<JIntNode>(key, value.get<int>()));
        }
        else if (value.is_number_float()) {
            mapNode.emplace(key, std::make_unique<JFloatNode>(key, value.get<double>()));
        }
        else if (value.is_string()) {
            mapNode.emplace(key, std::make_unique<JStringNode>(key, value.get<std::string>()));
        }
        else if (value.is_boolean()) {
            mapNode.emplace(key, std::make_unique<JBoolNode>(key, value.get<bool>()));
        }
    }
}


void JsonSerializer::parseToTable(const Json &doc, JTable &table) 
{
    for (const auto& item : doc) {
        std::vector<std::pair<std::string, std::unique_ptr<IJBasic>>> row;
        
        for (auto& [key, value] : item.items()) {
            if (value.is_number_integer()) {
                row.emplace_back(key, std::make_unique<JIntNode>(key, value.get<int>()));
            }
            else if (value.is_number_float()) {
                row.emplace_back(key, std::make_unique<JFloatNode>(key, value.get<double>()));
            }
            else if (value.is_string()) {
                row.emplace_back(key, std::make_unique<JStringNode>(key, value.get<std::string>()));
            }
            else if (value.is_boolean()) {
                row.emplace_back(key, std::make_unique<JBoolNode>(key, value.get<bool>()));
            }
            else {
                throw std::runtime_error("Unsupported type in table for key: " + key);
            }
        }
        
        if (!row.empty()) {
            table.pushBack(std::move(row));
        }
    }
}
