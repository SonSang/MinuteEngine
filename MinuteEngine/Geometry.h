/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#ifndef __ME_GEOMETRY_H__
#define __ME_GEOMETRY_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "glm/vec3.hpp"
#include <vector>

namespace ME {
	class Geometry {
	public:
		struct Vertex {
			glm::vec3 position;
			glm::vec3 normal;
		};
	};
	class Surface : public Geometry {
	public:
		using VertexMap = std::vector<std::vector<Vertex>>;
	private:
		VertexMap vertexMap;
	public:
		inline static Surface create(const VertexMap& vertexMap) noexcept {
			Surface surface;
			surface.vertexMap = vertexMap;
			return surface;
		}

		inline void setVertexMap(const VertexMap& vertexMap) {
			this->vertexMap = vertexMap;
		}
		VertexMap& getVertexMap() noexcept {
			return vertexMap;
		}
		const VertexMap& getVertexMapC() const noexcept {
			return vertexMap;
		}

		// center + [ Rsinv * cosu, Rsinv * sinu, Rcosv ]
		// @epsAtPole : Give epsilon value at v == 0 and v == PI to differentiate points
		static Surface createSphere(const glm::vec3& center, double radius, int udiv, int vdiv, bool epsAtPole = false);
	};
}

#endif