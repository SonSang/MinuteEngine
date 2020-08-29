/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#ifndef __ME_SKYBOX_MAP_SHADER_H__
#define __ME_SKYBOX_MAP_SHADER_H__

#ifdef _MSC_VER
#pragma once
#endif

#include <GL/glew.h>
#include <SDL_opengl.h>
#include "../Shader.h"
#include "../Scene.h"
#include "glm/gtc/type_ptr.hpp"

namespace ME {
	class SkyboxShader : public Shader {
	public:
		inline static SkyboxShader create(const std::string& vpath, const std::string& fpath) {
			SkyboxShader s;
			s.vertShader = createVertShader(vpath);
			s.fragShader = createFragShader(fpath);
			s.program = createProgram(s.vertShader, s.fragShader);
			return s;
		}
		// Shader uniform variable names
		inline static std::string uViewMat() noexcept {
			return "viewMat";
		}
		inline static std::string uProjMat() noexcept {
			return "projMat";
		}
		inline static std::string uSkybox() noexcept {
			return "skybox";
		}
		// Shader attributes
		inline static uint aPosition() noexcept {
			return 0;
		}
		inline static void setAttributes(const Render& render) {
			glBindVertexArray(render.getVAO());
			glBindBuffer(GL_ARRAY_BUFFER, render.getVBO());

			const static auto vertMemSize = Render::Vertex::memSize();
			const static auto pOffset = Render::Vertex::positionOffset();

			glEnableVertexAttribArray(aPosition());
			glVertexAttribPointer(aPosition(), 3, GL_FLOAT, GL_FALSE, vertMemSize, (void*)pOffset);
		}

		// Draw
		inline bool draw(const Render& render) {
			const auto& option = render.getOptionC();
			setAttributes(render);		// Send attribute data

			// Send uniform data according to the render type
			if (render.type() == 3) {
				// TriRender
				enable();

				glBindVertexArray(render.getVAO());
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDrawElements(GL_TRIANGLES, 3 * option.drawNum, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
			else if (render.type() == 4) {
				// QuadRender
				enable();

				glBindVertexArray(render.getVAO());
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDrawElements(GL_QUADS, 4 * option.drawNum, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
			return true;
		}
		inline bool draw(const Scene& scene) {
			if (scene.getSkyboxC().valid && scene.getSkyboxC().cube != nullptr) {
				glDepthMask(GL_FALSE);
				enable();

				glEnable(GL_TEXTURE_CUBE_MAP);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, scene.getSkyboxC().texture.id);
				setUnifInt(uSkybox(), 0);

				draw(*scene.getSkyboxC().cube);
				glDepthMask(GL_TRUE);
			}
			return true;
		}
	};
}

#endif