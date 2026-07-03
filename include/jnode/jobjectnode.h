#ifndef JOBJECTNODE_H
#define JOBJECTNODE_H

#include "ijobject.h"

class JObject;

class JObjectNode : public IJObject 
{
public:
    explicit JObjectNode(const std::string& key);
    ~JObjectNode();

    JObject& getObject() override { return *object_; }
    void setObject(std::unique_ptr<JObject> value) override;

    IJNode::TypeIndex typeIndex() const override { return IJNode::TypeIndex::OBJECT; }
    Json saveAsJson() const override;

private:
    std::unique_ptr<JObject> object_;
};

#endif