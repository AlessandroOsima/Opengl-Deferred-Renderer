#include "stdafx.h"
#include "RenderingInfo.h"
#include <sstream>

RenderingInfo::RenderingInfo(std::shared_ptr<Material> Material) :
	CurrentMaterial(Material)
{
}


RenderingInfo::RenderingInfo(const RenderingInfo & RP)
{
	assert(0);
}

RenderingInfo::RenderingInfo(RenderingInfo && RenderPassToReplace)
{

	Uniforms = std::move(RenderPassToReplace.Uniforms);
	CurrentMaterial = std::move(RenderPassToReplace.CurrentMaterial);
}

RenderingInfo::RenderingInfo()
{

}

void RenderingInfo::operator=(RenderingInfo && RenderPassToReplace)
{

	Uniforms = std::move(RenderPassToReplace.Uniforms);
	CurrentMaterial = std::move(RenderPassToReplace.CurrentMaterial);
}

RenderingInfo::~RenderingInfo()
{
}

void RenderingInfo::SetUniforms()
{

	bool Found = false;
	ShaderProgram & shader = ShaderManager::GetShaderManager().GetShader(CurrentMaterial->Program, Found);

	if (Found)
	{
		for (UniformsToBind & uniform : Uniforms)
		{
			unsigned int index = shader.GetUniformIndex(uniform.UniformName);

			if (index != GL_INVALID_INDEX)
			{
				switch (uniform.Type)
				{
				case  UniformType::Mat3:
				{
					shader.SetUniformMatrix3(index, uniform.TypeData.mat3Val);
					break;
				}

				case  UniformType::Mat4:
				{
					shader.SetUniformMatrix4(index, uniform.TypeData.mat4Val);
					break;
				}

				case  UniformType::Vec3:
				{
					shader.SetUniformVector3(index, uniform.TypeData.vec3Val);
					break;
				}
				case  UniformType::Vec4:
				{
					shader.SetUniformVector4(index, uniform.TypeData.vec4Val);
					break;
				}

				case  UniformType::Float:
				{
					shader.SetUniformFloat(index, uniform.TypeData.floatVal);
					break;
				}

				default:
				{
					break;
				}
				}
			}
		}
	}
}