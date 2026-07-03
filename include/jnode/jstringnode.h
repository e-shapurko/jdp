#ifndef JSTRINGNODE_H
#define JSTRINGNODE_H

#include "ijbasic.h"
#include <string>

class JStringNode : public IJBasic {
public:
    JStringNode(const std::string& key, const std::string& value = "") 
        : IJBasic(key), value_(value) {}

    std::string getString() const override { return value_; }
    void setString(const std::string& value) override { value_ = value; }

    IJNode::TypeIndex typeIndex() const override {return IJNode::TypeIndex::STRING;}
    Json saveAsJson() const override {
        return Json{
            {key_, value_}
        };
    }

private:
    std::string value_;
};
#endif