/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#ifndef __ME_SHADER_H__
#define __ME_SHADER_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "Utils.h"
#include "Light.h"
#include "Material.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <string>

namespace ME {
    class Shader {
    protected:
        uint program;
        uint vertShader;
        uint fragShader;
    public:
        static Shader create(const std::string& vpath, const std::string& fpath);
        static void destroy(Shader& shader);

        static uint createVertShader(const std::string& path);
        static uint createFragShader(const std::string& path);
        static uint createProgram(uint vert, uint frag);

        uint getProgram() const noexcept;
        uint getVertShader() const noexcept;
        uint getFragShader() const noexcept;

        // Shader variable related functions.
        int getAttrLoc(const std::string& var) const;
        int getUnifLoc(const std::string& var) const;
        bool setUnifInt(const std::string& var, int i) const;
        bool setUnifFloat(const std::string& var, float f) const;
        bool setUnifVec3(const std::string& var, const glm::vec3& vec) const;   // Return true if succeeded.
        bool setUnifVec4(const std::string& var, const glm::vec4& vec) const;   // Return true if succeeded.
        bool setUnifMat4(const std::string& var, const glm::mat4& mat) const;
        bool setUnifBool(const std::string& var, bool b) const;

        void enable() const;
        static void disable();
    };
}

#endif