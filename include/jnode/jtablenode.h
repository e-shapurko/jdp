#ifndef JTABLENODE_H
#define JTABLENODE_H

#include "ijobject.h"

class JTable;

class JTableNode : public IJObject 
{
public:
    explicit JTableNode(const std::string& key);
    ~JTableNode();
    JTable &getTable() override { return *table_; }
    void setTable(std::unique_ptr<JTable> table) override;

    IJNode::TypeIndex typeIndex() const override { return IJNode::TypeIndex::TABLE; }
    Json saveAsJson() const override;

private:
    std::unique_ptr<JTable> table_;
};

#endif