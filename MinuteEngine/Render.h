/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#ifndef __ME_RENDER_H__
#define __ME_RENDER_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "Utils.h"
#include "Color.h"
#include "Geometry.h"
#include "Material.h"
#include "Texture.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <memory>
#include <vector>

#define DEF_RENDER_TYPE			0
#define POINT_RENDER_TYPE		1
#define LINE_RENDER_TYPE		2
#define TRI_RENDER_TYPE			3
#define QUAD_RENDER_TYPE		4

namespace ME {
	class Render {
	public:
		struct Vertex {
			glm::vec3 position;			
			glm::vec3 normal;
			glm::vec4 texcoord;		// Texture coordinates
			glm::vec3 tangent;
			glm::vec3 bitangent;	// Tangent space vectors

			static inline auto memSize() {
				static const auto size = sizeof(position) + sizeof(normal) + sizeof(texcoord) + sizeof(tangent) + sizeof(bitangent);
				return size;
			}

			static inline auto positionOffset() {
				return 0;
			}
			static inline auto normalOffset() {
				static const auto size = sizeof(position);
				return size;
			}
			static inline auto texcoordOffset() {
				static const auto size = sizeof(position) + sizeof(normal);
				return size;
			}
			static inline auto tangentOffset() {
				static const auto size = sizeof(position) + sizeof(normal) + sizeof(texcoord);
				return size;
			}
			static inline auto bitangentOffset() {
				static const auto size = sizeof(position) + sizeof(normal) + sizeof(texcoord) + sizeof(tangent);
				return size;
			}
		};
		struct VertexMap {
			std::vector<std::vector<Vertex>> data;
			inline static VertexMap create(const Surface& surface) {
				int row = surface.getVertexMapC().size();
				int col = surface.getVertexMapC()[0].size();
				VertexMap vertexMap;
				vertexMap.data.resize(row);
				for (int i = 0; i < row; i++) {
					vertexMap.data[i].resize(col);
					for (int j = 0; j < col; j++) {
						vertexMap.data[i][j].position = surface.getVertexMapC()[i][j].position;
						vertexMap.data[i][j].normal = surface.getVertexMapC()[i][j].normal;
					}
				}
				return vertexMap;
			}
			inline static int index(int row, int col, int colSize) noexcept {
				return row * colSize + col;
			}
			inline int row() const {
				return (int)data.size();
			}
			inline int col() const {
				return (int)data[0].size();
			}
			inline Vertex get(int row, int col) const noexcept {
				return data[row][col];
			}
			inline std::vector<Vertex> toArray() const noexcept {
				std::vector<Vertex> arr;
				arr.resize(row() * col());
				for (int i = 0; i < row(); i++) 
					for (int j = 0; j < col(); j++) 
						arr[index(i, j, col())] = get(i, j);
				return arr;
			}
		};

		struct Texture2D {
			ME::Texture2D texture;
			bool valid = false;
		};

		struct Option {
			int drawNum = 0;	// Used in glDrawElements() call

			// @shadeMode : Shading mode for face rendering
			int shadeMode = 0;	// 0 : Simple shading, use ( basic ) color without lighting
								// 1 : Phong shading, with material
								// 2 : Phong shading, with textures
			bool drawFace = true;
			bool drawEdge = true;

			// Edge
			Color edgeColor = Color::black();
			float edgeWidth = 1.0f;

			// Colors
			Color color = Color::red();
			Material material = Material::red();

			// Textures
			Texture2D diffuseMap;
			Texture2D specularMap;
			Texture2D normalMap;
			Texture2D parallaxMap;

			// Parallax mapping
			int pmMode = 1;				// 0 : Single sampling
										// 1 : Multiple sampling
										// 2 : Occlusion
			float pmDepthScale = 0.05f;
			int pmMinLayers = 8;
			int pmMaxLayers = 128;		// Number of layers to do multiple layer sampling

			// Environment mapping
			bool emMode = false;
			float emFactor = 0.1f;		// Factor multiplied to environment pixel
		};
		
		using Ptr = std::shared_ptr<Render>;
	protected:
		uint vao = 0;
		uint vbo = 0;
		uint ebo = 0;
		Option option;
	public:
		// Return type of this [ Render ]
		inline virtual int type() const {
			return DEF_RENDER_TYPE;
		}

		inline void destroy() {
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo);
			glDeleteBuffers(1, &ebo);
			vao = 0;
			vbo = 0;
			ebo = 0;
		}

		inline uint getVAO() const noexcept {
			return vao;
		}
		inline uint getVBO() const noexcept {
			return vbo;
		}
		inline uint getEBO() const noexcept {
			return ebo;
		}

		inline void setVAO(uint vao) noexcept {
			this->vao = vao;
		}
		inline void setVBO(uint vbo) noexcept {
			this->vbo = vbo;
		}
		inline void setEBO(uint ebo) noexcept {
			this->ebo = ebo;
		}

		inline Option& getOption() noexcept {
			return option;
		}
		inline const Option& getOptionC() const noexcept {
			return option;
		}

		// Compute tangent space vectors [tangent], [bitangent] with given information
		// @aVec, bVec : Vectors in world space that describe two edges of a triangle
		// @aTVec, bTVec : Vectors in texture space that correspond to aVec and bVec
		static void computeTangentSpace(
			const glm::vec3& aVec, const glm::vec3& bVec, 
			const glm::vec2& aTVec, const glm::vec2& bTVec,
			glm::vec3& tan, glm::vec3& btan);

		virtual void drawUI(bool update);
	};

	class PointRender : public Render {
	private:
		PointRender() = default;
	public:
		using Ptr = std::shared_ptr<PointRender>;
		
		inline virtual int type() const {
			return POINT_RENDER_TYPE;
		}
		static PointRender create(const Vertex& vert);
		static Ptr createPtr(const Vertex& vert);
	};

	class LineRender : public Render {
	private:
		LineRender() = default;
	public:
		using Ptr = std::shared_ptr<LineRender>;
		
		inline virtual int type() const {
			return LINE_RENDER_TYPE;
		}
		static LineRender create(const Vertex vert[2]);
		static Ptr createPtr(const Vertex vert[2]);
	};

	class TriRender : public Render {
	private:
		TriRender() = default;
	public:
		using Ptr = std::shared_ptr<TriRender>;

		inline virtual int type() const {
			return TRI_RENDER_TYPE;
		}

		static TriRender createTriangle(const Vertex vert[3], bool computeNormal = true, bool computeTangentSpace = true) noexcept;
		static TriRender createQuad(const Vertex vert[4], bool computeNormal = true, bool computeTangentSpace = true) noexcept;

		static Ptr createTrianglePtr(const Vertex vert[3], bool computeNormal = true, bool computeTangentSpace = true) noexcept;
		static Ptr createQuadPtr(const Vertex vert[4], bool computeNormal = true, bool computeTangentSpace = true) noexcept;
	};

	class QuadRender : public Render {
	private:
		QuadRender() = default;
	public:
		using Ptr = std::shared_ptr<QuadRender>;

		inline virtual int type() const {
			return QUAD_RENDER_TYPE;
		}
		static QuadRender createQuad(const Vertex vertices[4]) noexcept;
		static QuadRender createCube(const Vertex& min, const Vertex& max) noexcept;
		static QuadRender createSurface(const VertexMap& vertexMap) noexcept;
		static Ptr createQuadPtr(const Vertex vertices[4]) noexcept;
		static Ptr createCubePtr(const Vertex& min, const Vertex& max) noexcept;
		static Ptr createSurfacePtr(const VertexMap& vertexMap) noexcept;
	};
}
#endif