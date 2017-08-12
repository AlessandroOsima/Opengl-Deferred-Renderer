#version 450 core

layout (location = 0) out vec4 color;
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
};

uniform Material material;

void main()
{
	vec4 totalScattered = ambientLight;

	for(int i = 0; i < maxLightSize; i++)
	{
		if(directionalLights[i].direction.w > 0) //if enabled
	    {
	    	float diffuse = max(0, dot(pixelNormal, -directionalLights[i].direction.xyz)); 

			vec4 specular = vec4(0,0,0,1);
			if(diffuse > 0.0f)
			{
				//reflect works with incident light directionalLights[i].direction.xyz must not be inverted
				vec3 reflectDir = reflect(directionalLights[i].direction.xyz,  pixelNormal);

				float specularIntensity = max(dot(reflectDir, eyeVec), 0.0);
				specular = pow(specularIntensity,material.Shininess) * material.Specular;
			}

	    	totalScattered += specular + (diffuse * material.Diffuse)* directionalLights[i].color;
	    }
	}
	
	color = fragmentColor * totalScattered;
}

/*
			if(reflectDir.y != 0)
				reflectDir.y = (reflectDir.y + 1) / 2;

			if(reflectDir.x != 0)
				reflectDir.x = (reflectDir.x + 1) / 2;

			if(reflectDir.z != 0)
				reflectDir.z = (reflectDir.z + 1) / 2;
*/