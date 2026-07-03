#include "jobjectnode.h"
#include "jobject.h"
#include <stdexcept>

JObjectNode::JObjectNode(const std::string& key) : 
    IJObject(key), 
    object_(std::make_unique<JObject>()) 
{}

JObjectNode::~JObjectNode(){}

void JObjectNode::setObject(std::unique_ptr<JObject> value)
{ 
    object_ = std::move(value); 
}

Json JObjectNode::saveAsJson() const {
    Json result;
    for (const auto& [key, node] : *object_) {
        Json nodeJson = node->saveAsJson();
        if (!nodeJson.empty()) {
            result[key] = nodeJson[key];
        }
    }
    return Json{{key_, result}};
}