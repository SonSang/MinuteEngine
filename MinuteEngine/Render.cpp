/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#include "Render.h"
#include "imgui/imgui.h"
#include "glm/ext/matrix_transform.hpp"

#include <GL/glew.h>
#include <SDL_opengl.h>
#include <vector>
#include <map>

#define BUFFER_DATA_USAGE GL_STATIC_DRAW

namespace ME {
	// Render
	void Render::computeTangentSpace(const glm::vec3& aVec, const glm::vec3& bVec, const glm::vec2& aTVec, const glm::vec2& bTVec, glm::vec3& tan, glm::vec3& btan) {
		auto du1 = aTVec[0];
		auto dv1 = aTVec[1];
		auto du2 = bTVec[0];
		auto dv2 = bTVec[1];

		auto det = du1 * dv2 - dv1 * du2;

		if (det == 0.0)
			return;

		tan[0] = (dv2 * aVec[0] - dv1 * bVec[0]) / det;
		tan[1] = (dv2 * aVec[1] - dv1 * bVec[1]) / det;
		tan[2] = (dv2 * aVec[2] - dv1 * bVec[2]) / det;

		btan[0] = (-du2 * aVec[0] + du1 * bVec[0]) / det;
		btan[1] = (-du2 * aVec[1] + du1 * bVec[1]) / det;
		btan[2] = (-du2 * aVec[2] + du1 * bVec[2]) / det;
	}
	void Render::drawUI(bool update) {
		ImGui::Text("Render Options");

		// Shade Mode
		const char* shadeModes[] = { "Simple", "Phong(Material)", "Phong(Texture)" };
		const char* curShadeMode = shadeModes[option.shadeMode];
		if (ImGui::BeginCombo("Shade Mode", curShadeMode)) {
			for (int i = 0; i < IM_ARRAYSIZE(shadeModes); i++) {
				bool isSelected = (curShadeMode == shadeModes[i]);
				if (ImGui::Selectable(shadeModes[i], isSelected)) {
					curShadeMode = shadeModes[i];
					option.shadeMode = i;
				}
			}
			ImGui::EndCombo();
		}

		// Draw Face & Edge
		ImGui::Checkbox("Draw Face", &option.drawFace);
		ImGui::Checkbox("Draw Edge", &option.drawEdge);

		// Edge Setting
		float edgeColor[3] = { option.edgeColor.r, option.edgeColor.g, option.edgeColor.b };
		ImGui::ColorEdit3("Edge Color", edgeColor);
		option.edgeColor.r = edgeColor[0];
		option.edgeColor.g = edgeColor[1];
		option.edgeColor.b = edgeColor[2];

		ImGui::InputFloat("Edge Width", &option.edgeWidth);

		// Face Setting
		float faceColor[3] = { option.color.r, option.color.g, option.color.b };
		ImGui::ColorEdit3("Face Color", faceColor);
		option.color.r = faceColor[0];
		option.color.g = faceColor[1];
		option.color.b = faceColor[2];

		// Material
		float matColor[3] = {
			option.material.getDiffuse().r,
			option.material.getDiffuse().g,
			option.material.getDiffuse().b };
		ImGui::ColorEdit3("Face Material(Diffuse)", matColor);
		option.material.setDiffuse(Color::create(matColor[0], matColor[1], matColor[2]));

		// Environment Mapping
		ImGui::Checkbox("Env Mapping", &option.emMode);
		ImGui::InputFloat("Env Mapping Factor", &option.emFactor);
	}
	// PointRender
	PointRender PointRender::create(const Vertex& vert) {
		PointRender render;

		glGenVertexArrays(1, &render.vao);
		glBindVertexArray(render.vao);

		auto vertMemSize = Vertex::memSize();

		// 1. VBO
		glGenBuffers(1, &render.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, render.vbo);
		glBufferData(GL_ARRAY_BUFFER, vertMemSize, &vert, BUFFER_DATA_USAGE);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// 2. EBO
		GLuint index[] = { 0 };

		glGenBuffers(1, &render.ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, BUFFER_DATA_USAGE);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// @WARNING : EBO must be unbound after VAO is unbounded.

		render.option.drawNum = 1;

		return render;
	}
	PointRender::Ptr PointRender::createPtr(const Vertex& vert) {
		PointRender p = create(vert);
		return std::make_shared<PointRender>(p);
	}

	// LineRender
	LineRender LineRender::create(const Vertex vert[2]) {
		LineRender render;

		glGenVertexArrays(1, &render.vao);
		glBindVertexArray(render.vao);

		auto vertMemSize = Vertex::memSize();

		// 1. VBO
		glGenBuffers(1, &render.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, render.vbo);
		glBufferData(GL_ARRAY_BUFFER, vertMemSize * 2, vert, BUFFER_DATA_USAGE);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// 2. EBO
		GLuint index[] = { 0, 1 };

		glGenBuffers(1, &render.ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, BUFFER_DATA_USAGE);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// @WARNING : EBO must be unbound after VAO is unbounded.

		render.option.drawNum = 2;

		return render;
	}
	LineRender::Ptr LineRender::createPtr(const Vertex vert[2]) {
		LineRender l = create(vert);
		return std::make_shared<LineRender>(l);
	}

	// TriRender
	TriRender TriRender::createTriangle(const Vertex vert[3], bool computeNormal, bool computeTangentSpace) noexcept {
		auto vertMemSize = Vertex::memSize();
		auto pOffset = Vertex::positionOffset();
		auto nOffset = Vertex::normalOffset();
		auto tOffset = Vertex::texcoordOffset();
		auto tanOffset = Vertex::tangentOffset();
		auto bOffset = Vertex::bitangentOffset();

		Vertex copy[3] = { vert[0], vert[1], vert[2] };

		if (computeNormal) {
			glm::vec3 normal;
			normal = glm::cross((copy[1].position - copy[0].position), (copy[2].position - copy[0].position));
			normal = glm::normalize(normal);
			for (int i = 0; i < 3; i++)
				copy[i].normal = normal;
		}

		if (computeTangentSpace) {
			glm::vec3 tangent, bitangent;
			Render::computeTangentSpace(
				copy[1].position - copy[0].position,
				copy[2].position - copy[0].position,
				copy[1].texcoord - copy[0].texcoord,
				copy[2].texcoord - copy[0].texcoord,
				tangent, bitangent);

			for (int i = 0; i < 3; i++) {
				copy[i].tangent = tangent;
				copy[i].bitangent = bitangent;
			}
		}

		TriRender render;
		glGenVertexArrays(1, &render.vao);
		glBindVertexArray(render.vao);

		// 1. VBO
		glGenBuffers(1, &render.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, render.vbo);
		glBufferData(GL_ARRAY_BUFFER, vertMemSize * 3, copy, BUFFER_DATA_USAGE);

		/*glEnableVertexAttribArray(Vertex::aPosition);
		glVertexAttribPointer(Vertex::aPosition, 3, GL_FLOAT, GL_FALSE, vertMemSize, (void*)pOffset);
		glEnableVertexAttribArray(Vertex::aNormal);
		glVertexAttribPointer(Vertex::aNormal, 3, GL_FLOAT, GL_FALSE, vertMemSize, (void*)nOffset);
		glEnableVertexAttribArray(Vertex::aTexcoord);
		glVertexAttribPointer(Vertex::aTexcoord, 4, GL_FLOAT, GL_FALSE, vertMemSize, (void*)tOffset);
		glEnableVertexAttribArray(Vertex::aTangent);
		glVertexAttribPointer(Vertex::aTangent, 3, GL_FLOAT, GL_FALSE, vertMemSize, (void*)tanOffset);
		glEnableVertexAttribArray(Vertex::aBitangent);
		glVertexAttribPointer(Vertex::aBitangent, 3, GL_FLOAT, GL_FALSE, vertMemSize, (void*)bOffset);*/
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// 2. EBO
		GLuint index[] = { 0, 1, 2 };

		glGenBuffers(1, &render.ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, BUFFER_DATA_USAGE);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// @WARNING : EBO must be unbound after VAO is unbounded.

		// 3. faceNum
		render.option.drawNum = 1;

		return render;
	}
	TriRender TriRender::createQuad(const Vertex vert[4], bool computeNormal, bool computeTangentSpace) noexcept {
		auto vertMemSize = Vertex::memSize();
		auto pOffset = Vertex::positionOffset();
		auto nOffset = Vertex::normalOffset();
		auto tOffset = Vertex::texcoordOffset();
		auto tanOffset = Vertex::tangentOffset();
		auto bOffset = Vertex::bitangentOffset();

		Vertex nvertices[6];
		nvertices[0] = vert[0];
		nvertices[1] = vert[1];
		nvertices[2] = vert[2];

		nvertices[3] = vert[0];
		nvertices[4] = vert[2];
		nvertices[5] = vert[3];

		if (computeNormal) {
			glm::vec3 n0 = glm::normalize(glm::cross((vert[1].position - vert[0].position), (vert[2].position - vert[0].position)));
			glm::vec3 n1 = glm::normalize(glm::cross((vert[2].position - vert[0].position), (vert[3].position - vert[0].position)));

			nvertices[0].normal = n0;
			nvertices[1].normal = n0;
			nvertices[2].normal = n0;

			nvertices[3].normal = n1;
			nvertices[4].normal = n1;
			nvertices[5].normal = n1;
		}

		/* TBN */
		if(computeTangentSpace) {
			glm::vec3 tangent, bitangent;
			Render::computeTangentSpace(
				nvertices[1].position - nvertices[0].position,
				nvertices[2].position - nvertices[0].position,
				nvertices[1].texcoord - nvertices[0].texcoord,
				nvertices[2].texcoord - nvertices[0].texcoord,
				tangent, bitangent);

			for (int i = 0; i < 3; i++) {
				nvertices[i].tangent = tangent;
				nvertices[i].bitangent = bitangent;
			}

			Render::computeTangentSpace(
				nvertices[2].position - nvertices[0].position,
				nvertices[3].position - nvertices[0].position,
				nvertices[2].texcoord - nvertices[0].texcoord,
				nvertices[3].texcoord - nvertices[0].texcoord,
				tangent, bitangent);

			for (int i = 0; i < 3; i++) {
				nvertices[i + 3].tangent = tangent;
				nvertices[i + 3].bitangent = bitangent;
			}
		}
		
		TriRender render;
		glGenVertexArrays(1, &render.vao);
		glBindVertexArray(render.vao);

		// 1. VBO
		glGenBuffers(1, &render.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, render.vbo);
		glBufferData(GL_ARRAY_BUFFER, vertMemSize * 6, nvertices, BUFFER_DATA_USAGE);

		/*glEnableVertexAttribArray(Vertex::aPosition);
		glVertexAttribPointer(Vertex::aPosition, 3, GL_FLOAT, GL_FALSE, vertMemSize, (void*)pOffset);
		glEnableVertexAttribArray(Vertex::aNormal);
		glVertexAttribPointer(Vertex::aNormal, 3, GL_FLOAT, GL_FALSE, vertMemSize, (void*)nOffset);
		glEnableVertexAttribArray(Vertex::aTexcoord);
		glVertexAttribPointer(Vertex::aTexcoord, 4, GL_FLOAT, GL_FALSE, vertMemSize, (void*)tOffset);
		glEnableVertexAttribArray(Vertex::aTangent);
		glVertexAttribPointer(Vertex::aTangent, 3, GL_FLOAT, GL_FALSE, vertMemSize, (void*)tanOffset);
		glEnableVertexAttribArray(Vertex::aBitangent);
		glVertexAttribPointer(Vertex::aBitangent, 3, GL_FLOAT, GL_FALSE, vertMemSize, (void*)bOffset);*/
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// 2. EBO
		GLuint index[] = { 0, 1, 2, 3, 4, 5 };

		glGenBuffers(1, &render.ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, BUFFER_DATA_USAGE);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// @WARNING : EBO must be unbound after VAO is unbounded.

		// 3. faceNum
		render.option.drawNum = 2;

		return render;
	}

	TriRender::Ptr TriRender::createTrianglePtr(const Vertex vert[3], bool computeNormal, bool computeTangentSpace) noexcept {
		TriRender t = createTriangle(vert, computeNormal);
		return std::make_shared<TriRender>(t);
	}
	TriRender::Ptr TriRender::createQuadPtr(const Vertex vert[4], bool computeNormal, bool computeTangentSpace) noexcept {
		TriRender t = createQuad(vert, computeNormal);
		return std::make_shared<TriRender>(t);
	}

	/*
	TriRender TriRender::createCube(const glm::vec3& min, const glm::vec3& max) noexcept {
		TriRender render;

		glGenVertexArrays(1, &render.vao);
		glBindVertexArray(render.vao);

		float minX, minY, minZ, maxX, maxY, maxZ;
		if (min.x < max.x) { minX = min.x; maxX = max.x; }
		else { minX = max.x; maxX = min.x; }
		if (min.y < max.y) { minY = min.y; maxY = max.y; }
		else { minY = max.y; maxY = min.y; }
		if (min.z < max.z) { minZ = min.z; maxZ = max.z; }
		else { minZ = max.z; maxZ = min.z; }

		glm::vec3 v[8];
		v[0] = glm::vec3(minX, minY, maxZ);
		v[1] = glm::vec3(maxX, minY, maxZ);
		v[2] = glm::vec3(maxX, maxY, maxZ);
		v[3] = glm::vec3(minX, maxY, maxZ);
		v[4] = glm::vec3(minX, minY, minZ);
		v[5] = glm::vec3(maxX, minY, minZ);
		v[6] = glm::vec3(maxX, maxY, minZ);
		v[7] = glm::vec3(minX, maxY, minZ);

		glm::vec3 n[6];
		n[0] = glm::vec3(0.0f, 0.0f, 1.0f);
		n[1] = glm::vec3(0.0f, 0.0f, -1.0f);
		n[2] = glm::vec3(0.0f, 1.0f, 0.0f);
		n[3] = glm::vec3(0.0f, -1.0f, 0.0f);
		n[4] = glm::vec3(1.0f, 0.0f, 0.0f);
		n[5] = glm::vec3(-1.0f, 0.0f, 0.0f);

		Vertex vertices[24];

		// Front
		vertices[0].position = v[0];	vertices[0].normal = n[0];
		vertices[1].position = v[1];	vertices[1].normal = n[0];
		vertices[2].position = v[2];	vertices[2].normal = n[0];
		vertices[3].position = v[3];	vertices[3].normal = n[0];

		// Back
		vertices[4].position = v[7];	vertices[4].normal = n[1];
		vertices[5].position = v[6];	vertices[5].normal = n[1];
		vertices[6].position = v[5];	vertices[6].normal = n[1];
		vertices[7].position = v[4];	vertices[7].normal = n[1];

		// Up
		vertices[8].position = v[3];	vertices[8].normal = n[2];
		vertices[9].position = v[2];	vertices[9].normal = n[2];
		vertices[10].position = v[6];	vertices[10].normal = n[2];
		vertices[11].position = v[7];	vertices[11].normal = n[2];

		// Down
		vertices[12].position = v[4];	vertices[12].normal = n[3];
		vertices[13].position = v[5];	vertices[13].normal = n[3];
		vertices[14].position = v[1];	vertices[14].normal = n[3];
		vertices[15].position = v[0];	vertices[15].normal = n[3];

		// Right
		vertices[16].position = v[1];	vertices[16].normal = n[4];
		vertices[17].position = v[5];	vertices[17].normal = n[4];
		vertices[18].position = v[6];	vertices[18].normal = n[4];
		vertices[19].position = v[2];	vertices[19].normal = n[4];

		// Left
		vertices[20].position = v[3];	vertices[20].normal = n[5];
		vertices[21].position = v[7];	vertices[21].normal = n[5];
		vertices[22].position = v[4];	vertices[22].normal = n[5];
		vertices[23].position = v[0];	vertices[23].normal = n[5];

		// 1. VBO
		glGenBuffers(1, &render.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, render.vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 24, vertices, BUFFER_DATA_USAGE);
		glEnableVertexAttribArray(SHADER_POSITION_ATTR);
		glVertexAttribPointer(SHADER_POSITION_ATTR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(SHADER_NORMAL_ATTR);
		glVertexAttribPointer(SHADER_NORMAL_ATTR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// 2. EBO
		GLuint index[] = {
			// Front
			0, 1, 2,
			0, 2, 3,
			// Back
			4, 5, 6,
			4, 6, 7,
			// Up
			8, 9, 10,
			8, 10, 11,
			// Down
			12, 13, 14,
			12, 14, 15,
			// Right
			16, 17, 18,
			16, 18, 19,
			// Left
			20, 21, 22,
			20, 22, 23
		};

		glGenBuffers(1, &render.ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, BUFFER_DATA_USAGE);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// @WARNING : EBO must be unbound after VAO is unbounded.

		// 3. faceNum
		render.faceNum = 12;	// 2 * 6

		return render;
	}

	struct triangle {
		glm::vec3 a;
		glm::vec3 b;
		glm::vec3 c;
	};
	static void createSeedTrianglesForSphere(const triangle& base, std::vector<triangle>& result, int complexity) {
		triangle t0, t1, t2, t3;
		glm::vec3
			mid01 = (base.a + base.b) * 0.5f,
			mid12 = (base.b + base.c) * 0.5f,
			mid20 = (base.c + base.a) * 0.5f;
		t0.a = base.a; t0.b = mid01; t0.c = mid20;
		t1.a = base.b; t1.b = mid12; t1.c = mid01;
		t2.a = base.c; t2.b = mid20; t2.c = mid12;
		t3.a = mid01; t3.b = mid12; t3.c = mid20;
		if (complexity-- == 1) {
			result.push_back(t0);
			result.push_back(t1);
			result.push_back(t2);
			result.push_back(t3);
			return;
		}
		createSeedTrianglesForSphere(t0, result, complexity);
		createSeedTrianglesForSphere(t1, result, complexity);
		createSeedTrianglesForSphere(t2, result, complexity);
		createSeedTrianglesForSphere(t3, result, complexity);
	}
	TriRender TriRender::createSphere(const glm::vec3& center, float radius, int complexity) noexcept {
		auto vertMemSize = Vertex::memSize();
		auto pOffset = Vertex::positionOffset();
		auto nOffset = Vertex::normalOffset();
		auto tOffset = Vertex::texcoordOffset();
		auto tanOffset = Vertex::tangentOffset();
		auto bOffset = Vertex::bitangentOffset();

		TriRender render;

		glGenVertexArrays(1, &render.vao);
		glBindVertexArray(render.vao);

		glm::vec3
			xrad(radius, 0.0f, 0.0f),
			yrad(0.0f, radius, 0.0f),
			zrad(0.0f, 0.0f, radius),
			npole = center + yrad,
			spole = center - yrad,
			waist0 = center + zrad,
			waist1 = center + xrad,
			waist2 = center - zrad,
			waist3 = center - xrad;
		triangle
			base[8];
		base[0].a = npole; base[0].b = waist0; base[0].c = waist1;
		base[1].a = npole; base[1].b = waist1; base[1].c = waist2;
		base[2].a = npole; base[2].b = waist2; base[2].c = waist3;
		base[3].a = npole; base[3].b = waist3; base[3].c = waist0;
		base[4].a = spole; base[4].b = waist1; base[4].c = waist0;
		base[5].a = spole; base[5].b = waist2; base[5].c = waist1;
		base[6].a = spole; base[6].b = waist3; base[6].c = waist2;
		base[7].a = spole; base[7].b = waist0; base[7].c = waist3;

		std::vector<triangle>
			triangles;
		for (int i = 0; i < 8; i++)
			createSeedTrianglesForSphere(base[i], triangles, complexity);

		std::vector<Vertex>
			vertices;

		vertices.reserve(triangles.size() * 3);
		for (int i = 0; i < triangles.size(); i++) {
			triangle& t = triangles[i];

			Vertex vert;

			glm::vec3 v = t.a - center;
			v = glm::normalize(v);
			vert.normal = v;
			vert.position = center + v * radius;
			vertices.push_back(vert);

			v = t.b - center;
			v = glm::normalize(v);
			vert.normal = v;
			vert.position = center + v * radius;
			vertices.push_back(vert);

			v = t.c - center;
			v = glm::normalize(v);
			vert.normal = v;
			vert.position = center + v * radius;
			vertices.push_back(vert);
		}

		// 1. VBO
		glGenBuffers(1, &render.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, render.vbo);
		glBufferData(GL_ARRAY_BUFFER, vertMemSize * vertices.size(), vertices.data(), BUFFER_DATA_USAGE);
		glEnableVertexAttribArray(SHADER_POSITION_ATTR);
		glVertexAttribPointer(SHADER_POSITION_ATTR, 3, GL_FLOAT, GL_FALSE, vertMemSize, (void*)pOffset);
		glEnableVertexAttribArray(SHADER_NORMAL_ATTR);
		glVertexAttribPointer(SHADER_NORMAL_ATTR, 3, GL_FLOAT, GL_FALSE, vertMemSize, (void*)nOffset);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// 2. EBO
		GLuint* index = new GLuint[triangles.size() * 3];
		for (int i = 0; i < triangles.size() * 3; i++)
			index[i] = (GLuint)i;

		glGenBuffers(1, &render.ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * triangles.size() * 3, index, BUFFER_DATA_USAGE);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// @WARNING : EBO must be unbound after VAO is unbounded.

		// 3. faceNum
		render.faceNum = (int)triangles.size();

		return render;
	}

	TriRender::Ptr TriRender::createTrianglePtr(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) noexcept {
		TriRender t = createTriangle(a, b, c);
		return std::make_shared<TriRender>(t);
	}
	TriRender::Ptr TriRender::createQuadPtr(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d) noexcept {
		TriRender t = createQuad(a, b, c, d);
		return std::make_shared<TriRender>(t);
	}
	TriRender::Ptr TriRender::createCubePtr(const glm::vec3& min, const glm::vec3& max) noexcept {
		TriRender t = createCube(min, max);
		return std::make_shared<TriRender>(t);
	}
	TriRender::Ptr TriRender::createSpherePtr(const glm::vec3& center, float radius, int complexity) noexcept {
		TriRender t = createSphere(center, radius, complexity);
		return std::make_shared<TriRender>(t);
	}*/


	// QuadRender
	QuadRender QuadRender::createQuad(const Vertex vertices[4]) noexcept {
		const static auto vertMemSize = vertices[0].memSize();
		const static auto pOffset = vertices[0].positionOffset();
		const static auto nOffset = vertices[0].normalOffset();
		const static auto tOffset = vertices[0].texcoordOffset();
		const static auto tanOffset = vertices[0].tangentOffset();
		const static auto bOffset = vertices[0].bitangentOffset();

		glm::vec3 tangent, bitangent;
		computeTangentSpace(
			vertices[1].position - vertices[0].position,
			vertices[3].position - vertices[0].position,
			vertices[1].texcoord - vertices[0].texcoord,
			vertices[3].texcoord - vertices[0].texcoord,
			tangent, bitangent);

		Vertex copy[4];
		for (int i = 0; i < 4; i++) {
			copy[i] = vertices[i];
			copy[i].tangent = tangent;
			copy[i].bitangent = bitangent;
		}
		
		QuadRender render;
		glGenVertexArrays(1, &render.vao);
		glBindVertexArray(render.vao);

		// 1. VBO
		glGenBuffers(1, &render.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, render.vbo);
		glBufferData(GL_ARRAY_BUFFER, vertMemSize * 4, copy, BUFFER_DATA_USAGE);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// 2. EBO
		GLuint index[] = { 0, 1, 2, 3 };

		glGenBuffers(1, &render.ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, BUFFER_DATA_USAGE);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// @WARNING : EBO must be unbound after VAO is unbounded.

		// 3. faceNum
		render.option.drawNum = 1;

		return render;
	}
	QuadRender QuadRender::createCube(const Vertex& min, const Vertex& max) noexcept {
		const static auto vertMemSize = Vertex::memSize();
		const static auto pOffset = Vertex::positionOffset();
		const static auto nOffset = Vertex::normalOffset();
		const static auto tOffset = Vertex::texcoordOffset();
		const static auto tanOffset = Vertex::tangentOffset();
		const static auto bOffset = Vertex::bitangentOffset();

		auto minpX = min.position[0];
		auto minpY = min.position[1];
		auto minpZ = min.position[2];
		auto maxpX = max.position[0];
		auto maxpY = max.position[1];
		auto maxpZ = max.position[2];

		Vertex vert[24];
		// Top
		vert[0].position[0] = minpX; vert[0].position[1] = maxpY; vert[0].position[2] = minpZ;
		vert[1].position[0] = minpX; vert[1].position[1] = maxpY; vert[1].position[2] = maxpZ;
		vert[2].position[0] = maxpX; vert[2].position[1] = maxpY; vert[2].position[2] = maxpZ;
		vert[3].position[0] = maxpX; vert[3].position[1] = maxpY; vert[3].position[2] = minpZ;
		vert[0].normal = glm::vec3(0, 1, 0);
		vert[1].normal = glm::vec3(0, 1, 0);
		vert[2].normal = glm::vec3(0, 1, 0);
		vert[3].normal = glm::vec3(0, 1, 0);

		// Bottom
		vert[4].position[0] = minpX; vert[4].position[1] = minpY; vert[4].position[2] = minpZ;
		vert[5].position[0] = maxpX; vert[5].position[1] = minpY; vert[5].position[2] = minpZ;
		vert[6].position[0] = maxpX; vert[6].position[1] = minpY; vert[6].position[2] = maxpZ;
		vert[7].position[0] = minpX; vert[7].position[1] = minpY; vert[7].position[2] = maxpZ;
		vert[4].normal = glm::vec3(0, -1, 0);
		vert[5].normal = glm::vec3(0, -1, 0);
		vert[6].normal = glm::vec3(0, -1, 0);
		vert[7].normal = glm::vec3(0, -1, 0);

		// Left
		vert[8].position[0] = minpX; vert[8].position[1] = maxpY; vert[8].position[2] = minpZ;
		vert[9].position[0] = minpX; vert[9].position[1] = minpY; vert[9].position[2] = minpZ;
		vert[10].position[0] = minpX; vert[10].position[1] = minpY; vert[10].position[2] = maxpZ;
		vert[11].position[0] = minpX; vert[11].position[1] = maxpY; vert[11].position[2] = maxpZ;
		vert[8].normal = glm::vec3(-1, 0, 0);
		vert[9].normal = glm::vec3(-1, 0, 0);
		vert[10].normal = glm::vec3(-1, 0, 0);
		vert[11].normal = glm::vec3(-1, 0, 0);

		// Right
		vert[12].position[0] = maxpX; vert[12].position[1] = minpY; vert[12].position[2] = minpZ;
		vert[13].position[0] = maxpX; vert[13].position[1] = maxpY; vert[13].position[2] = minpZ;
		vert[14].position[0] = maxpX; vert[14].position[1] = maxpY; vert[14].position[2] = maxpZ;
		vert[15].position[0] = maxpX; vert[15].position[1] = minpY; vert[15].position[2] = maxpZ;
		vert[12].normal = glm::vec3(1, 0, 0);
		vert[13].normal = glm::vec3(1, 0, 0);
		vert[14].normal = glm::vec3(1, 0, 0);
		vert[15].normal = glm::vec3(1, 0, 0);

		// Front
		vert[16].position[0] = minpX; vert[16].position[1] = maxpY; vert[16].position[2] = maxpZ;
		vert[17].position[0] = minpX; vert[17].position[1] = minpY; vert[17].position[2] = maxpZ;
		vert[18].position[0] = maxpX; vert[18].position[1] = minpY; vert[18].position[2] = maxpZ;
		vert[19].position[0] = maxpX; vert[19].position[1] = maxpY; vert[19].position[2] = maxpZ;
		vert[16].normal = glm::vec3(0, 0, 1);
		vert[17].normal = glm::vec3(0, 0, 1);
		vert[18].normal = glm::vec3(0, 0, 1);
		vert[19].normal = glm::vec3(0, 0, 1);

		// Back
		vert[20].position[0] = maxpX; vert[20].position[1] = minpY; vert[20].position[2] = minpZ;
		vert[21].position[0] = minpX; vert[21].position[1] = minpY; vert[21].position[2] = minpZ;
		vert[22].position[0] = minpX; vert[22].position[1] = maxpY; vert[22].position[2] = minpZ;
		vert[23].position[0] = maxpX; vert[23].position[1] = maxpY; vert[23].position[2] = minpZ;
		vert[20].normal = glm::vec3(0, 0, -1);
		vert[21].normal = glm::vec3(0, 0, -1);
		vert[22].normal = glm::vec3(0, 0, -1);
		vert[23].normal = glm::vec3(0, 0, -1);

		QuadRender render;
		glGenVertexArrays(1, &render.vao);
		glBindVertexArray(render.vao);

		// 1. VBO
		glGenBuffers(1, &render.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, render.vbo);
		glBufferData(GL_ARRAY_BUFFER, vertMemSize * 24, vert, BUFFER_DATA_USAGE);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// 2. EBO
		GLuint index[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };

		glGenBuffers(1, &render.ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, BUFFER_DATA_USAGE);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// @WARNING : EBO must be unbound after VAO is unbounded.

		// 3. faceNum
		render.option.drawNum = 24;

		return render;
	}
	QuadRender QuadRender::createSurface(const VertexMap& vertexMap) noexcept {
		QuadRender render;
		glGenVertexArrays(1, &render.vao);
		glBindVertexArray(render.vao);

		auto vertexArray = vertexMap.toArray();

		// 1. VBO
		glGenBuffers(1, &render.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, render.vbo);
		glBufferData(GL_ARRAY_BUFFER, Vertex::memSize() * vertexArray.size(), vertexArray.data(), BUFFER_DATA_USAGE);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// 2. EBO
		int rowNum = vertexMap.row();
		int colNum = vertexMap.col();

		std::vector<GLuint> index;
		index.resize((rowNum - 1) * (colNum - 1) * 4);
		int cnt = 0;
		for (int i = 0; i < rowNum - 1; i++) {
			for (int j = 0; j < colNum - 1; j++) {
				int row = i;
				int col = j;
				index[cnt++] = VertexMap::index(row, col, colNum);
				index[cnt++] = VertexMap::index(row + 1, col, colNum);
				index[cnt++] = VertexMap::index(row + 1, col + 1, colNum);
				index[cnt++] = VertexMap::index(row, col + 1, colNum);
			}
		}

		glGenBuffers(1, &render.ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * index.size(), index.data(), BUFFER_DATA_USAGE);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// @WARNING : EBO must be unbound after VAO is unbounded.

		// 3. faceNum
		render.option.drawNum = (rowNum - 1) * (colNum - 1);

		return render;
	}
	QuadRender::Ptr QuadRender::createQuadPtr(const Vertex vertices[4]) noexcept {
		return std::make_shared<QuadRender>(createQuad(vertices));
	}
	QuadRender::Ptr QuadRender::createCubePtr(const Vertex& min, const Vertex& max) noexcept {
		return std::make_shared<QuadRender>(createCube(min, max));
	}
	QuadRender::Ptr QuadRender::createSurfacePtr(const VertexMap& vertexMap) noexcept {
		return std::make_shared<QuadRender>(createSurface(vertexMap));
	}
	
}