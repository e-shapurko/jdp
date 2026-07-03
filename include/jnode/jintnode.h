#ifndef JINTNODE_H
#define JINTNODE_H

#include "ijbasic.h"

class JIntNode : public IJBasic {
public:
    JIntNode(const std::string& key, int value = 0) 
        : IJBasic(key), value_(value) {}

    int getInt() const override { return value_; }
    void setInt(int value) override { value_ = value; }

    IJNode::TypeIndex typeIndex() const override {return IJNode::TypeIndex::INT;}
    Json saveAsJson() const override {
        return Json{
            {key_, value_}
        };
    }

private:
    int value_;
};
#endif