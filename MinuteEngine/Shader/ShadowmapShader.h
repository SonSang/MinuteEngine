/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#ifndef __ME_SHADOW_MAP_SHADER_H__
#define __ME_SHADOW_MAP_SHADER_H__

#ifdef _MSC_VER
#pragma once
#endif

#include <GL/glew.h>
#include <SDL_opengl.h>
#include "../Shader.h"
#include "../Scene.h"
#include "glm/gtc/type_ptr.hpp"

namespace ME {
	class ShadowmapShader : public Shader {
	public:
		inline static ShadowmapShader create(const std::string& vpath, const std::string& fpath) {
			ShadowmapShader s;
			s.vertShader = createVertShader(vpath);
			s.fragShader = createFragShader(fpath);
			s.program = createProgram(s.vertShader, s.fragShader);
			return s;
		}
		// Shader uniform variable names
		inline static std::string uModelMat() noexcept {
			return "modelMat";
		}
		inline static std::string uLightSpaceMat() noexcept {
			return "lightSpaceMat";
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
		inline bool draw(const Render& render, const glm::mat4& modelMat) {
			const auto& option = render.getOptionC();
			setAttributes(render);		// Send attribute data

			// Send uniform data according to the render type
			if (render.type() == 1) {
				// PointRender
				enable();

				setUnifMat4(uModelMat(), modelMat);
				glPointSize(option.edgeWidth);

				glBindVertexArray(render.getVAO());
				glDrawElements(GL_POINTS, option.drawNum, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
			else if (render.type() == 2) {
				// LineRender
				enable();

				setUnifMat4(uModelMat(), modelMat);
				glLineWidth(option.edgeWidth);

				glBindVertexArray(render.getVAO());
				glDrawElements(GL_LINES, option.drawNum, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
			else if (render.type() == 3) {
				// TriRender
				enable();

				// Since [ TriRender ] has its own model matrix, apply it first.
				setUnifMat4(uModelMat(), modelMat);// *this->modelMat);

				glBindVertexArray(render.getVAO());
				if (option.drawFace) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glDrawElements(GL_TRIANGLES, 3 * option.drawNum, GL_UNSIGNED_INT, 0);
				}
				if (option.drawEdge) {
					glLineWidth(option.edgeWidth);
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glDrawElements(GL_TRIANGLES, 3 * option.drawNum, GL_UNSIGNED_INT, 0);
				}
				glBindVertexArray(0);
			}
			else if (render.type() == 4) {
				// QuadRender
				enable();

				// Since [ QuadRender ] has its own model matrix, apply it first.
				setUnifMat4(uModelMat(), modelMat);// *this->modelMat);

				glBindVertexArray(render.getVAO());
				if (option.drawFace) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glDrawElements(GL_QUADS, 4 * option.drawNum, GL_UNSIGNED_INT, 0);
				}
				if (option.drawEdge) {
					glLineWidth(option.edgeWidth);
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glDrawElements(GL_QUADS, 4 * option.drawNum, GL_UNSIGNED_INT, 0);
				}
				glBindVertexArray(0);
			}
			return true;
		}
		inline bool draw(const Property& prop, const glm::mat4& modelMat) {
			if (prop.getRenderC() != nullptr)
				draw(*prop.getRenderC(), modelMat);
			return true;
		}
		inline bool draw(const Object& object, const glm::mat4& prevMat) {
			glm::mat4 modelMat = prevMat * object.getTransformC().getMat4();
			for (const auto& prop : object.getPropertiesC())
				draw(*prop.second, modelMat);
			return true;
		}
		inline bool draw(const Scene& scene) {
			const auto& lightManager = scene.getLightManagerC();
			struct Item {
				int         id;
				glm::mat4   modelMat;
			};
			auto& graph = scene.getGraphC();
			int root = scene.getRoot();

			for (const auto& light : lightManager.lights) {
				if (light.getValid() && light.shadow.getValid()) {
					// Ready framebuffer
					uint width, height;
					light.shadow.getSize(width, height);
					glViewport(0, 0, width, height);
					glBindFramebuffer(GL_FRAMEBUFFER, light.shadow.getFBO());
					glClear(GL_DEPTH_BUFFER_BIT);

					// Ready light space matrix
					const auto& viewMat = light.getViewMat();
					const auto& projMat = light.shadow.getProjMat();
					const auto& lightSpaceMat = projMat * viewMat;
					enable();
					setUnifMat4(uLightSpaceMat(), lightSpaceMat);

					// Render scene
					Item rootItem;
					rootItem.id = root;
					rootItem.modelMat = graph.at(root).getObjectC()->getTransformC().getMat4();

					std::vector<Item> drawQueue;
					drawQueue.reserve(graph.size());
					drawQueue.push_back(rootItem);
					while (!drawQueue.empty()) {
						Item item = drawQueue.back(); drawQueue.pop_back();
						auto& node = graph.at(item.id);
						auto& object = node.getObjectC();

						draw(*object, item.modelMat);

						item.modelMat = item.modelMat * object->getTransformC().getMat4();
						for (auto child : node.getChildC()) {
							item.id = child;
							drawQueue.push_back(item);
						}
					}

					// Unbind
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
				}
			}
			return true;
		}
	};
}

#endif