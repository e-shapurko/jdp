#ifndef JSERIALIZER_H
#define JSERIALIZER_H

#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <fstream> 
#include <nlohmann/json.hpp>
#include "jtypes.h"
#include "jobject.h"
#include "jtable.h"


using Json = nlohmann::ordered_json;


class JsonSerializer 
{
public:
	JsonSerializer() = default;
	JObject parseString(const std::string& jsonStr);
	JObject parseFile(const std::string& jsonPath);
private:
	void parseToObject(const Json& doc, JObject& mapNode);
	void parseToTable(const Json &doc, JTable &table);
};


#endif
