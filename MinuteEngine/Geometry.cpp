/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#include "Geometry.h"
#include "glm/gtc/constants.hpp"
#include "glm/geometric.hpp"

namespace ME {
	Surface Surface::createSphere(const glm::vec3& center, double radius, int udiv, int vdiv, bool epsAtPole) {
		const static auto PI = glm::pi<double>();
		const static double umin = 0, umax = PI * 2.0;
		const static double vmin = 0, vmax = PI;
		const static double eps = 1e-8;
		
		VertexMap vertexMap;
		vertexMap.resize(udiv + 1);
		for (int i = 0; i <= udiv; i++) {
			double u = umin + (umax - umin) * (i / (double)udiv);
			vertexMap[i].resize(vdiv + 1);
			for (int j = 0; j <= vdiv; j++) {
				double v = vmin + (vmax - vmin) * (j / (double)vdiv);
				if (v == vmin && epsAtPole)
					v = eps;
				if (v == vmax && epsAtPole)
					v = vmax - eps;

				// center + [ Rsinv * cosu, Rsinv * sinu, Rcosv ]
				vertexMap[i][j].position[0] = center[0] + radius * sin(v) * cos(u);
				vertexMap[i][j].position[1] = center[1] + radius * sin(v) * sin(u);
				vertexMap[i][j].position[2] = center[2] + radius * cos(v);

				vertexMap[i][j].normal[0] = vertexMap[i][j].position[0] - center[0];
				vertexMap[i][j].normal[1] = vertexMap[i][j].position[1] - center[1];
				vertexMap[i][j].normal[2] = vertexMap[i][j].position[2] - center[2];

				vertexMap[i][j].normal = glm::normalize(vertexMap[i][j].normal);
			}
		}
		return create(vertexMap);
	}
}