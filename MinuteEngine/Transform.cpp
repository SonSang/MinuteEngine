/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#include "Transform.h"

namespace ME {
	const glm::vec3 Transform::defT = glm::vec3(0.0f);
	const glm::quat Transform::defR = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	void Transform::clear() {
		translation = defT;
		rotation = defR;
	}
	void Transform::translate(const glm::vec3& v) {
		translation += v;
	}
	void Transform::rotate(const glm::vec3& axis, float angle) {
		rotation = glm::rotate(rotation, glm::radians(angle), axis);
	}
	glm::mat4 Transform::getMat4() const noexcept {
		glm::mat4 tMat = glm::translate(glm::mat4(1.0f), translation);
		glm::mat4 rMat = glm::mat4_cast(rotation);
		return tMat * rMat;
	}
	void Transform::setTranslation(const glm::vec3& v) noexcept {
		this->translation = v;
	}
	void Transform::setRotation(const glm::quat& q) noexcept {
		this->rotation = q;
	}
	glm::vec3& Transform::getTranslation() noexcept {
		return translation;
	}
	glm::quat& Transform::getRotation() noexcept {
		return rotation;
	}
	const glm::vec3& Transform::getTranslationC() const noexcept {
		return translation;
	}
	const glm::quat& Transform::getRotationC() const noexcept {
		return rotation;
	}
}