/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#ifndef __ME_OBJECT_H__
#define __ME_OBJECT_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "Property.h"
#include "Transform.h"
#include "Utils.h"

#include <list>
#include <memory>
#include <set>
#include <map>

namespace ME {
    // This class represents objects that can be "placed" in the scene.
    // Each object can have their own properties and work with them.
    class Object {
    public:
        using PropList = std::map<int, Property::Ptr>;
    private:
        std::string		name = "Object";
        int             nID = 0;        // ID for the next property to be added.
        Transform       transform;
        PropList        properties;

        Object() = default;
    public:
        using Ptr = std::shared_ptr<Object>;

        static Ptr createPtr(const std::string& name);

        void setName(const std::string& name);
        const std::string& getName() const noexcept;
        
        void setTransform(const Transform& transform) noexcept;
        Transform& getTransform() noexcept;
        const Transform& getTransformC() const noexcept;

        void translate(const glm::vec3& v);
        void rotate(const glm::vec3& axis, float angle);

        int propertySize() const noexcept;
        bool addProperty(const Property::Ptr& property);
        bool delProperty(int id);
        bool delProperty(const std::string& name);      // @WARNING : Destroys all rendering info
        Property::Ptr getProperty(int id);
        PropList& getProperties() noexcept;
        const PropList& getPropertiesC() const noexcept;

        // Get properties with type <T> at runtime.
        template<class T>
        std::set<std::shared_ptr<T>> getProperty() noexcept {
            std::set<std::shared_ptr<T>>
                ret;
            for (auto it = properties.begin(); it != properties.end(); it++) {
                if (typeid(*(it->second)) == typeid(T))
                    ret.insert(std::dynamic_pointer_cast<T>(*it));
            }
            return ret;
        }
        template<class T>
        std::set<std::shared_ptr<T>> getDerivedProperty() noexcept {
            std::set<std::shared_ptr<T>>
                ret;
            for (auto it = properties.begin(); it != properties.end(); it++) {
                auto
                    cast = std::dynamic_pointer_cast<T>(*it);
                if (cast != nullptr)
                    ret.insert(cast);
            }
            return ret;
        }
        template<class T>
        void delProperty() {
            auto
                propList = getProperty<T>();
            for (auto it = propList.begin(); it != propList.end(); it++) {
                delProperty((*it)->getID());
            }
        }

        void update(double deltaTime);
    };
}
#endif