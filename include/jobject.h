#ifndef JOBJECT_H
#define JOBJECT_H

#include <map>
#include <memory>
#include <string>
#include "ijbasic.h"
#include "ijobject.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

using boost::multi_index_container;
using namespace boost::multi_index;

class JTable;

struct NodeEntry {
    std::string key;
    std::unique_ptr<IJNode> node;
    
    NodeEntry(std::string k, std::unique_ptr<IJNode> n) 
        : key(std::move(k)), node(std::move(n)) {}
};

struct by_seq{};
struct by_key{};

using NodeContainer = multi_index_container<
    NodeEntry,
    indexed_by<
        sequenced<tag<by_seq>>,
        ordered_unique<
            tag<by_key>,
            member<NodeEntry, std::string, &NodeEntry::key>
        >
    >
>;

class JObject {
public:

    int getInt(const std::string& key) const;
    float getFloat(const std::string& key) const;
    double getDouble(const std::string& key) const;
    std::string getString(const std::string& key) const;
    bool getBool(const std::string& key) const;
    JObject& getObject(const std::string& key);
    JTable& getTable(const std::string& key);
    bool hasKey(const std::string& key) const;

    bool isValid() const noexcept { 
        return !data_.empty(); 
    }

    std::vector<std::string> getAvailableFields() const;
    std::string getAvailableFieldsStr() const;
    //чтоб работать как с обычной мапой.
    void emplace(const std::string& key, std::unique_ptr<IJNode> node) {
        auto& seq_index = data_.get<by_seq>();
        seq_index.emplace_back(NodeEntry{key, std::move(node)});
    }
    
    void insert(std::string key, std::unique_ptr<IJNode> node) {
        auto& seq_index = data_.get<by_seq>();
        seq_index.emplace_back(NodeEntry{std::move(key), std::move(node)});
    }
    auto begin() noexcept { return data_.get<by_seq>().begin(); }
    auto end() noexcept { return data_.get<by_seq>().end(); }
    auto begin() const noexcept { return data_.get<by_seq>().begin(); }
    auto end() const noexcept { return data_.get<by_seq>().end(); }

    bool empty() const noexcept { return data_.empty(); }
    size_t size() const noexcept { return data_.size(); }

    Json toJson() const;

private:
    NodeContainer data_;
    IJNode* getNode(const std::string& key) const;
};

#endif