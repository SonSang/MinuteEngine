/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#ifndef __ME_CAMERA_H__
#define __ME_CAMERA_H__

#ifdef _MSC_VER
#pragma once
#endif

#define CAM_DEF_EYE     0.0f, 0.0f, 5.0f
#define CAM_DEF_CENTER  0.0f, 0.0f, 0.0f
#define CAM_DEF_UP      0.0f, 1.0f, 0.0f
#define CAM_DEF_YAW     -90.0f
#define CAM_DEF_PITCH   0.0f
#define CAM_DEF_SPEED   1.0f
#define CAM_DEF_FOVY    45.0f
#define CAM_DEF_NPLANE  0.1f
#define CAM_DEF_FPLANE  1000.0f
#define CAM_PITCH_UBOUND    89.0f
#define CAM_PITCH_LBOUND    -89.0f
#define CAM_DEF_ORTHO_WIDTH     10.0f;
#define CAM_DEF_ORTHO_HEIGHT    10.0f;

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "Color.h"

namespace ME {
    class Camera {
    private:
        // Camera configuration
        glm::vec3 eye{ CAM_DEF_EYE };
        glm::vec3 center{ CAM_DEF_CENTER };
        glm::vec3 up{ CAM_DEF_UP };
        float yaw = CAM_DEF_YAW;
        float pitch = CAM_DEF_PITCH;
        float orthoWidth = CAM_DEF_ORTHO_WIDTH;
        float orthoHeight = CAM_DEF_ORTHO_HEIGHT;

        // Window
        int wndWidth = 0;
        int wndHeight = 0;

        // Speed
        float speed{ CAM_DEF_SPEED };

        // Near, far plane.
        float fovy = CAM_DEF_FOVY;
        float nPlane = CAM_DEF_NPLANE;
        float fPlane = CAM_DEF_FPLANE;  // To reduce flickering,
                                        // enlarge near plane value.

        // view, projection matrix.
        glm::mat4 viewMat = glm::mat4(1.0f);
        glm::mat4 projMat = glm::mat4(1.0f);

        // Frame
        double frameTime = 0.0;

        // Back ground color
        Color bgColor = Color::black();

        // Control mode
        bool fpsMode = true;
        bool orthoMode = false;             // Projection mode : Orthogonal vs Perspective

        Camera() = default;
    public:
        static Camera create(int width, int height);

        void setEye(const glm::vec3& eye);
        void setCenter(const glm::vec3& center);
        void setUp(const glm::vec3& up);
        void setYaw(float yaw);
        void setPitch(float pitch);
        void setSpeed(float speed);
        void setFovy(float fovy);
        void setNPlane(float nPlane);
        void setFPlane(float fPlane);

        void setWindowSize(int width, int height) noexcept;
        int getWindowWidth() const noexcept;
        int getWindowHeight() const noexcept;

        glm::vec3 getEye() const noexcept;
        glm::vec3 getCenter() const noexcept;
        glm::vec3 getUp() const noexcept;
        float getYaw();
        float getPitch();
        float getSpeed() const noexcept;
        float getFovy() const noexcept;
        float getNPlane() const noexcept;
        float getFPlane() const noexcept;
        inline float getOrthoWidth() const noexcept {
            return orthoWidth;
        }
        inline float getOrthoHeight() const noexcept {
            return orthoHeight;
        }
        inline void setOrthoWidth(float width) {
            orthoWidth = width;
        }
        inline void setOrthoHeight(float height) {
            orthoHeight = height;
        }

        bool getFpsMode() const noexcept;
        void setFpsMode(bool b) noexcept;

        inline bool getOrthoMode() const noexcept {
            return orthoMode;
        }
        inline void setOrthoMode(bool b) noexcept {
            this->orthoMode = b;
        }

        // Move

        // FPS mode
        void updateAngle(float yawOffset, float pitchOffset);    // Update by yaw and pitch
        void moveBackForth(int forward, float deltaTime = 0.0f);
        void moveLeftRight(int right, float deltaTime = 0.0f);

        // Not FPS mode
        void translate(const glm::vec3& vec);
        void rotate(const glm::vec3& axis, float angle);
        void pan(const glm::vec3& vec);

        // == Actual matrix change only occurs here
        void updateViewMat();
        void updateProjMat();

        // View, projection matrix
        glm::mat4& getViewMat() noexcept;
        glm::mat4& getProjMat() noexcept;
        const glm::mat4& getViewMatC() const noexcept;
        const glm::mat4& getProjMatC() const noexcept;

        // Frame
        void setFrameTime(double frameTime) noexcept;
        double getFrameTime() const noexcept;
        double getFrameRate() const noexcept;

        // Background color
        void drawBG();
        void setBG(const Color& color) noexcept;
        Color getBG() const noexcept;
    };
}

#endif