#ifndef JTABLE_H
#define JTABLE_H

#include <vector>
#include <utility>
#include <memory>
#include <stdexcept>
#include "ijnode.h"
#include "ijbasic.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

using namespace boost::multi_index;

class JTable {
public:
    
    struct Column {
        std::vector<std::unique_ptr<IJBasic>> data;
        IJNode::TypeIndex typeIndex;
    };

    struct ColumnEntry {
        std::string name;
        Column column;
        
        ColumnEntry(std::string n, Column c) 
            : name(std::move(n)), column(std::move(c)) {}
    };

    struct RowEntry {
        std::string key;
        const IJBasic* value;
        
        RowEntry(std::string k, const IJBasic* v) 
            : key(std::move(k)), value(v) {}
    };    


    struct by_seq {};
    struct by_key {};

    using ColumnContainer = multi_index_container<
        ColumnEntry,
        indexed_by<
            sequenced<tag<by_seq>>,  
            ordered_unique<
                tag<by_key>,
                member<ColumnEntry, std::string, &ColumnEntry::name>
            >
        >
    >;

    using RowContainer = multi_index_container<
        RowEntry,
        indexed_by<
            sequenced<tag<by_seq>>,
            ordered_unique<
                tag<by_key>,
                member<RowEntry, std::string, &RowEntry::key>
            >
        >
    >;    

    class RowView {
    private:
        RowContainer container_;
        
    public:
        RowView(RowContainer&& container) : container_(std::move(container)) {}
        
        const IJBasic* at(const std::string& key) const {
            auto& key_index = container_.get<by_key>();
            auto it = key_index.find(key);
            if (it == key_index.end()) {
                throw std::runtime_error("RowView: Key '" + key + "' not found");
            }
            return it->value;
        }

        std::optional<const IJBasic*> column(const std::string& columnName) const {
            auto& key_index = container_.get<by_key>();
            auto it = key_index.find(columnName);
            return (it == key_index.end()) ? std::nullopt : std::optional<const IJBasic*>(it->value);
        }

        auto begin() const { return container_.get<by_seq>().begin(); }
        auto end() const { return container_.get<by_seq>().end(); }

        size_t size() const { return container_.size(); }
        bool empty() const { return container_.empty(); }
    };
    

    JTable() = default;
    ~JTable();

    template<typename T>
    bool contain(const std::string& column_name, const T& key) const {
        auto& key_index = columns_.get<by_key>();
        auto it = key_index.find(column_name);
        if (it == key_index.end()) {
            throw std::runtime_error("JTable: Column '" + column_name + "' not found");
        }
        
        if (!isTypeCompatible<T>(it->column.typeIndex)) {
            throw std::runtime_error("JTable: Type mismatch in column '" + column_name + "'");
        }
        
        const Column& col = it->column;
        for (const auto& item : col.data) {
            if (isEqual<T>(item.get(), key)) {
                return true;
            }
        }
        return false;
    }

    std::optional<RowView> getRow(const std::string& key) const;
    std::optional<RowView> getRow(const int key) const;

    void pushBack(std::vector<std::pair<std::string, std::unique_ptr<IJBasic>>> row);

    IJBasic* at(const std::string& column_name, size_t row_index);
    const IJBasic* at(const std::string& column_name, size_t row_index) const;

    size_t size() const { return rowCount_; }
    size_t columnCount() const { return columns_.size(); }
    std::vector<std::string> columnNames() const;
    bool hasColumn(const std::string& name) const;
    IJNode::TypeIndex columnType(const std::string& name) const;
    std::vector<const IJBasic*> getColumn(const std::string& name) const;

    class RowIterator {
        const JTable* table_;
        size_t current_row_;
    public:
        RowIterator(const JTable* table, size_t row) 
            : table_(table), current_row_(row) {}

        RowView operator*() const {
            RowContainer row;
            auto& seq_index = table_->columns_.get<by_seq>();
            for (const auto& col_entry : seq_index) {
                if (current_row_ < col_entry.column.data.size()) {
                    row.emplace_back(std::string(col_entry.name), col_entry.column.data[current_row_].get());
                }
            }
            return RowView(std::move(row));
        }
        
        RowIterator& operator++() {
            current_row_++;
            return *this;
        }
        
        bool operator!=(const RowIterator& other) const {
            return current_row_ != other.current_row_;
        }
    };

    RowIterator begin() const { return RowIterator(this, 0); }
    RowIterator end() const { return RowIterator(this, rowCount_); }

private:
    template<typename T>
    static bool isTypeCompatible(IJNode::TypeIndex typeIndex) {
        if constexpr (std::is_same_v<T, int>) {
            return typeIndex == IJNode::INT;
        } else if constexpr (std::is_same_v<T, float>) {
            return typeIndex == IJNode::FLOAT;
        } else if constexpr (std::is_same_v<T, double>) {
            return typeIndex == IJNode::FLOAT;
        } else if constexpr (std::is_same_v<T, std::string>) {
            return typeIndex == IJNode::STRING;
        } else if constexpr (std::is_same_v<T, bool>) {
            return typeIndex == IJNode::BOOL;
        } else {
            return false;
        }
    }

    template<typename T>
    static bool isEqual(const IJBasic* item, const T& value) {
        if constexpr (std::is_same_v<T, int>) {
            return item->getInt() == value;
        } else if constexpr (std::is_same_v<T, float>) {
            return item->getFloat() == value;
        } else if constexpr (std::is_same_v<T, double>) {
            return item->getDouble() == value;
        } else if constexpr (std::is_same_v<T, std::string>) {
            return item->getString() == value;
        } else if constexpr (std::is_same_v<T, bool>) {
            return item->getBool() == value;
        }
        return false;
    }

    ColumnContainer columns_;
    size_t rowCount_ = 0;
};

#endif