#include "stdafx.h"
#include "Mesh.h"
#include "Managers/ShaderManager.h"
#include "Managers/TextureManager.h"

Mesh::Mesh(std::shared_ptr<VertexData> Data) : VData(Data), MaterialParameters(new EmptytMeshMaterialParameters())
{
	LinkVertexData();
	LinkMaterialData();
}

//Mesh::Mesh(): Model(glm::mat4(1))
//{
//
//}

Mesh::Mesh(Mesh && MeshToReplace) : VData (std::move(MeshToReplace.VData))
{
	Model = std::move(MeshToReplace.Model);
	MaterialData = std::move(MeshToReplace.MaterialData);
	MaterialParameters = std::move(MeshToReplace.MaterialParameters);
}

Mesh::Mesh()
{
	LinkMaterialData();
}

Mesh & Mesh::operator=(Mesh && MeshToReplace)
{
	VData = std::move(MeshToReplace.VData);
	Model = std::move(MeshToReplace.Model);
	MaterialData = std::move(MeshToReplace.MaterialData);
	MaterialParameters = std::move(MeshToReplace.MaterialParameters);

	return *this;
}

Mesh::~Mesh()
{
	MaterialData.RemoveObjects();
}

void Mesh::LinkVertexData()
{
	VData->GenerateVertexData();
}

void Mesh::LinkMaterialData()
{
	MaterialData.CreateObjects();
	MaterialParameters->LinkParameters(this);
}

void Mesh::Bind()
{
	VData->Bind();

	MaterialData.Bind();
	MaterialParameters->BindParameters(this);
	MaterialData.SetUniforms();

}

void Mesh::Unbind()
{
	VData->Unbind();
	MaterialParameters->UnbindParameters(this);
	MaterialData.UnBind();
}

bool Mesh::SetMaterialParameters(std::unique_ptr<BaseMeshMaterialParameters> && NewMaterialParameters)
{
	if (NewMaterialParameters->IsValidForMaterial(MaterialData))
	{
		MaterialParameters = std::move(NewMaterialParameters);
		return true;
	}

	return false;
}

void Mesh::UpdateVertexData()
{
	VData->UpdateVertexData();
}

void Mesh::UpdateIndexData()
{
	VData->UpdateIndexData();
}
