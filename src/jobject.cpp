#include "jobject.h"
#include <stdexcept>

int JObject::getInt(const std::string& key) const 
{
    auto* basic = dynamic_cast<IJBasic*>(getNode(key));
    if (!basic) throw std::runtime_error("Key '" + key + "' is not a basic type");
    return basic->getInt();
}

float JObject::getFloat(const std::string& key) const 
{
    auto* basic = dynamic_cast<IJBasic*>(getNode(key));
    if (!basic) throw std::runtime_error("Key '" + key + "' is not a basic type");
    return basic->getFloat();
}

double JObject::getDouble(const std::string& key) const 
{
    auto* basic = dynamic_cast<IJBasic*>(getNode(key));
    if (!basic) throw std::runtime_error("Key '" + key + "' is not a basic type");
    return basic->getDouble();
}

std::string JObject::getString(const std::string& key) const 
{
    auto* basic = dynamic_cast<IJBasic*>(getNode(key));
    if (!basic) throw std::runtime_error("Key '" + key + "' is not a basic type");
    return basic->getString();
}

bool JObject::getBool(const std::string& key) const 
{
    auto* basic = dynamic_cast<IJBasic*>(getNode(key));
    if (!basic) throw std::runtime_error("Key '" + key + "' is not a basic type");
    return basic->getBool();
}

JTable& JObject::getTable(const std::string& key) 
{
    auto* table = dynamic_cast<IJObject*>(getNode(key));
    if (!table) throw std::runtime_error("Key '" + key + "' is not a object");
    return table->getTable();
}

JObject& JObject::getObject(const std::string& key) 
{
    auto* objNode = dynamic_cast<IJObject*>(getNode(key));
    if (!objNode) throw std::runtime_error("Key '" + key + "' is not a object");
    return objNode->getObject();
}

bool JObject::hasKey(const std::string& key) const 
{
    auto& key_index = data_.get<by_key>();
    return key_index.find(key) != key_index.end();
}

IJNode* JObject::getNode(const std::string& key) const 
{
    auto& key_index = data_.get<by_key>();
    auto it = key_index.find(key);
    if (it == key_index.end()) {
        throw std::runtime_error("Key '" + key + "' not found");
    }
    return it->node.get();
}

std::vector<std::string> JObject::getAvailableFields() const {
    std::vector<std::string> fields;
    fields.reserve(data_.size());
    
    for (const auto& entry : data_.get<by_seq>()) {
        fields.push_back(entry.key);
    }
    
    return fields;
}

std::string JObject::getAvailableFieldsStr() const {
    if (data_.empty()) {
        return "";
    }
    
    std::string result;
    for (const auto& entry : data_.get<by_seq>()) {
        if (!result.empty()) {
            result += ", ";
        }
        result += entry.key;
    }
    
    return result;
}


Json JObject::toJson() const
{
    Json result;
    for (const auto& entry : data_.get<by_seq>()) {
        result[entry.key] = entry.node->saveAsJson().at(entry.key);
    }
    return result;
}
