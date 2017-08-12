#include "stdafx.h"
#include "PrimitiveDrawer.h"
#include "Managers/ShaderManager.h"
#include "Renderer/Texture.h"

PrimitiveDrawer::PrimitiveDrawer()
{

}


void PrimitiveDrawer::DrawLine(glm::vec3 Start, glm::vec3 End, glm::vec4 StartColor, glm::vec4 EndColor, float Duration)
{
	Lines.push_back({ Start, StartColor, End, EndColor });
}

void PrimitiveDrawer::DrawPoint(glm::vec3 Position, glm::vec4 Color, float Duration)
{
	Points.push_back({ Position, Color });
}

void PrimitiveDrawer::Init()
{
	size_t id;
	if (ShaderManager::GetShaderManager().CreateShader("primitives", "primitives.vs", "primitives.fs", id))
	{
		DebugMaterial = std::make_unique<Material>(id);
	}
}

void PrimitiveDrawer::Update(float DeltaTime)
{

}

void PrimitiveDrawer::Render()
{
	if (!DebugMaterial)
	{
		return;
	}

	DebugMaterial->Bind();

	if (Lines.size())
	{
		glCreateBuffers(1, &LinesVBO);
		glNamedBufferStorage(LinesVBO, Lines.size() * sizeof(Line), Lines.data(), 0);
		glBindBuffer(GL_ARRAY_BUFFER, LinesVBO);

		glGenVertexArrays(1, &LinesVAO);
		glBindVertexArray(LinesVAO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec4), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec4), (GLvoid*)(sizeof(glm::vec3)));
		glEnableVertexAttribArray(1);

		glDrawArrays(GL_LINES, 0, Lines.size() * 2);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteVertexArrays(1, &LinesVAO);
		glDeleteBuffers(1, &LinesVBO);
	}


	if (Points.size())
	{
		glCreateBuffers(1, &PointsVBO);
		glNamedBufferStorage(PointsVBO, Points.size() * sizeof(Point), Points.data(), 0);
		glBindBuffer(GL_ARRAY_BUFFER, PointsVBO);

		glGenVertexArrays(1, &PointsVAO);
		glBindVertexArray(PointsVAO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec4), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec4), (GLvoid*)(sizeof(glm::vec3)));
		glEnableVertexAttribArray(1);

		glDrawArrays(GL_POINTS, 0, Points.size());

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteVertexArrays(1, &PointsVAO);
		glDeleteBuffers(1, &PointsVBO);
	}

	DebugMaterial->UnBind();

	Lines.clear();
	Points.clear();
}

void PrimitiveDrawer::DeInit()
{

}

PrimitiveDrawer::~PrimitiveDrawer()
{
}
