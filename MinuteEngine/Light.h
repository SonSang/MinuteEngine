/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#ifndef __ME_LIGHT_H__
#define __ME_LIGHT_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "Color.h"
#include "Shadow.h"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <string>
#include <memory>
#include <vector>

namespace ME {
    class Light {
    protected:
        std::string name = "Light";
        bool valid = true;

        int type;       // 0 : Directional Light
                        // 1 : Point Light
        glm::vec3 position;
        glm::vec3 direction;    // Even directional light needs position vector to make shadow!
        glm::vec3 up;
        Color ambient = Color::black();
        Color diffuse = Color::white();
        Color specular = Color::white();
    public:
        using Ptr = std::shared_ptr<Light>;
        Shadow shadow;

        inline void setName(const std::string& name) {
            this->name = name;
        }
        inline std::string getName() const noexcept {
            return name;
        }

        inline void setType(int type) {
            this->type = type;
        }
        inline void setValid(bool valid) {
            this->valid = valid;
        }
        inline void setPosition(const glm::vec3& position) {
            this->position = position;
        }
        inline void setDirection(const glm::vec3& direction) {
            this->direction = direction;
        }
        inline void setUp(const glm::vec3& up) {
            this->up = up;
        }
        inline void setAmbient(const Color& v) {
            ambient = v;
        }
        void setDiffuse(const Color& v) {
            diffuse = v;
        }
        void setSpecular(const Color& v) {
            specular = v;
        }

        inline int getType() const noexcept {
            return type;
        }
        inline bool getValid() const noexcept {
            return valid;
        }
        inline glm::vec3 getPosition() const noexcept {
            return position;
        }
        inline glm::vec3 getDirection() const noexcept {
            return direction;
        }
        inline glm::vec3 getUp() const noexcept {
            return up;
        }
        inline Color getAmbient() const noexcept {
            return ambient;
        }
        inline Color getDiffuse() const noexcept {
            return diffuse;
        }
        inline Color getSpecular() const noexcept {
            return specular;
        }

        inline glm::mat4 getViewMat() const noexcept {
            return glm::lookAt(position, position + direction, up);
        }
    };

    class LightManager {
    public:
        using Ptr = std::shared_ptr<LightManager>;
        std::vector<Light> lights;

        inline void addLight(const Light light) {
            lights.push_back(light);
        }
        inline void delLight(int index) {
            auto num = lights.size();
            if (num <= index)
                throw("Index overflow");
            else
                lights.erase(lights.begin() + index);
        }
        inline void delLight(const std::string& name) {
            auto num = lights.size();
            while (true) {
                bool find = false;
                int idx;
                for (int i = 0; i < num; i++) {
                    const auto& light = lights[i];
                    if (light.getName() == name) {
                        find = true;
                        idx = i;
                        break;
                    }
                }
                if (find)
                    lights.erase(lights.begin() + idx);
                else
                    break;
            }
        }
    };
}

#endif