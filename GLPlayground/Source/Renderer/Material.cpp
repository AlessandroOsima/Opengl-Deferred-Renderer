#include "stdafx.h"
#include "Material.h"
#include "Managers/ShaderManager.h"
#include "Managers/TextureManager.h"
#include "Renderer/GLUtilities.h"
#include "Logger/Asserts.h"

void Material::Bind()
{
	AssertWithMessage(Program, "Material has an invalid program")

	ShaderManager::GetShaderManager().UseProgram(Program);

	//Sampler i is bound to texture unit i
	for (int i = 0; i < Samplers.size(); i++)
	{
		Samplers[i].Bind(i);
	}
}

void Material::UnBind()
{
	for (int i = 0; i < Samplers.size(); i++)
	{
		Samplers[i].UnBind();
	}

	glUseProgram(0);
}

Material::Material(size_t Program): Program(Program)
{
}

Material::Material(Material && MaterialToReplace) : 
	Program(MaterialToReplace.Program), 
	Uniforms(MaterialToReplace.Uniforms)
{
	MaterialToReplace.Program = 0;
}

Material::Material()
{

}

Material & Material::operator=(Material && MaterialToReplace)
{

	Program = std::move(MaterialToReplace.Program);
	Uniforms = std::move(MaterialToReplace.Uniforms);

	MaterialToReplace.Program = 0;

	return *this;
}

Material::~Material()
{
}

void Material::CreateObjects()
{
}

void Material::RemoveObjects()
{

}

bool Material::ProgramHasUniform(const std::string & UniformName) const
{
	if (Program)
	{
		bool Found = false;
		ShaderProgram & shader = ShaderManager::GetShaderManager().GetShader(Program, Found);

		if (Found)
		{
			if (shader.GetUniformIndex(UniformName) == GL_INVALID_INDEX)
			{
				return false;
			}

			return true;
		}
	}

	return false;
}

bool Material::ProgramHasResource(unsigned int ResourceType, const std::string & ResourceName) const
{
	if (Program)
	{
		bool Found = false;
		ShaderProgram & shader = ShaderManager::GetShaderManager().GetShader(Program, Found);

		if (Found)
		{
			if (shader.GetResourceLocation(ResourceType, ResourceName) == GL_INVALID_INDEX)
			{
				return false;
			}

			return true;
		}
	}

	return false;
}

bool Material::SetUniform(const UniformsToBind & Uniform)
{
	auto it = std::find(Uniforms.begin(), Uniforms.end(), Uniform);

	if (it != Uniforms.end())
	{
		*it = Uniform;
		return true;
	}

	return false;
}

UniformsToBind * Material::GetUniformByName(const std::string & UniformName)
{
	for (auto & uni : Uniforms)
	{
		if (uni.UniformName == UniformName)
		{
			return &uni;
		}
	}

	return nullptr;
}


void Material::AddUniform(const UniformsToBind & Uniform)
{
	auto it = std::find(Uniforms.begin(), Uniforms.end(), Uniform);

	if (it != Uniforms.end())
	{
		*it = Uniform;
	}
	else
	{
		Uniforms.push_back(Uniform);
	}
}

void Material::SetUniforms()
{

	bool Found = false;
	ShaderProgram & shader = ShaderManager::GetShaderManager().GetShader(Program, Found);

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

				case  UniformType::Int:
				{
					shader.SetUniformInt(index, uniform.TypeData.IntVal);
					break;
				}

				default:
				{
#ifdef _DEBUG
					std::stringstream str;
					str << "Uniform type of  " << uniform.UniformName << " not valid on material " << Program << std::ends;
					Logger::GetLogger().LogString(str.str(), LogType::WARNING);
#endif
					break;
				}
				}
			}
			else
			{
#ifdef _DEBUG
				std::stringstream str;
				str << "Uniform  " << uniform.UniformName << " not found on material " << Program << std::ends;
				Logger::GetLogger().LogString(str.str(), LogType::WARNING);
#endif
			}
		}
	}
}

Sampler & Material::AddSampler()
{
	Samplers.push_back(std::move(Sampler()));
	return *(Samplers.end() - 1);
}
