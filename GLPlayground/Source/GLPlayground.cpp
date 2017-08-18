// GLPlayground.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Renderer/GLRenderer.h"
#include "Renderer/ShaderProgram.h"
#include "Renderer/Mesh.h"
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <strstream>
#include "Logger/Logger.h"
#include "Managers/ResourceManager.h"
#include "Managers/InputManager.h"
#include "Renderer/Texture.h"
#include "Renderer/GLUtilities.h"
#include "Renderer/RenderableScene.h"
#include "GameObjects/Scene.h"
#include "Managers/InputManager.h"

void GLFWErrorCallback(int ErrorCode, const char * Description)
{
	std::stringstream stream;
	stream << "GLFW error with code " << ErrorCode << " and description " << Description << std::ends;
	Logger::GetLogger().LogString(stream.str(), LogType::ERROR);
}

GLFWwindow * CreateWindow(const WindowInfo & Window, const std::string & WindowTitle)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

#ifdef GL_DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	return glfwCreateWindow(Window.Width, Window.Height, WindowTitle.c_str(), nullptr, nullptr);

}

//Separation is done here to force destruction of glfw related objects before glfwTerminate is called
void RunMainLoop(GLFWwindow * window)
{
	Assert(window);

	GLRenderer renderer;

	if (!renderer.Initialize(window))
	{
		glfwTerminate();
		return;
	}

	InputManager & InputManagerInstance = InputManager::GetInputManager();
	InputManagerInstance.Initialize(*window);

	RenderableScene renderScene(renderer);
	Scene scene(renderScene);

	renderScene.Initialize();
	scene.Initialize();


	double StartTime, EndTime;
	float DeltaTime = 0;

	bool ShouldTerminate = false;
	InputManager::GetInputManager() += [&](KeyState keyState, KeyCode keyCode)
	{
		if (keyCode == KeyCode::Escape && keyState == KeyState::PRESSED)
		{
			ShouldTerminate = true;
			Logger::GetLogger().LogString("Escape Key Detected, Terminating", LogType::LOG);
		}
	};

	while (!ShouldTerminate && !glfwWindowShouldClose(window))
	{
		StartTime = glfwGetTime();

		glfwPollEvents();

		scene.Update(DeltaTime);
		renderScene.RenderScene(DeltaTime);
		EndTime = glfwGetTime();

		DeltaTime = static_cast<float>(EndTime - StartTime);
	}

	scene.DeInitialize();
	renderScene.DeInitialize();
}

int main()
{
	Logger::GetLogger().LogOnFile(true);

	glfwSetErrorCallback(GLFWErrorCallback);

	if (glfwInit() == GL_FALSE)
	{
		return -1;
	}


	//WindowInfo windowInfo{ 1340, 880 };
	WindowInfo windowInfo{ 1680, 1050 };
	GLFWwindow * window = CreateWindow(windowInfo, "OpenGL Renderer - Blinn Phong");

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	RunMainLoop(window);

	glfwTerminate();

    return 0;
}

