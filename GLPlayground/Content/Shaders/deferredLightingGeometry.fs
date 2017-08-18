#version 450 core

layout (location = 0) out vec4 positionPixel;
layout (location = 1) out vec4 normalPixel;
layout (location = 2) out vec4 diffusePixel;
layout (location = 3) out vec4 specularPixel;
layout (location = 4) out vec4 ambientPixel;

//layout (binding = 0) uniform sampler2D diffuseSampler;

in vec3 pixelNormal;
in vec4 fragmentColor;
in vec2 uvCoords;
in vec3 eyeVec;


struct directionalLight
{
	vec4 direction;
	vec4 color;
};

const int maxLightSize = 10;
layout (std140, binding = 2) uniform lights
{
	vec4 ambientLight;
	directionalLight directionalLights[maxLightSize];
};

struct Material
{
	vec4 Diffuse;
	vec4 Specular;
	float Shininess;
	int DiffuseType;
};

uniform Material material;

void main()
{
	positionPixel = vec4(eyeVec,1);
	normalPixel = vec4(normalize(pixelNormal),1);
	diffusePixel = material.Diffuse;
	specularPixel = vec4(material.Specular.xyz, 1);
	ambientPixel = vec4(material.Shininess, material.DiffuseType,0,1);
}