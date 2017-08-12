#pragma once
#include <GL/glew.h>
#include "Math/MathTypes.h"
#include <vector>
#include "Renderer/Texture.h"
#include "Renderer/ShaderProgram.h"
#include <memory>
#include "Renderer/Material.h"
#include "Renderer/Uniform.h"
#include "Renderer/VertexData.h"
#include "Renderer/RenderingInfo.h"
#include "MeshMaterialParameters.h"

using Index = uint32_t;

class Mesh
{
public:
	Mesh(std::shared_ptr<VertexData> Data);
	Mesh();

	Mesh(Mesh && MeshToReplace);
	Mesh & operator=(Mesh && MeshToReplace);

	~Mesh();

	void LinkVertexData();
	void LinkMaterialData();

	void Bind();
	void Unbind();

	inline void SetVertexData(std::shared_ptr<VertexData> NewVertexData)
	{
		this->VData = NewVertexData;
	}

	inline std::vector<Vertex> & GetVertices()
	{
		return VData->GetVertices();
	}

	inline std::vector<Index> & GetIndices()
	{
		return VData->GetIndices();
	}


	inline void SetModel(const glm::mat4 NewModel)
	{
		Model = NewModel;
	}

	inline glm::mat4 GetModel() const
	{
		return Model;
	}

	inline glm::mat3 GetNormal() const
	{
		//matemagically removes translation and non uniform scaling
		//http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
		glm::mat3 norm(glm::transpose(glm::inverse(Model)));
		return norm;
	}

	inline Material & GetMaterial()
	{
		return MaterialData;
	}

	inline bool SetMaterial(Material && NewMaterialData)
	{
		if (MaterialParameters->IsValidForMaterial(NewMaterialData))
		{
			MaterialData = std::move(NewMaterialData);
			LinkMaterialData();
			return true;
		}
		
		return false;
	}

	inline bool SetMaterialAndParameters(Material && NewMaterialData, std::unique_ptr<BaseMeshMaterialParameters> && NewMaterialParameters)
	{

			MaterialData = std::move(NewMaterialData);

			MaterialParameters = std::move(NewMaterialParameters);

			LinkMaterialData();
			return true;
	}

	inline BaseMeshMaterialParameters * GetMaterialParameters()
	{
		return MaterialParameters.get();
	}

	bool SetMaterialParameters(std::unique_ptr<BaseMeshMaterialParameters> && NewMaterialParameters);


	void UpdateVertexData();
	void UpdateIndexData();

private:

	glm::mat4 Model;
	std::shared_ptr<VertexData> VData;
	Material MaterialData;

	UniformIt NormalUniform;
	UniformIt ModelUniform;
	std::unique_ptr<BaseMeshMaterialParameters> MaterialParameters;
};

