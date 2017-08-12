#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 uv;

layout (std140, binding = 0) uniform Matrices
{
	mat4 Projection;
	mat4 View;
};

uniform mat4 Model;
uniform mat3 Normal;

out vec3 pixelNormal;
out vec4 fragmentColor;
out vec2 uvCoords;
out vec3 eyeVec;

void main()
{
  gl_Position = Projection * Model * vec4(position.xyz, 1.0);
  pixelNormal = normalize(Normal * normal);
  uvCoords = uv;
  eyeVec = normalize((Model * vec4(position.xyz,0)).xyz);
  fragmentColor = color;
}