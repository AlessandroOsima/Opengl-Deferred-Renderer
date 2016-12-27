﻿// GLPlayground.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GLRenderer.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <strstream>
#include "Logger.h"
#include "ResourceManager.h"
#include "Texture.h"


struct WindowInfo
{
	int Width;
	int Height;
};

GLFWwindow * CreateWindow(const WindowInfo & Window, const std::string & WindowTitle)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

	return glfwCreateWindow(Window.Width, Window.Height, WindowTitle.c_str(), nullptr, nullptr);

}

int main()
{
	Logger::GetLogger().LogOnFile(true);

	GLRenderer renderer;

	if (glfwInit() == GL_FALSE)
	{
		return -1;
	}


	WindowInfo windowInfo{ 800, 600 };
	GLFWwindow * window = CreateWindow(windowInfo, "Test Window");

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!renderer.Initialize(window))
	{
		glfwTerminate();
		return -1;
	}

	//DUMMY MESH LOADING -- NEEDS SCENE CLASS
	ShaderProgram prg;

	//FIXME : Generating a string for each resource load is TERRIBLE
	std::string vsStr = ResourceManager::GetShadersForlder();

	if (!prg.CompileShader(vsStr += "base.vs", ShaderType::Vertex))
	{
		Logger::GetLogger().LogString("Vertex shader compile failed", ERROR);
	}
	std::string fsStr = ResourceManager::GetShadersForlder();

	if (!prg.CompileShader(fsStr += "base.fs", ShaderType::Fragment))
	{
		Logger::GetLogger().LogString("Fragment shader compile failed", ERROR);
	}

	prg.LinkProgram();

	Texture texture;

	std::string txStr = ResourceManager::GetTexturesFolder();
	texture.LoadFromFile(txStr += "test.jpg");
	//

	Mesh mesh(
	  {  //Vertices
		{glm::vec3(1.f, 1.f, 0.0f), glm::vec4(0, 0, 1, 1), glm::vec2(1,0) }, //0
		{glm::vec3(1.f,  -1.f, 0.0f), glm::vec4(0, 1, 0, 1), glm::vec2(1,1) },  //1
		{glm::vec3(-1.f, 1.f, 0.0f), glm::vec4(1, 0, 0, 1), glm::vec2(0,0) }, //2
		{glm::vec3(-1.f,   -1.f, 0.0f), glm::vec4(0, 0, 1, 1), glm::vec2(0,1) }  //3
	  },
	     //Indices
	  {
		0,
		1,
		2,
		2,
		1,
		3
	  },

	  { &texture, &prg }
	);
	//

	prg.UseProgram();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		renderer.Clear();

		mesh.BindMesh();
		renderer.DrawMesh(mesh);
		mesh.UnbindMesh();

		renderer.Present();
	}

	glfwTerminate();

    return 0;
}
