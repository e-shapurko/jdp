#ifndef JBOOLNODE_H
#define JBOOLNODE_H

#include "ijbasic.h"

class JBoolNode : public IJBasic {
public:
    JBoolNode(const std::string& key, bool value = false) 
        : IJBasic(key), value_(value) {}

    bool getBool() const override { return value_; }
    void setBool(bool value) override { value_ = value; }

    IJNode::TypeIndex typeIndex() const override {return IJNode::TypeIndex::BOOL;}
    Json saveAsJson() const override {
        return Json{
            {key_, value_}
        };
    }

private:
    bool value_;
};
#endif