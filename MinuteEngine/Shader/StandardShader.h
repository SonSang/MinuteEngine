/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#ifndef __ME_STANDARD_SHADER_H__
#define __ME_STANDARD_SHADER_H__

#ifdef _MSC_VER
#pragma once
#endif

#include <GL/glew.h>
#include <SDL_opengl.h>
#include "../Shader.h"
#include "../Scene.h"
#include "glm/gtc/type_ptr.hpp"

#define STANDARD_SHADER_MAX_LIGHT_NUM	16		// For texture number limit

namespace ME {
	class StandardShader : public Shader {
	public:
		inline static StandardShader create(const std::string& vpath, const std::string& fpath) {
			StandardShader s;
			s.vertShader = createVertShader(vpath);
			s.fragShader = createFragShader(fpath);
			s.program = createProgram(s.vertShader, s.fragShader);
			return s;
		}
		// Shader uniform variable names
		inline static std::string uPosition() noexcept {
			return "position";
		}
		inline static std::string uNormal() noexcept {
			return "normal";
		}

		inline static std::string uModelMat() noexcept {
			return "modelMat";
		}
		inline static std::string uViewMat() noexcept {
			return "viewMat";
		}
		inline static std::string uProjMat() noexcept {
			return "projMat";
		}

		inline static std::string uColor() noexcept {
			return "color";
		}
		inline static std::string uEdgeColor() noexcept {
			return "edgeColor";
		}

		inline static std::string uPolygonMode() noexcept {
			return "polygonMode";
		}
		inline static std::string uTextureMode() noexcept {
			return "textureMode";
		}
		inline static std::string uPhongMode() {
			return "phongMode";
		}

		inline static std::string uMaterialEmission() {
			return "material.emission";
		}
		inline static std::string uMaterialAmbient() {
			return "material.ambient";
		}
		inline static std::string uMaterialDiffuse() {
			return "material.diffuse";
		}
		inline static std::string uMaterialSpecular() {
			return "material.specular";
		}
		inline static std::string uMaterialShininess() {
			return "material.shininess";
		}

		inline static std::string uDiffuseMap() {
			return "diffuseMap";
		}
		inline static std::string uSpecularMap() {
			return "specularMap";
		}
		inline static std::string uNormalMap() {
			return "normalMap";
		}
		inline static std::string uParallaxMap() {
			return "parallaxMap";
		}

		inline static std::string uPmOptionMode() {
			return "pmOption.mode";
		}
		inline static std::string uPmOptionDepthScale() {
			return "pmOption.depthScale";
		}
		inline static std::string uPmOptionMinLayers() {
			return "pmOption.minLayers";
		}
		inline static std::string uPmOptionMaxLayers() {
			return "pmOption.maxLayers";
		}

		inline static std::string uLightNum() {
			return "lightNum";
		}
		inline static std::string uLightType(uint id) {
			return std::string("light[") + std::to_string(id) + "].type";
		}
		inline static std::string uLightPosition(uint id) {
			return std::string("light[") + std::to_string(id) + "].position";
		}
		inline static std::string uLightDirection(uint id) {
			return std::string("light[") + std::to_string(id) + "].direction";
		}
		inline static std::string uLightAmbient(uint id) {
			return std::string("light[") + std::to_string(id) + "].ambient";
		}
		inline static std::string uLightDiffuse(uint id) {
			return std::string("light[") + std::to_string(id) + "].diffuse";
		}
		inline static std::string uLightSpecular(uint id) {
			return std::string("light[") + std::to_string(id) + "].specular";
		}
		inline static std::string uLightValid(uint id) {
			return std::string("light[") + std::to_string(id) + "].valid";
		}

		inline static std::string uShadowNum() {
			return "shadowNum";
		}
		inline static std::string uShadow(uint id) {
			return std::string("shadow[") + std::to_string(id) + "]";
		}

		inline static std::string uEnvironmentMap() {
			return "envMap";
		}
		inline static std::string uCameraPosition() {
			return "cameraPosition";
		}
		inline static std::string uEmMode() {
			return "emMode";
		}
		inline static std::string uEmFactor() {
			return "emFactor";
		}

		// Shader texture unit
		inline static uint tDiffuseMap() {
			return 0;
		}
		inline static uint tSpecularMap() {
			return 1;
		}
		inline static uint tNormalMap() {
			return 2;
		}
		inline static uint tParallaxMap() {
			return 3;
		}
		inline static uint tEnvironmentMap() {
			return 4;
		}
		inline static uint tShadow(uint id) {
			return tEnvironmentMap() + 1 + id;
		}

		// Shader attributes
		inline static uint aPosition() noexcept {
			return 0;
		}
		inline static uint aNormal() noexcept {
			return 1;
		}
		inline static uint aTexcoord() noexcept {
			return 2;
		}
		inline static uint aTangent() noexcept {
			return 3;
		}
		inline static uint aBitangent() noexcept {
			return 4;
		}
		inline static void setAttributes(const Render& render) {
			glBindVertexArray(render.getVAO());
			glBindBuffer(GL_ARRAY_BUFFER, render.getVBO());

			const static auto vertMemSize = Render::Vertex::memSize();
			const static auto pOffset = Render::Vertex::positionOffset();
			const static auto nOffset = Render::Vertex::normalOffset();
			const static auto tOffset = Render::Vertex::texcoordOffset();
			const static auto tanOffset = Render::Vertex::tangentOffset();
			const static auto bOffset = Render::Vertex::bitangentOffset();

			glEnableVertexAttribArray(aPosition());
			glVertexAttribPointer(aPosition(), 3, GL_FLOAT, GL_FALSE, vertMemSize, (void*)pOffset);
			glEnableVertexAttribArray(aNormal());
			glVertexAttribPointer(aNormal(), 3, GL_FLOAT, GL_FALSE, vertMemSize, (void*)nOffset);
			glEnableVertexAttribArray(aTexcoord());
			glVertexAttribPointer(aTexcoord(), 4, GL_FLOAT, GL_FALSE, vertMemSize, (void*)tOffset);
			glEnableVertexAttribArray(aTangent());
			glVertexAttribPointer(aTangent(), 3, GL_FLOAT, GL_FALSE, vertMemSize, (void*)tanOffset);
			glEnableVertexAttribArray(aBitangent());
			glVertexAttribPointer(aBitangent(), 3, GL_FLOAT, GL_FALSE, vertMemSize, (void*)bOffset);
		}

		// Set uniform variables
		inline bool setUnifMaterial(const Material& mat) const {
			enable();
			int loc = getUnifLoc(uMaterialEmission());
			if (loc == -1) return false;
			glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(mat.getEmission())));

			loc = getUnifLoc(uMaterialAmbient());
			glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(mat.getAmbient())));

			loc = getUnifLoc(uMaterialDiffuse());
			glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(mat.getDiffuse())));

			loc = getUnifLoc(uMaterialSpecular());
			glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(mat.getSpecular())));

			loc = getUnifLoc(uMaterialShininess());
			glUniform1f(loc, mat.getShininess());

			return true;
		}
		inline bool setUnifShadow(const std::string& name, uint unitID, const Shadow& shadow, const glm::mat4& lightViewMat) const {
			enable();

			if (shadow.getValid()) {
				setUnifBool(name + ".valid", true);

				glEnable(GL_TEXTURE_2D);
				glActiveTexture(GL_TEXTURE0 + unitID);
				glBindTexture(GL_TEXTURE_2D, shadow.getDepthMap());
				setUnifInt(name + ".data", unitID);

				const auto projMat = shadow.getProjMat();
				const auto shadowMat = projMat * lightViewMat;
				setUnifMat4(name + ".shadowMat", shadowMat);
			}
			else
				setUnifBool(name + ".valid", false);
			return true;
		}
		inline bool setUnifLight(uint id, const Light& light) const {
			enable();

			int loc = getUnifLoc(uLightAmbient(id));
			if (loc == -1) return false;
			glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(light.getAmbient())));

			loc = getUnifLoc(uLightDiffuse(id));
			glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(light.getDiffuse())));

			loc = getUnifLoc(uLightSpecular(id));
			glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(light.getSpecular())));

			loc = getUnifLoc(uLightPosition(id));
			glUniform3fv(loc, 1, glm::value_ptr(light.getPosition()));

			loc = getUnifLoc(uLightDirection(id));
			glUniform3fv(loc, 1, glm::value_ptr(light.getDirection()));

			loc = getUnifLoc(uLightType(id));
			glUniform1i(loc, light.getType());

			loc = getUnifLoc(uLightValid(id));
			glUniform1i(loc, light.getValid());

			setUnifShadow(uShadow(id), tShadow(id), light.shadow, light.getViewMat());

			return true;
		}
		inline bool setUnifTexture2D(const std::string& name, uint unitID, const Render::Texture2D& texture) const {
			if (texture.valid) {
				setUnifBool(name + ".valid", true);
				
				glEnable(GL_TEXTURE_2D);
				glActiveTexture(GL_TEXTURE0 + unitID);
				glBindTexture(GL_TEXTURE_2D, texture.texture.id);
				setUnifInt(name + ".data", unitID);
			}
			else
				setUnifBool(name + ".valid", false);
			return true;
		}

		// Etc
		inline static uint maxLightNum() noexcept {
			return STANDARD_SHADER_MAX_LIGHT_NUM;
		}

		// Draw
		inline bool draw(const LightManager& lightManager) {
			auto num = lightManager.lights.size();
			num = (num < maxLightNum() ? num : maxLightNum());
			setUnifInt(uLightNum(), num);
			setUnifInt(uShadowNum(), num);
			bool ret = true;
			for (int i = 0; i < num; i++) {
				bool success = setUnifLight(i, lightManager.lights[i]);
				if (!success)
					ret = false;
			}
			return ret;
		}
		
		inline bool draw(const Render& render, const glm::mat4& modelMat) {
			const auto& option = render.getOptionC();
			setAttributes(render);		// Send attribute data

			// Send uniform data according to the render type
			if (render.type() == 1) {
				// PointRender
				enable();

				setUnifMat4(uModelMat(), modelMat);
				setUnifVec3(uColor(), option.color);
				setUnifBool(uPolygonMode(), true);
				setUnifBool(uPhongMode(), false);
				glPointSize(option.edgeWidth);

				glBindVertexArray(render.getVAO());
				glDrawElements(GL_POINTS, option.drawNum, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
			else if (render.type() == 2) {
				// LineRender
				enable();

				setUnifMat4(uModelMat(), modelMat);
				setUnifVec3(uColor(), option.color);
				setUnifBool(uPolygonMode(), true);
				setUnifBool(uPhongMode(), false);
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
					setUnifBool(uPolygonMode(), true);

					if (option.shadeMode == 0) {
						// Simple shading, use(basic) color without lighting
						setUnifVec3(uColor(), option.color);
						setUnifBool(uPhongMode(), false);
						setUnifBool(uTextureMode(), false);
					}
					else if (option.shadeMode == 1) {
						// Phong shading, with material
						setUnifMaterial(option.material);
						setUnifBool(uPhongMode(), true);
						setUnifBool(uTextureMode(), false);

						// Environment mapping option
						setUnifBool(uEmMode(), option.emMode);
						setUnifFloat(uEmFactor(), option.emFactor);
					}
					else if (option.shadeMode == 2) {
						// Phong shading, with textures
						setUnifBool(uPhongMode(), true);
						setUnifBool(uTextureMode(), true);
						setUnifTexture2D(uDiffuseMap(), tDiffuseMap(), option.diffuseMap);
						setUnifTexture2D(uSpecularMap(), tSpecularMap(), option.specularMap);
						setUnifTexture2D(uNormalMap(), tNormalMap(), option.normalMap);
						setUnifTexture2D(uParallaxMap(), tParallaxMap(), option.parallaxMap);

						// Parallax mapping option
						if (option.parallaxMap.valid) {
							setUnifInt(uPmOptionMode(), option.pmMode);
							setUnifFloat(uPmOptionDepthScale(), option.pmDepthScale);
							setUnifInt(uPmOptionMinLayers(), option.pmMinLayers);
							setUnifInt(uPmOptionMaxLayers(), option.pmMaxLayers);
						}

						// Environment mapping option
						setUnifBool(uEmMode(), option.emMode);
						setUnifFloat(uEmFactor(), option.emFactor);
					}
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glDrawElements(GL_TRIANGLES, 3 * option.drawNum, GL_UNSIGNED_INT, 0);
				}
				if (option.drawEdge) {
					setUnifVec3(uEdgeColor(), option.edgeColor);
					setUnifBool(uPolygonMode(), false);
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
					setUnifBool(uPolygonMode(), true);

					if (option.shadeMode == 0) {
						// Simple shading, use(basic) color without lighting
						setUnifVec3(uColor(), option.color);
						setUnifBool(uPhongMode(), false);
						setUnifBool(uTextureMode(), false);
					}
					else if (option.shadeMode == 1) {
						// Phong shading, with material
						setUnifMaterial(option.material);
						setUnifBool(uPhongMode(), true);
						setUnifBool(uTextureMode(), false);

						// Environment mapping option
						setUnifBool(uEmMode(), option.emMode);
						setUnifFloat(uEmFactor(), option.emFactor);
					}
					else if (option.shadeMode == 2) {
						// Phong shading, with textures
						setUnifBool(uPhongMode(), true);
						setUnifBool(uTextureMode(), true);
						setUnifTexture2D(uDiffuseMap(), tDiffuseMap(), option.diffuseMap);
						setUnifTexture2D(uSpecularMap(), tSpecularMap(), option.specularMap);
						setUnifTexture2D(uNormalMap(), tNormalMap(), option.normalMap);
						setUnifTexture2D(uParallaxMap(), tParallaxMap(), option.parallaxMap);

						// Parallax mapping option
						if (option.parallaxMap.valid) {
							setUnifInt(uPmOptionMode(), option.pmMode);
							setUnifFloat(uPmOptionDepthScale(), option.pmDepthScale);
							setUnifInt(uPmOptionMinLayers(), option.pmMinLayers);
							setUnifInt(uPmOptionMaxLayers(), option.pmMaxLayers);
						}

						// Environment mapping option
						setUnifBool(uEmMode(), option.emMode);
						setUnifFloat(uEmFactor(), option.emFactor);
					}
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glDrawElements(GL_QUADS, 4 * option.drawNum, GL_UNSIGNED_INT, 0);
				}
				if (option.drawEdge) {
					setUnifVec3(uEdgeColor(), option.edgeColor);
					setUnifBool(uPolygonMode(), false);
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
		inline bool draw(const Scene::Skybox& skybox) {
			if (skybox.valid) {
				enable();

				glEnable(GL_TEXTURE_CUBE_MAP);
				glActiveTexture(GL_TEXTURE0 + tEnvironmentMap());
				glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.texture.id);
				setUnifInt(uEnvironmentMap(), tEnvironmentMap());
			}
			return true;
		}
		inline bool draw(const Scene& scene) {
			draw(scene.getSkyboxC());

			// First draw lights
			draw(scene.getLightManagerC());

			// Second draw objects
			struct Item {
				int         id;
				glm::mat4   modelMat;
			};
			auto& graph = scene.getGraphC();
			int root = scene.getRoot();

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
			return true;
		}
	};
}

#endif