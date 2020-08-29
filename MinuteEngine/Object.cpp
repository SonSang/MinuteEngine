/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#include "Object.h"

namespace ME {
    Object::Ptr Object::createPtr(const std::string& name) {
        Object object;
        object.setName(name);
        return std::make_shared<Object>(object);
    }
    void Object::setName(const std::string& name) {
        this->name = name;
    }
    const std::string& Object::getName() const noexcept {
        return name;
    }
    void Object::setTransform(const Transform& transform) noexcept {
        this->transform = transform;
    }
    Transform& Object::getTransform() noexcept {
        return transform;
    }
    const Transform& Object::getTransformC() const noexcept {
        return transform;
    }
    void Object::translate(const glm::vec3& v) {
        transform.translate(v);
    }
    void Object::rotate(const glm::vec3& axis, float angle) {
        transform.rotate(axis, angle);
    }
    int Object::propertySize() const noexcept {
        return (int)properties.size();
    }
    bool Object::addProperty(const Property::Ptr& property) {
        if (property->getOwner() != nullptr)
            return false;
        properties.insert({ nID++, property });
        property->setOwner(this);
        return true;
    }
    bool Object::delProperty(int id) {
        auto find = properties.find(id);
        if (find == properties.end())
            return false;
        properties.erase(find);
        return true;
    }
    bool Object::delProperty(const std::string& name) {
        std::vector<int> delProps;
        for (auto& prop : getProperties()) {
            if (prop.second->getName() == name)
                delProps.push_back(prop.first);
        }
        if (delProps.empty())
            return false;
        for (auto& id : delProps) {
            getProperty(id)->getRender()->destroy();
            delProperty(id);
        }
        return true;
    }
    Property::Ptr Object::getProperty(int id) {
        auto find = properties.find(id);
        if (find == properties.end())
            return nullptr;
        else
            return find->second;
    }
    Object::PropList& Object::getProperties() noexcept {
        return properties;
    }
    const Object::PropList& Object::getPropertiesC() const noexcept {
        return properties;
    }
    void Object::update(double deltaTime) {
        for (auto prop : properties)
            prop.second->update(deltaTime);
    }
}