/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#ifndef __ME_SCENE_H__
#define __ME_SCENE_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "Utils.h"
#include "Object.h"
#include "Light.h"
#include "Texture.h"

namespace ME {
	class Scene {
    public:
        // This class represents a node in the scene graph.
        class Node {
        public:
            using Child = std::set<int>;
        private:
            Object::Ptr     object;
            int             parent = -1;
            Child           child;
        public:
            void setObject(const Object::Ptr& object);
            Object::Ptr& getObject() noexcept;
            const Object::Ptr& getObjectC() const noexcept;

            void addChild(int child) noexcept;
            void delChild(int child) noexcept;
            Child& getChild() noexcept;
            const Child& getChildC() const noexcept;

            void setParent(int parent) noexcept;
            int getParent() const noexcept;
        };
        struct Skybox {
            QuadRender::Ptr cube;
            TextureCube texture;
            bool valid = false;

            inline static Skybox create(const std::string path[6]) {
                Skybox skybox;
                Render::Vertex min, max;
                min.position = { -1, -1, -1 };
                max.position = { 1, 1, 1 };
                skybox.cube = QuadRender::createCubePtr(min, max);
                skybox.texture = TextureCube::create(path);
                return skybox;
            }
        };
        using Graph = std::map<int, Node>;
    private:
        Graph graph;
        int nID = 0;
        int root = -1;
        LightManager lightManager;
        Skybox skybox;
        Scene() = default;
    public:
        static Scene create();
        void destroy();

        Graph& getGraph() noexcept;
        const Graph& getGraphC() const noexcept;

        inline int getRoot() const noexcept {
            return root;
        }

        LightManager& getLightManager() noexcept;
        const LightManager& getLightManagerC() const noexcept;

        Skybox& getSkybox() noexcept;
        const Skybox& getSkyboxC() const noexcept;

        Node& getNode(int id);
        const Node& getNodeC(int id) const;
        void delNode(int id);
        void connect(int childID, int parentID);
        
        // @parent : Parent of newly inserted object. If -1, root node is set.
        int addObject(const Object::Ptr& object, int parent = -1);
        void addObjectProperty(int id, const std::shared_ptr<Property>& property);

        void delObjectProperty(const Object::Ptr& object, int propertyID);
        void delObjectProperty(int objectID, int propertyID);
        void delObjectProperties(int objectID);

        // For all the [ Render ]s in this scene, check if any of them has same memory as given arguments.
        // If not, we can safely destroy those memories.
        bool checkDuplicateMemory(uint vao, uint vbo, uint ebo);

        void update(double deltaTime);
	};
}

#endif