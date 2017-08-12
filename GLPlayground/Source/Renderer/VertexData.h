#pragma once
#include <GL/glew.h>
#include "Math/MathTypes.h"
#include <vector>
#include "Renderer/Texture.h"
#include "Renderer/ShaderProgram.h"
#include <memory>
#include "Renderer/Material.h"
#include "Renderer/Uniform.h"

using Index = uint32_t;


class VertexData
{
public:
	VertexData(const std::vector<Vertex> & Vertices, const std::vector<Index> & Indices);
	VertexData();

	VertexData(VertexData && MeshToReplace);
	VertexData & operator=(VertexData && MeshToReplace);

	~VertexData();

	void GenerateVertexData();

	void Bind();
	void Unbind();

	inline std::vector<Vertex> & GetVertices()
	{
		return Vertices;
	}

	inline std::vector<Index> & GetIndices()
	{
		return Indices;
	}

	void UpdateVertexData();
	void UpdateIndexData();
private:
	std::vector<Vertex> Vertices;
	std::vector<Index> Indices;

	GLuint VBO;
	GLuint VEO;
	GLuint VAO;
};

