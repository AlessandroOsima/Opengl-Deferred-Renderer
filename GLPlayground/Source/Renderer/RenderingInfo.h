#pragma once
#include <vector>
#include <memory>
#include "Managers/ShaderManager.h"
#include "Managers/TextureManager.h"
#include "Renderer/Material.h"
#include "Renderer/Uniform.h"

class RenderingInfo
{

public:
	RenderingInfo();
	RenderingInfo(std::shared_ptr<Material> Material);
	RenderingInfo(const RenderingInfo & RP);
	RenderingInfo(RenderingInfo && RenderPassToReplace);
	void operator=(RenderingInfo && RenderPassToReplace);
	~RenderingInfo();

	inline std::shared_ptr<Material> GetMaterial()
	{
		return CurrentMaterial;
	}

	inline size_t AddUniform(const UniformsToBind & Uniform)
	{
		Uniforms.push_back(Uniform);
		return Uniforms.size() - 1;
	}

	inline UniformsToBind * GetUniform(unsigned int Index)
	{
		if (Index < Uniforms.size())
		{
			return &Uniforms[Index];
		}

		return nullptr;
	}

	void SetUniforms();

private:
	std::vector<UniformsToBind> Uniforms;
	std::shared_ptr<Material> CurrentMaterial;
};
