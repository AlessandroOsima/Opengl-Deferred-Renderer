#pragma once
#include <glm/glm.hpp>

//#pragma pack (push, 1)
const int MaxLights = 10;

struct PointLight
{
	glm::vec4 Position = glm::vec4(0,0,0,0);
	glm::vec4 Color = glm::vec4(0, 0, 0, 1);

	inline bool GetEnabled()
	{
		return Position.w > 0;
	}

	//Keeps the enabled flag in w
	inline void SetPosition(glm::vec3 NewPosition)
	{
		Position.x = NewPosition.x;
		Position.y = NewPosition.y;
		Position.z = NewPosition.z;
	}

	inline void SetEnabled(bool Enabled)
	{
		Enabled ? Position.w = 1 : Position.w = -1;
	}
};

struct DirectionalLight
{
	glm::vec4 Direction = glm::vec4(0, 0, 0, 0);
	glm::vec4 Color = glm::vec4(0, 0, 0, 1);

	inline bool GetEnabled()
	{
		return Direction.w > 0;
	}

	inline void SetEnabled(bool Enabled)
	{
		Enabled ? Direction.w = 1 : Direction.w = -1;
	}
};

struct ConeLight
{
	glm::vec4 Position = glm::vec4(0, 0, 0, 0);
	glm::vec4 Direction = glm::vec4(0, 0, 0, 0);
	glm::vec4 Color = glm::vec4(0, 0, 0, 1);

	inline bool GetEnabled()
	{
		return Position.w > 0;
	}

	inline void SetEnabled(bool Enabled)
	{
		Enabled ? Position.w = 1 : Position.w = -1;
	}

	inline float GetAngle()
	{
		return Direction.w;
	}

	inline void SetAngle(float Angle)
	{
		Direction.w = Angle;
	}
};


//struct SceneLights
//{
//	glm::vec4 Ambient;
//
//	PointLight PointLights[MaxLights];
//	DirectionalLight DirectionalLights[MaxLights];
//	ConeLight ConeLights[MaxLights];
//};


struct SceneLights
{
	glm::vec4 Ambient;

	DirectionalLight DirectionalLights[MaxLights];
	PointLight PointLights[MaxLights];
};

//#pragma pack (pop)