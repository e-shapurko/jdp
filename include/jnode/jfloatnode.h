#ifndef JFLOATNODE_H
#define JFLOATNODE_H

#include "ijbasic.h"

class JFloatNode : public IJBasic {
public:
    JFloatNode(const std::string& key, double value = 0.0) 
        : IJBasic(key), value_(value) {}

    float getFloat() const override { return value_; }
    double getDouble() const override { return value_; }
    void setFloat(float value) override { value_ = value; }
    void setDouble(double value) override { value_ = value; }

    IJNode::TypeIndex typeIndex() const override {return IJNode::TypeIndex::FLOAT;}
    Json saveAsJson() const override {
        
        return Json{
            {key_, value_}
        };
    }

private:
    double value_;
};
#endif