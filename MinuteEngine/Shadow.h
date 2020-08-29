/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#ifndef __ME_SHADOW_H__
#define __ME_SHADOW_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "Utils.h"
#include "Shader.h"
#include "glm/mat4x4.hpp"
#include "glm/gtx/rotate_vector.hpp"    // For camera rotation.

#include <GL/glew.h>
#include <SDL_opengl.h>

namespace ME {
	class Shadow {
	public:
		struct ProjOption {
			// Projection matrix variables
			bool orthoMode = true;      // If false, use perspective projection
			float orthoLeft = -10.0f;
			float orthoRight = 10.0f;
			float orthoBottom = -10.0f;
			float orthoTop = 10.0f;

			float perspFovy = glm::radians(45.0f);
			float perspAspect = 1.0f;

			float projNear = 0.1f;
			float projFar = 100.0f;
		};
	private:
		bool valid = false;
		ProjOption projOption;
		uint fbo;					// Depth map Frame Buffer Object
		uint depthMap;				// Texture ID of depth map 
		uint mapWidth = 1024;
		uint mapHeight = 1024;		// Size of depth map
	public:
		inline static Shadow create() {
			Shadow shadow;

			glGenFramebuffers(1, &shadow.fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, shadow.fbo);

			glGenTextures(1, &shadow.depthMap);
			glBindTexture(GL_TEXTURE_2D, shadow.depthMap);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow.mapWidth, shadow.mapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow.depthMap, 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);	// Since we do not use color buffer, just set these to GL_NONE
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			return shadow;
		}
		inline uint getFBO() const noexcept {
			return fbo;
		}
		inline uint getDepthMap() const {
			return depthMap;
		}
		inline void getSize(uint& width, uint& height) const noexcept {
			width = mapWidth;
			height = mapHeight;
		}
		inline bool getValid() const noexcept {
			return valid;
		}
		inline ProjOption& getProjOption() noexcept {
			return projOption;
		}
		inline const ProjOption& getProjOptionC() const noexcept {
			return projOption;
		}
		inline glm::mat4 getProjMat() const noexcept {
			if (projOption.orthoMode) {
				return glm::ortho(
					projOption.orthoLeft,
					projOption.orthoRight,
					projOption.orthoBottom,
					projOption.orthoTop,
					projOption.projNear,
					projOption.projFar
				);
			}
			else {
				return glm::perspective(
					projOption.perspFovy,
					projOption.perspAspect,
					projOption.projNear,
					projOption.projFar
				);
			}
		}

		inline void setValid(bool valid) noexcept {
			this->valid = valid;
		}
	};
};

#endif