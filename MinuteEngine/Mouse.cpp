/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#include "Mouse.h"
#include "glm/gtx/rotate_vector.hpp"

namespace ME {
	Mouse Mouse::create() {
		return Mouse();
	}
	void Mouse::setCoords(float x, float y) {
		this->x = x;
		this->y = y;
	}
	void Mouse::setDirty(bool dirty) {
		this->dirty = dirty;
	}
	void Mouse::setLPress(bool p) {
		this->LPress = p;
	}
	void Mouse::setRPress(bool p) {
		this->RPress = p;
	}
	void Mouse::setMPress(bool p) {
		this->MPress = p;
	}
	void Mouse::setSensitivity(float sensitivity) {
		this->sensitivity = sensitivity;
	}
	float Mouse::getX() const noexcept {
		return x;
	}
	float Mouse::getY() const noexcept {
		return y;
	}
	bool Mouse::getDirty() const noexcept {
		return dirty;
	}
	bool Mouse::getLPress() const noexcept {
		return LPress;
	}
	bool Mouse::getRPress() const noexcept {
		return RPress;
	}
	bool Mouse::getMPress() const noexcept {
		return MPress;
	}
	float Mouse::getSensitivity() const noexcept {
		return sensitivity;
	}
	void Mouse::moveCamera(Camera& cam, float nx, float ny) {
		if (cam.getOrthoMode()) {
			if (RPress)
				rotCamera(cam, nx, ny);
			else if (LPress)
				panCamera(cam, nx, ny);
		}
		else {
			if (cam.getFpsMode()) {
				if (RPress)
					rotCamera(cam, nx, ny);
			}
			else {
				if (LPress)
					rotCamera(cam, nx, ny);
				else if (RPress)
					panCamera(cam, nx, ny);
			}
		}
	}
	void Mouse::rotCamera(Camera& cam, float nx, float ny) {
		float
			xoffset = (nx - x),
			yoffset = (ny - y);
		if (cam.getFpsMode()) {
			xoffset *= sensitivity;
			yoffset *= -sensitivity;
			cam.updateAngle(xoffset, yoffset);
		}
		else {
			// Rotation around Y axis
			//cam.rotate({ 0.0f, 1.0f, 0.0f }, glm::radians(-xoffset));

			// Rotation around arbitrary axis
			glm::vec3
				sight = cam.getCenter() - cam.getEye(),
				right = glm::cross(sight, cam.getUp()),
				up = glm::cross(right, sight);
			if (sight.x == 0.0f && sight.y == 0.0f && sight.z == 0.0f)
				return;

			glm::vec3
				axis = up * -xoffset + right * -yoffset;	// Since Y coordinates are upside-down in OpenGL, put (-) in front of yoffset, too.
			axis = glm::normalize(axis);
			float
				angle = sqrt(xoffset * xoffset + yoffset * yoffset) * sensitivity;
			cam.rotate(axis, angle);
		}
		setCoords(nx, ny);
	}
	void Mouse::panCamera(Camera& cam, float nx, float ny) {
		float
			xoffset = (nx - x),
			yoffset = (ny - y);
		glm::vec3
			sight = cam.getCenter() - cam.getEye(),
			right = glm::cross(sight, cam.getUp()),
			up = glm::cross(right, sight);
		if (sight.x == 0.0f && sight.y == 0.0f && sight.z == 0.0f)
			return;

		right = glm::normalize(right);
		up = glm::normalize(up);
		glm::vec3
			offset = right * -xoffset + up * yoffset;
		offset = glm::normalize(offset) * sensitivity;
		cam.pan(offset);

		setCoords(nx, ny);
	}
}