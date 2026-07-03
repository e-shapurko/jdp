#include "jtablenode.h"
#include "jtable.h"

JTableNode::JTableNode(const std::string& key) : 
    IJObject(key),
    table_(std::make_unique<JTable>()) 
{}

JTableNode::~JTableNode() = default;

void JTableNode::setTable(std::unique_ptr<JTable> table)
{
    table_ = std::move(table); 
}

Json JTableNode::saveAsJson() const 
{
    Json jarray = Json::array();
    
    for (const auto& row : *table_) {
        Json jrow;
        for (const auto& entry : row) {
            const auto& value = entry.value;
            switch (value->typeIndex()) {
                case IJNode::INT: jrow[entry.key] = value->getInt(); break;
                case IJNode::FLOAT: jrow[entry.key] = value->getDouble(); break;
                case IJNode::STRING: jrow[entry.key] = value->getString(); break;
                case IJNode::BOOL: jrow[entry.key] = value->getBool(); break;
                default: throw std::runtime_error("Unsupported type in table");
            }
        }
        jarray.push_back(jrow);
    }
    
    return Json{
        {key_, jarray}
    };  
}