#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

layout (binding = 0) uniform Matrices
{
	mat4 Projection;
	mat4 View;
};

uniform mat4 Model;
uniform mat3 Normal;

out vec4 fragmentColor;

void main()
{
  gl_Position = Projection * vec4(position.xyz, 1.0);
  fragmentColor = color;
}