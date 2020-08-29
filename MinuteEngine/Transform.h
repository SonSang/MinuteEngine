/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#ifndef __ME_TRANSFORM_H__
#define __ME_TRANSFORM_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/mat4x4.hpp"

namespace ME {
	class Transform {
	private:
		static const glm::vec3 defT; 
		static const glm::quat defR; 
		glm::vec3 translation = glm::vec3(0.0f);
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	public:
		void clear();
		void translate(const glm::vec3& v);
		void rotate(const glm::vec3& axis, float angle);
		glm::mat4 getMat4() const noexcept;

		void setTranslation(const glm::vec3& v) noexcept;
		void setRotation(const glm::quat& q) noexcept;
		glm::vec3& getTranslation() noexcept;
		glm::quat& getRotation() noexcept;
		const glm::vec3& getTranslationC() const noexcept;
		const glm::quat& getRotationC() const noexcept;
	};
}

#endif