#include "jtable.h"

JTable::~JTable(){}

void JTable::pushBack(std::vector<std::pair<std::string, std::unique_ptr<IJBasic>>> row) {
    if (row.empty()) throw std::runtime_error("JTable: Empty row");
    
    if (rowCount_ == 0) {
        for (auto& [name, node] : row) {
            if (name.empty()) throw std::runtime_error("JTable: Column name cannot be empty");

            Column col;
            col.typeIndex = node->typeIndex();
            col.data.push_back(std::move(node));
            
            auto& seq_index = columns_.get<by_seq>();
            seq_index.emplace_back(ColumnEntry{name, std::move(col)});
        }
    } else {
        if (row.size() != columns_.size()) {
            throw std::runtime_error("JTable: Invalid number of columns");
        }
        
        auto& key_index = columns_.get<by_key>();
        
        for (auto& [name, value] : row) {
            auto it = key_index.find(name);
            
            if (it == key_index.end()) {
                throw std::runtime_error("JTable: Column '" + name + "' not found");
            }
            
            if (value->typeIndex() != it->column.typeIndex) {
                throw std::runtime_error("JTable: Type mismatch in column '" + name + "'");
            }
            
            key_index.modify(it, [&value](ColumnEntry& entry) {
                entry.column.data.push_back(std::move(value));
            });            
        }
    }
    rowCount_++;
}

IJBasic* JTable::at(const std::string& column_name, size_t row_index) {
    auto& key_index = columns_.get<by_key>();
    auto it = key_index.find(column_name);
    if (it == key_index.end()) {
        throw std::runtime_error("JTable: Column '" + column_name + "' not found");
    }
    
    if (row_index >= it->column.data.size()) {
        throw std::runtime_error("JTable: Row index out of range");
    }
    return it->column.data[row_index].get();
}

const IJBasic* JTable::at(const std::string& column_name, size_t row_index) const {
    auto& key_index = columns_.get<by_key>();
    auto it = key_index.find(column_name);
    if (it == key_index.end()) {
        throw std::runtime_error("JTable: Column '" + column_name + "' not found");
    }
    
    if (row_index >= it->column.data.size()) {
        throw std::runtime_error("JTable: Row index out of range");
    }
    return it->column.data[row_index].get();
}

std::optional<JTable::RowView> JTable::getRow(const std::string& key) const {
    auto& key_index = columns_.get<by_key>();
    auto col_it = key_index.find("keyVal");
    if (col_it == key_index.end()) {
        return std::nullopt;
    }

    if (!isTypeCompatible<std::string>(col_it->column.typeIndex)) {
        return std::nullopt;
    }

    const Column& col = col_it->column;
    for (size_t i = 0; i < col.data.size(); ++i) {
        if (isEqual<std::string>(col.data[i].get(), key)) {
            return *RowIterator(this, i);
        }
    }
    
    return std::nullopt;
}

std::optional<JTable::RowView> JTable::getRow(const int key) const {
    auto& key_index = columns_.get<by_key>();
    auto col_it = key_index.find("id");
    if (col_it == key_index.end()) {
        return std::nullopt;
    }

    if (!isTypeCompatible<int>(col_it->column.typeIndex)) {
        return std::nullopt;
    }

    const Column& col = col_it->column;
    for (size_t i = 0; i < col.data.size(); ++i) {
        if (isEqual<int>(col.data[i].get(), key)) {
            return *RowIterator(this, i);
        }
    }
    
    return std::nullopt;
}

std::vector<std::string> JTable::columnNames() const {
    std::vector<std::string> names;
    names.reserve(columns_.size());
    
    auto& seq_index = columns_.get<by_seq>();
    for (const auto& col_entry : seq_index) {
        names.push_back(col_entry.name);
    }
    return names;
}

bool JTable::hasColumn(const std::string& name) const {
    auto& key_index = columns_.get<by_key>();
    return key_index.find(name) != key_index.end();
}

IJNode::TypeIndex JTable::columnType(const std::string& name) const {
    auto& key_index = columns_.get<by_key>();
    auto it = key_index.find(name);
    if (it == key_index.end()) {
        throw std::runtime_error("JTable: Column '" + name + "' not found");
    }
    return it->column.typeIndex;
}

std::vector<const IJBasic*> JTable::getColumn(const std::string& name) const {
    auto& key_index = columns_.get<by_key>();
    auto it = key_index.find(name);
    if (it == key_index.end()) {
        throw std::runtime_error("Column '" + name + "' not found");
    }

    const Column& col = it->column;
    std::vector<const IJBasic*> result;
    result.reserve(col.data.size());

    for (const auto& item : col.data) {
        result.push_back(item.get());
    }

    return result;
}