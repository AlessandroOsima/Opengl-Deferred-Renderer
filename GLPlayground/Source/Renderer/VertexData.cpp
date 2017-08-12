#include "stdafx.h"
#include "VertexData.h"
#include "Managers/ShaderManager.h"
#include "Managers/TextureManager.h"

VertexData::VertexData(const std::vector<Vertex> & Vertices, const std::vector<Index> & Indices) : Vertices(Vertices), Indices(Indices)
{
	GenerateVertexData();
}

VertexData::VertexData() : VEO(0), VBO(0), VAO(0)
{

}

VertexData::VertexData(VertexData && MeshToReplace)
{
	Vertices = std::move(MeshToReplace.Vertices);
	Indices = std::move(MeshToReplace.Indices);

	VBO = MeshToReplace.VBO;
	MeshToReplace.VBO = 0;

	VEO = MeshToReplace.VEO;
	MeshToReplace.VEO = 0;

	VAO = MeshToReplace.VAO;
	MeshToReplace.VAO = 0;

}

VertexData & VertexData::operator=(VertexData && MeshToReplace)
{
	Vertices = std::move(MeshToReplace.Vertices);
	Indices = std::move(MeshToReplace.Indices);

	VBO = MeshToReplace.VBO;
	MeshToReplace.VBO = 0;

	VEO = MeshToReplace.VEO;
	MeshToReplace.VEO = 0;

	VAO = MeshToReplace.VAO;
	MeshToReplace.VAO = 0;

	return *this;
}

VertexData::~VertexData()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VEO);

	glDeleteVertexArrays(1, &VAO);
}

void VertexData::GenerateVertexData()
{

	glCreateBuffers(1, &VEO);
	glNamedBufferStorage(VEO, sizeof(Index) * Indices.size(), Indices.data(), GL_DYNAMIC_STORAGE_BIT);


	glCreateBuffers(1, &VBO);
	glNamedBufferStorage(VBO, sizeof(Vertex) * Vertices.size(), Vertices.data(), GL_DYNAMIC_STORAGE_BIT);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);

	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);

	//Normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)((sizeof(glm::vec3) + sizeof(glm::vec4))));
	glEnableVertexAttribArray(2);

	//UV
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)((sizeof(glm::vec3) + sizeof(glm::vec4) + sizeof(glm::vec3))));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void VertexData::Bind()
{
	glBindVertexArray(VAO);
}

void VertexData::Unbind()
{
	glBindVertexArray(0);
}

void VertexData::UpdateVertexData()
{
	glNamedBufferSubData(VBO, 0, Vertices.size() * sizeof(Vertex), Vertices.data());
}

void VertexData::UpdateIndexData()
{
	glNamedBufferSubData(VEO, 0, Indices.size() * sizeof(Index), Indices.data());
}
