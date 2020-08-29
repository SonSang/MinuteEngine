/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#include "Shader.h"
#include "IO.h"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <GL/glew.h>
#include <SDL_opengl.h>

namespace ME {
    Shader Shader::create(const std::string& vpath, const std::string& fpath) {
        Shader s;
        s.vertShader = createVertShader(vpath);
        s.fragShader = createFragShader(fpath);
        s.program = createProgram(s.vertShader, s.fragShader);
        return s;
    }
    void Shader::destroy(Shader& shader) {
        glDetachShader(shader.getProgram(), shader.getVertShader());
        glDetachShader(shader.getProgram(), shader.getFragShader());
        glDeleteShader(shader.getVertShader());
        glDeleteShader(shader.getFragShader());
        glDeleteProgram(shader.getProgram());
    }

    // Create ( Compile )
    uint Shader::createVertShader(const std::string& path) {
        int
            success;
        char
            infoLog[512];
        GLuint
            id;
        std::string
            shader = IO::read_text(path);
        const GLchar
            * glshader = shader.c_str();
        id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(id, 1, &glshader, NULL);
        glCompileShader(id);

        glGetShaderiv(id, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(id, 512, NULL, infoLog);
            throw(std::runtime_error(std::string("[SHADER ERROR] : Vertex shader compilation failed\n") +
                std::string("[COMPILATION ERROR MESSAGE] : \n") +
                std::string(infoLog)));
        }
        return id;
    }
    uint Shader::createFragShader(const std::string& path) {
        int
            success;
        char
            infoLog[512];
        GLuint
            id;
        std::string
            shader = IO::read_text(path);
        const GLchar
            * glshader = shader.c_str();
        id = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(id, 1, &glshader, NULL);
        glCompileShader(id);

        glGetShaderiv(id, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(id, 512, NULL, infoLog);
            throw(std::runtime_error(std::string("[SHADER ERROR] : Fragment shader compilation failed\n") +
                std::string("[COMPILATION ERROR MESSAGE] : \n") +
                std::string(infoLog)));
        }
        return id;
    }
    uint Shader::createProgram(uint vert, uint frag) {
        GLuint
            p = glCreateProgram();
        glAttachShader(p, vert);
        glAttachShader(p, frag);
        glLinkProgram(p);
        int
            success;
        char
            infoLog[512];
        glGetProgramiv(p, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(p, 512, NULL, infoLog);
            throw(std::runtime_error(std::string("[SHADER ERROR] : Shader program linkage failed\n") +
                std::string("[LINKAGE ERROR MESSAGE] : \n") +
                std::string(infoLog)));
        }
        return p;
    }
    
    // Get
    uint Shader::getProgram() const noexcept {
        return this->program;
    }
    uint Shader::getVertShader() const noexcept {
        return this->vertShader;
    }
    uint Shader::getFragShader() const noexcept {
        return this->fragShader;
    }

    // Matrix settings.
    /*void shader::set_uniform_model_matrix(const transform& TR) const {
        float mat[16];
        TR.to_array(mat, true);

        uint id = get_uniform_location("model");

        enable();
        glUniformMatrix4fv(id, 1, GL_FALSE, mat);
        disable();
    }
    void shader::set_uniform_view_matrix(const transform& TR) const {
        float mat[16];
        TR.to_array(mat, true);

        uint id = get_uniform_location("view");

        enable();
        glUniformMatrix4fv(id, 1, GL_FALSE, mat);
        disable();
    }
    void shader::set_uniform_view_matrix(const mat4d& M) const {
        float mat[16];
        M.to_array(mat, true);

        uint id = get_uniform_location("view");

        enable();
        glUniformMatrix4fv(id, 1, GL_FALSE, mat);
        disable();
    }
    void shader::set_uniform_projection_matrix(const mat4d& M) const {
        float mat[16];
        M.to_array(mat, true);

        uint id = get_uniform_location("projection");

        enable();
        glUniformMatrix4fv(id, 1, GL_FALSE, mat);
        disable();
    }*/

    int Shader::getAttrLoc(const std::string& var) const {
        return glGetAttribLocation(getProgram(), var.c_str());
    }
    int Shader::getUnifLoc(const std::string& var) const {
        return glGetUniformLocation(getProgram(), var.c_str());
    }
    bool Shader::setUnifInt(const std::string& var, int i) const {
        enable();
        auto loc = getUnifLoc(var);
        if (loc == -1)
            return false;
        glUniform1i(loc, i);
        return true;
    }
    bool Shader::setUnifFloat(const std::string& var, float f) const {
        enable();
        auto loc = getUnifLoc(var);
        if (loc == -1)
            return false;
        glUniform1f(loc, f);
        return true;
    }
    bool Shader::setUnifVec3(const std::string& var, const glm::vec3& vec) const {
        enable();
        int loc = getUnifLoc(var);
        if (loc == -1) 
            return false;
        glUniform3fv(loc, 1, glm::value_ptr(vec));
        return true;
    }
    bool Shader::setUnifMat4(const std::string& var, const glm::mat4& mat) const {
        enable();
        int loc = getUnifLoc(var);
        if (loc == -1)
            return false;
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
        return true;
    }
    bool Shader::setUnifBool(const std::string& var, bool b) const {
        enable();
        int loc = getUnifLoc(var);
        if (loc == -1)
            return false;
        glUniform1i(loc, b);
        return true;
    }

    // Usage
    void Shader::enable() const {
        glUseProgram(getProgram());
    }
    void Shader::disable() {
        glUseProgram(0);
    }
}