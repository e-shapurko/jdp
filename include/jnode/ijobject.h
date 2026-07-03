#ifndef IJOBJECT_H
#define IJOBJECT_H

#include "ijnode.h"

class JTable;
class JObject;

class IJObject: public IJNode
{
public:
    IJObject(const std::string &key):IJNode(key){}
    virtual ~IJObject() = default;

    virtual JTable &getTable() { throw std::runtime_error("This is not Table type"); }
    virtual JObject &getObject() { throw std::runtime_error("This is not Object type"); }

    virtual void setTable(std::unique_ptr<JTable>) { throw std::runtime_error("Can not set Table value"); }
    virtual void setObject(std::unique_ptr<JObject>) { throw std::runtime_error("Can not set Object value"); }

    virtual Json saveAsJson() const override = 0;
    virtual IJNode::TypeIndex typeIndex() const override = 0;    
};

#endif
