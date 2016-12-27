#include "ShaderProgram.h"
#include <strstream>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Logger.h"

ShaderProgram::ShaderProgram()
{
	ProgramID = glCreateProgram();
}


ShaderProgram::~ShaderProgram()
{
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	glDeleteProgram(ProgramID);
}

bool ShaderProgram::CompileShader(const std::string & ShaderFilename, ShaderType Type)
{

	std::fstream fileStream;

	fileStream.open(ShaderFilename);

	if (!fileStream.is_open())
	{
		std::stringstream stream;
		stream << "Unable to open shader file " << ShaderFilename << std::ends;
		Logger::GetLogger().LogString(stream.str(), LogType::ERROR);

		return false;
	}

	std::stringstream stream;
	stream << fileStream.rdbuf();

	std::string fc((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

	const char * fileContent = fc.c_str();

	GLint compilationSuccess = 0;
	GLuint CompiledID = 0;

	switch (Type)
	{
	case ShaderType::Vertex:
	{
		CompiledID = VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(VertexShaderID, 1, &fileContent, NULL);
		glCompileShader(VertexShaderID);
		break;
	}
	case ShaderType::Fragment:
	{
		CompiledID = FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(FragmentShaderID, 1, &fileContent, NULL);
		glCompileShader(FragmentShaderID);

		break;
	}

	default:
		break;
	}

	glGetShaderiv(CompiledID, GL_COMPILE_STATUS, &compilationSuccess);

	if (!compilationSuccess)
	{
		GLint logSize;
		glGetShaderiv(CompiledID, GL_INFO_LOG_LENGTH, &logSize);

		if (logSize)
		{
			char * logMessage = new char[logSize];

			glGetShaderInfoLog(CompiledID, logSize, NULL, logMessage);

			std::stringstream stream;
			stream << "ERROR: Shader " << CompiledID << " compile failed with log: \n" << logMessage << std::ends;
			Logger::GetLogger().LogString(stream.str(), ERROR);

			delete logMessage;
		}

	}

	if (compilationSuccess)
	{
		glAttachShader(ProgramID, CompiledID);
	}

	return compilationSuccess == 1;
}

void ShaderProgram::LinkProgram()
{
	glLinkProgram(ProgramID);
}

void ShaderProgram::UseProgram()
{
	glUseProgram(ProgramID);
}