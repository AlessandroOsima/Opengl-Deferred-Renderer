#pragma once
#include "glm/glm.hpp"
#include <GL/glew.h>
#include "Renderer/Material.h"
#include <memory>

class PrimitiveDrawer
{

private:
	struct Line
	{
		glm::vec3 Start;
		glm::vec4 StartColor;
		glm::vec3 End;
		glm::vec4 EndColor;
	};

	struct Point
	{
		glm::vec3 Location;
		glm::vec4 Color;
	};

	std::vector<Line> Lines;
	std::vector<Point> Points;
public:
	PrimitiveDrawer();

	void DrawLine(glm::vec3 Start, glm::vec3 End, glm::vec4 StartColor = glm::vec4(1,1,1,1), glm::vec4 EndColor = glm::vec4(1, 1, 1, 1), float Duration = 0);
	void DrawPoint(glm::vec3 Position, glm::vec4 Color = glm::vec4(1, 1, 1, 1), float Duration = 0);

	void Init();

	void Update(float DeltaTime);
	void Render();
	void DeInit();

	~PrimitiveDrawer();

private:
	GLuint LinesVBO;
	GLuint LinesVAO;

	GLuint PointsVBO;
	GLuint PointsVAO;

	std::unique_ptr<Material> DebugMaterial = nullptr;
};

