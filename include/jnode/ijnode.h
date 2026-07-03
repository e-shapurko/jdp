#ifndef IJNODE_H
#define IJNODE_H

#include <string>
#include <stdexcept>
#include <nlohmann/json.hpp>

using Json = nlohmann::ordered_json;

class IJNode 
{
public:
	enum TypeIndex {
        INT = 0,
        FLOAT = 1,
        STRING = 2,
        BOOL = 3,
        TABLE = 4,
        OBJECT = 5,
        UNKNOWN = 255
    };	

	IJNode(const std::string &key):key_(key){}
    virtual ~IJNode() = default;
    virtual TypeIndex typeIndex() const = 0;
	virtual Json saveAsJson() const = 0;
protected:
	std::string key_;    
};

#endif