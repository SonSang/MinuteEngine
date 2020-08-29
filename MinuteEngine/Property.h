/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#ifndef __ME_PROPERTY_H__
#define __ME_PROPERTY_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "Render.h"
#include <string>
#include <vector>

namespace ME {
    class Object;       // Declaration.

    // This class is the base class of all kinds of properties an object can possess.
    // User can use this property to give abilities to the object.
    // A property can hold only single [ Render ].
    class Property {	
    private:
        Object*     owner = nullptr;
        std::string name = "Property";
        Render::Ptr render = nullptr;
    protected:
        Property(const Render::Ptr& render = nullptr);
    public:
        using Ptr = std::shared_ptr<Property>;

        static Ptr createPtr(const Render::Ptr& render = nullptr);

        void setOwner(Object* owner);
        Object* getOwner() noexcept;

        void setName(const std::string& name);
        const std::string& getName() const noexcept;

        void setRender(const Render::Ptr& render, bool destroyPrev = false);
        Render::Ptr getRender() noexcept;
        const Render::Ptr getRenderC() const noexcept;

        virtual void update(double deltaTime);   // @deltaTime : Given in seconds(sec).
        virtual void drawUI(bool update);
    };
}
#endif