/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#include "Property.h"

namespace ME {
    Property::Property(const Render::Ptr& render) {
        this->render = render;
    }
    Property::Ptr Property::createPtr(const Render::Ptr& render) {
        Property p(render);
        return std::make_shared<Property>(p);
    }
    void Property::setOwner(Object* owner) {
        this->owner = owner;
    }
    Object* Property::getOwner() noexcept {
        return owner;
    }
    void Property::setName(const std::string& name) {
        this->name = name;
    }
    const std::string& Property::getName() const noexcept {
        return name;
    }
    void Property::setRender(const Render::Ptr& render, bool destroyPrev) {
        if (destroyPrev && this->render != nullptr)
            this->render->destroy();
        this->render = render;
    }
    Render::Ptr Property::getRender() noexcept {
        return render;
    }
    const Render::Ptr Property::getRenderC() const noexcept {
        return render;
    }
    void Property::update(double deltaTime) {
        return;
    }
    void Property::drawUI(bool update) {
        return;
    }
}