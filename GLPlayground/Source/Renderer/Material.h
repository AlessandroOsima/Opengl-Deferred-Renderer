#pragma once
#include <GL/glew.h>
#include <list>
#include "Renderer/Uniform.h"
#include "Renderer/Texture.h"
#include "Renderer/Sampler.h"

using UniformIt = std::list<UniformsToBind>::iterator;

enum class DiffuseType : int
{
	Diffuse,
	Emissive
};

class Material
{
public:
	size_t Program = 0;

	void Bind();
	void UnBind();


	Material(size_t Program);
	Material(Material && MaterialToReplace);
	Material  & operator=(Material && MaterialToReplace);

	Material();
	virtual ~Material();

	void CreateObjects();
	void RemoveObjects();

	bool ProgramHasUniform(const std::string & UniformName) const;
	bool ProgramHasResource(unsigned int ResourceType, const std::string & ResourceName) const;

	bool SetUniform(const UniformsToBind & Uniform);
	UniformsToBind * GetUniformByName(const std::string & UniformName);
	void AddUniform(const UniformsToBind & Uniform);

	inline void DeleteUniform(UniformIt  Uniform)
	{
		Uniforms.erase(Uniform);
	}

	void SetUniforms();

	//Generate a new sampler with an idx (its position in the vector), idx i corresponds to texture unit i
	Sampler & AddSampler();

private:

	std::list<UniformsToBind> Uniforms;
	std::vector<Sampler> Samplers;

};

