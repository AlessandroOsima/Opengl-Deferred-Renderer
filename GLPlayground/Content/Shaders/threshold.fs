#version 450 core

layout (location = 0) out vec4 color;
layout (binding = 0) uniform sampler2D diffuseSampler;

in vec4 fragmentColor;
in float thresholdOut;
in vec2 uvCoords;

void main()
{
	vec4 colorSample = texture(diffuseSampler, uvCoords).rgba;
	
	vec4 thresholdValue = vec4(0,0,0,1);

	if(colorSample.r >= thresholdOut)
	{
	    thresholdValue = vec4(1,1,1,1);
	}

	color = thresholdValue;
	
}