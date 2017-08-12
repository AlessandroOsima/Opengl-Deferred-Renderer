#pragma once
#include <glm/glm.hpp>
#include <string>

union UniformTypeData
{
	glm::mat3 mat3Val;
	glm::mat4 mat4Val;
	glm::vec4 vec4Val;
	glm::vec3 vec3Val;
	float floatVal;
	int IntVal;
	UniformTypeData()
	{
		memset(this, 0, sizeof(*this));
	}
};

enum class UniformType
{
	Mat4,
	Mat3,
	Vec4,
	Vec3,
	Float,
	Int
};

struct UniformsToBind
{
	
	std::string UniformName;
	UniformTypeData TypeData;
	UniformType Type;

	bool operator==(const UniformsToBind & Other)
	{
		if (UniformName == Other.UniformName && Type == Other.Type)
		{
			return true;
		}

		return false;
	}
};

