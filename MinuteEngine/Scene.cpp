/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#include "Scene.h"

namespace ME {
    // Scene::Node
    void Scene::Node::setObject(const Object::Ptr& object) {
        this->object = object;
    }
    Object::Ptr& Scene::Node::getObject() noexcept {
        return object;
    }
    const Object::Ptr& Scene::Node::getObjectC() const noexcept {
        return object;
    }
    void Scene::Node::addChild(int child) noexcept {
        this->child.insert(child);
    }
    void Scene::Node::delChild(int child) noexcept {
        this->child.erase(child);
    }
    Scene::Node::Child& Scene::Node::getChild() noexcept {
        return child;
    }
    const Scene::Node::Child& Scene::Node::getChildC() const noexcept {
        return child;
    }
    void Scene::Node::setParent(int parent) noexcept {
        this->parent = parent;
    }
    int Scene::Node::getParent() const noexcept {
        return parent;
    }

    // Scene
    Scene Scene::create() {
        Scene scene;
        Node rootNode;
        Object::Ptr emptyObject = Object::createPtr("EmptyObject");

        rootNode.setObject(emptyObject);
        scene.graph.insert({ 0, rootNode });
        scene.root = scene.nID++;
        return scene;
    }
    void Scene::destroy() {
        // We do not have to care about memory duplicity, because everything will be deleted.
        for (auto node : graph) {
            Object::Ptr& object = node.second.getObject();
            auto& propList = object->getProperties();
            for (auto prop : propList) {
                auto render = prop.second->getRender();
                render->destroy();
            }
        }
        graph.clear();
    }
    Scene::Graph& Scene::getGraph() noexcept {
        return graph;
    }
    const Scene::Graph& Scene::getGraphC() const noexcept {
        return graph;
    }
    LightManager& Scene::getLightManager() noexcept {
        return lightManager;
    }
    const LightManager& Scene::getLightManagerC() const noexcept {
        return lightManager;
    }
    Scene::Skybox& Scene::getSkybox() noexcept {
        return skybox;
    }
    const Scene::Skybox& Scene::getSkyboxC() const noexcept {
        return skybox;
    }

    Scene::Node& Scene::getNode(int id) {
        return graph.at(id);
    }
    const Scene::Node& Scene::getNodeC(int id) const {
        return graph.at(id);
    }
    void Scene::delNode(int id) {
        Node& node = graph.at(id);
        auto parentID = node.getParent();
        auto childID = node.getChild();

        // Delete from parent
        Node& parentNode = graph.at(parentID);
        parentNode.delChild(id);
        for (auto cid : childID) 
            parentNode.addChild(cid);

        // Delete from child
        for (auto cid : childID) {
            Node& childNode = graph.at(cid);
            childNode.setParent(parentID);
        }

        // Delete from graph
        delObjectProperties(id);
        graph.erase(id);
    }
    void Scene::connect(int childID, int parentID) {
        Node& node = graph.at(childID);
        if (node.getParent() != -1) {
            Node& prevParentNode = graph.at(node.getParent());
            prevParentNode.delChild(childID);
        }
        node.setParent(parentID);
        Node& parentNode = graph.at(parentID);
        parentNode.addChild(childID);
    }
    int Scene::addObject(const Object::Ptr& object, int parent) {
        Node node;
        int id = nID;
        node.setObject(object);
        graph.insert({ nID++, node });

        if (parent == -1)
            connect(id, root);
        else
            connect(id, parent);

        return id;
    }
    void Scene::addObjectProperty(int id, const std::shared_ptr<Property>& property) {
        Object::Ptr& object = graph.at(id).getObject();
        object->addProperty(property);
    }

    void Scene::delObjectProperty(const Object::Ptr& object, int propertyID) {
        auto prop = object->getProperty(propertyID);
        auto render = prop->getRender();
        bool memDuplicate = checkDuplicateMemory(render->getVAO(), render->getVBO(), render->getEBO());
        if (!memDuplicate)
            render->destroy();
        object->delProperty(propertyID);
    }
    void Scene::delObjectProperty(int objectID, int propertyID) {
        auto& node = graph.at(objectID);
        auto& object = node.getObject();
        delObjectProperty(object, propertyID);
    }
    void Scene::delObjectProperties(int objectID) {
        auto& node = graph.at(objectID);
        auto& object = node.getObject();
        for (auto prop : object->getProperties()) 
            delObjectProperty(object, prop.first);
    }
    
    bool Scene::checkDuplicateMemory(uint vao, uint vbo, uint ebo) {
        for (auto node : graph) {
            auto& props = node.second.getObjectC()->getPropertiesC();
            for (auto prop : props) {
                auto renderPtr = prop.second->getRenderC();
                if (renderPtr->getVAO() == vao)
                    return true;
                if (renderPtr->getVBO() == vbo)
                    return true;
                if (renderPtr->getEBO() == ebo)
                    return true;
            }
        }
        return false;
    }
    void Scene::update(double deltaTime) {
        for (auto node : graph) {
            auto& object = node.second.getObject();
            object->update(deltaTime);
        }
    }
}