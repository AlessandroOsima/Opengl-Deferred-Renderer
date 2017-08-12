#pragma once
#include "Renderer/Mesh.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct WindowInfo
{
	int Width;
	int Height;
};

class GLRenderer
{
public:
	GLRenderer();

	bool Initialize(GLFWwindow * Window);

	void Clear();
	void Present();
	void DrawMesh(Mesh & mesh);
	
	void EnableDepthTest(bool Enable);

	~GLRenderer();

	inline void GetCurrentWindowInfo(WindowInfo & info) const
	{
		glfwGetWindowSize(Context, &info.Width, &info.Height);
	}

	void SetClearColor(const glm::vec4 & Color);


private:
	GLFWwindow * Context;
	glm::vec4 ClearColor = glm::vec4(0,0,0,0);
	bool DepthTestEnabled = false;
};

