/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#include "Camera.h"
#include "glm/gtx/rotate_vector.hpp"    // For camera rotation.

#include <SDL_opengl.h>

namespace ME {
    Camera Camera::create(int width, int height) {
        Camera camera;
        camera.wndWidth = width;
        camera.wndHeight = height;
        camera.updateViewMat();
        camera.updateProjMat();
        return camera;
    }
    void Camera::setEye(const glm::vec3& eye) {
        this->eye = eye;
    }
    void Camera::setCenter(const glm::vec3& center) {
        this->center = center;
    }
    void Camera::setUp(const glm::vec3& up) {
        this->up = up;
    }
    void Camera::setYaw(float yaw) {
        this->yaw = yaw;
    }
    void Camera::setPitch(float pitch) {
        this->pitch = pitch;
    }
    void Camera::setSpeed(float speed) {
        this->speed = speed;
    }
    void Camera::setFovy(float fovy) {
        this->fovy = fovy;
    }
    void Camera::setNPlane(float nPlane) {
        this->nPlane = nPlane;
    }
    void Camera::setFPlane(float fPlane) {
        this->fPlane = fPlane;
    }
    void Camera::setWindowSize(int width, int height) noexcept {
        wndWidth = width;
        wndHeight = height;
    }
    int Camera::getWindowWidth() const noexcept {
        return wndWidth;
    }
    int Camera::getWindowHeight() const noexcept {
        return wndHeight;
    }
    glm::vec3 Camera::getEye() const noexcept {
        return this->eye;
    }
    glm::vec3 Camera::getCenter() const noexcept {
        return this->center;
    }
    glm::vec3 Camera::getUp() const noexcept {
        return this->up;
    }
    float Camera::getYaw() {
        return yaw;
    }
    float Camera::getPitch() {
        return pitch;
    }
    float Camera::getSpeed() const noexcept {
        return speed;
    }
    float Camera::getFovy() const noexcept {
        return fovy;
    }
    float Camera::getNPlane() const noexcept {
        return nPlane;
    }
    float Camera::getFPlane() const noexcept {
        return fPlane;
    }
    bool Camera::getFpsMode() const noexcept {
        return fpsMode;
    }
    void Camera::setFpsMode(bool b) noexcept {
        fpsMode = b;
    }
    void Camera::updateAngle(float yawOffset, float pitchOffset) {
        float
            nyaw = yaw + yawOffset,
            npitch = pitch + pitchOffset;
        if (npitch > CAM_PITCH_UBOUND)
            npitch = CAM_PITCH_UBOUND;
        else if (npitch < CAM_PITCH_LBOUND)
            npitch = CAM_PITCH_LBOUND;
        yaw = nyaw;
        pitch = npitch;

        float
            yRadian = glm::radians(yaw),
            pRadian = glm::radians(pitch);
        glm::vec3
            front{ cos(pRadian) * cos(yRadian), sin(pRadian), cos(pRadian) * sin(yRadian) };
        center = eye + front;
        updateViewMat();
    }
    void Camera::moveBackForth(int forward, float deltaTime) {
        float distance = getSpeed() * deltaTime * forward;
        glm::vec3 addition = glm::normalize(center - eye) * distance;
        setEye(eye + addition);
        setCenter(center + addition);
        updateViewMat();
    }
    void Camera::moveLeftRight(int right, float deltaTime) {
        float distance = getSpeed() * deltaTime * right;
        glm::vec3
            rVector = glm::normalize(glm::cross((center - eye), up)),
            addition = rVector * distance;
        setEye(eye + addition);
        setCenter(center + addition);
        updateViewMat();
    }
    void Camera::translate(const glm::vec3& vec) {
        eye += vec;

        updateViewMat();
    }
    void Camera::rotate(const glm::vec3& axis, float angle) {
        glm::vec3 v = eye - center;
        v = glm::rotate(v, angle, axis);
        eye = center + v;

        glm::vec3 nu = up;
        nu = glm::rotate(nu, angle, axis);
        up = nu;

        updateViewMat();
    }
    void Camera::pan(const glm::vec3& vec) {
        eye += vec;
        center += vec;

        updateViewMat();
    }
    void Camera::updateViewMat() {
        viewMat = glm::lookAt(eye, center, up);
    }
    void Camera::updateProjMat() {
        if (getOrthoMode())
            projMat = glm::ortho(-orthoWidth, orthoWidth, -orthoHeight, orthoHeight, nPlane, fPlane);
        else
            projMat = glm::perspective(glm::radians(fovy), wndWidth / (float)wndHeight, nPlane, fPlane);
    }
    glm::mat4& Camera::getViewMat() noexcept {
        return viewMat;
    }
    glm::mat4& Camera::getProjMat() noexcept {
        return projMat;
    }
    const glm::mat4& Camera::getViewMatC() const noexcept {
        return viewMat;
    }
    const glm::mat4& Camera::getProjMatC() const noexcept {
        return projMat;
    }
    void Camera::setFrameTime(double frameTime) noexcept {
        this->frameTime = frameTime;
    }
    double Camera::getFrameTime() const noexcept {
        return frameTime;
    }
    double Camera::getFrameRate() const noexcept {
        return 1.0 / frameTime;
    }
    void Camera::drawBG() {
        glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    }
    void Camera::setBG(const Color& color) noexcept {
        this->bgColor = color;
    }
    Color Camera::getBG() const noexcept {
        return bgColor;
    }
}