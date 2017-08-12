#version 450 core

layout (location = 0) out vec4 color;


layout(binding=0) uniform sampler2D positionSampler;
layout(binding=1) uniform sampler2D normalSampler;
layout(binding=2) uniform sampler2D diffuseSampler;
layout(binding=3) uniform sampler2D specularSampler;
layout(binding=4) uniform sampler2D ambientSampler;

in vec4 fragmentColor;
in vec2 uvCoords;


struct directionalLight
{
	vec4 direction;
	vec4 color;
};

struct pointLight
{
	vec4 position;
	vec4 color;
};

const int maxLightSize = 10;
layout (std140, binding = 5) uniform lights
{
	vec4 ambientLight;
	directionalLight directionalLights[maxLightSize];
	pointLight pointLights[maxLightSize];
};

uniform vec3 pv;

const float PI = 3.1415926535897932384626433832795;

vec4 basePhong(vec3 v, vec3 n, vec4 diffuse, float type, vec4 specular, float shininess, vec4 ambientMask)
{

	if(type == 1) //material is emissive, no need to compute lights
	{
		return vec4(diffuse);
	}

	vec4 totalScattered = ambientLight * ambientMask.a;

	for(int i = 0; i < maxLightSize; i++)
	{
		if(directionalLights[i].direction.w > 0) //if enabled
	    {
	    	float diffuseFactor = max(0, dot(n, -directionalLights[i].direction.xyz)); 

			vec4 specularFactor = vec4(0,0,0,1);
			if(diffuseFactor > 0.0f)
			{
				//reflect works with incident light directionalLights[i].direction.xyz must not be inverted
				vec3 reflectDir = reflect(directionalLights[i].direction.xyz,  n);

				float specularIntensity = max(dot(reflectDir, normalize(v)), 0.0);
				specularFactor = pow(specularIntensity,shininess) * specular;
			}

	    	totalScattered += specularFactor + (diffuseFactor * diffuse)* directionalLights[i].color;
	    }
	}
	
	return totalScattered;
}

//BLINN-PHONG (real time rendering p257 eq 7.48) => cdiff / pi + (m+8/8*pi) * spec * (cos(h)^m)
vec4 blinnPhong(vec3 v, vec3 vp, vec3 n, vec4 diffuse, float type, vec4 specular, float shininess, vec4 ambientMask)
{
	
	if(type == 1) //material is emissive, no need to compute lights
	{
		return vec4(diffuse);
	}

	vec4 totalScattered = ambientLight * ambientMask.a;

	for(int i = 0; i < maxLightSize; i++)
	{
		if(directionalLights[i].direction.w > 0) //if enabled
	    {
			vec3 h = normalize(-directionalLights[i].direction.xyz + v);
			float thetaH =  clamp(dot(n,h),0,1);

	    	float diffuseFactor = clamp( dot(n, -directionalLights[i].direction.xyz), 0, 1); 
			vec4 specularFactor =  (shininess + 8) / (8 * PI)  * specular * pow(thetaH,shininess) ;

	    	totalScattered += (specularFactor + ( diffuse / PI)) * diffuseFactor * directionalLights[i].color;
	    }

		if(pointLights[i].position.w > 0) //if enabled
	    {
			vec3 l = normalize(pointLights[i].position.xyz - vp); 
			vec3 h = normalize(v + l);
			float thetaH =  clamp(dot(n,h),0,1);

	    	float diffuseFactor = clamp( dot(n, l), 0, 1); 
			vec4 specularFactor =  (shininess + 8) / (8 * PI)  * specular * pow(thetaH,shininess) ;

	    	totalScattered += (specularFactor + ( diffuse / PI)) * diffuseFactor * pointLights[i].color;
	    	//totalScattered += specularFactor * ambientMask;//diffuseFactor * pointLights[i].color;

		}
	}
	
	return totalScattered;
}

void main()
{
    vec3 vertexPosition = texture(positionSampler, uvCoords).xyz;
	vec3 pixelNormal = texture(normalSampler, uvCoords).xyz;
	vec4 materialDiffuse = texture(diffuseSampler, uvCoords);
	vec4 materialSpecular = texture(specularSampler, uvCoords);
	vec4 ambientMask = texture(ambientSampler, uvCoords);
	float materialShininess = ambientMask.r;
	float type = ambientMask.g;

	color =  blinnPhong(normalize(pv - vertexPosition), vertexPosition, pixelNormal, materialDiffuse, type, materialSpecular, materialShininess, ambientMask);
}


/*
			if(reflectDir.y != 0)
				reflectDir.y = (reflectDir.y + 1) / 2;

			if(reflectDir.x != 0)
				reflectDir.x = (reflectDir.x + 1) / 2;

			if(reflectDir.z != 0)
				reflectDir.z = (reflectDir.z + 1) / 2;
*/