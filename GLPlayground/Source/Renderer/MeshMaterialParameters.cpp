#include "stdafx.h"
#include "MeshMaterialParameters.h"
#include "Renderer/Mesh.h"


BaseMeshMaterialParameters::~BaseMeshMaterialParameters()
{
	//blabla
}

bool EmptytMeshMaterialParameters::LinkParameters(Mesh * CurrentMesh)
{
	AssertWithMessage(CurrentMesh, "Not a valid mesh bound to this parameters");

	return true;
}


bool EmptytMeshMaterialParameters::BindParameters(Mesh * CurrentMesh)
{
	AssertWithMessage(CurrentMesh, "Not a valid mesh bound to this parameters");

	return true;
}

bool EmptytMeshMaterialParameters::UnbindParameters(Mesh * CurrentMesh)
{
	AssertWithMessage(CurrentMesh, "Not a valid mesh bound to this parameters");

	return true;

}

bool EmptytMeshMaterialParameters::UnlinkParameters(Mesh * CurrentMesh)
{
	AssertWithMessage(CurrentMesh, "Not a valid mesh bound to this parameters");

	return true;
}

bool EmptytMeshMaterialParameters::IsValidForMaterial(const Material & CurrentMaterial)
{
	return true;
}


bool DefaultMeshMaterialParameters::LinkParameters(Mesh * CurrentMesh)
{
	AssertWithMessage(CurrentMesh, "Not a valid mesh bound to this parameters");

	UniformTypeData NormalData;
	NormalData.mat3Val = CurrentMesh->GetNormal();
	CurrentMesh->GetMaterial().AddUniform({ "Normal", NormalData , UniformType::Mat3 });

	UniformTypeData ModelData;
	ModelData.mat4Val = CurrentMesh->GetModel();
	CurrentMesh->GetMaterial().AddUniform({ "Model", ModelData , UniformType::Mat4 });

	return true;
}


bool DefaultMeshMaterialParameters::BindParameters(Mesh * CurrentMesh)
{
	AssertWithMessage(CurrentMesh, "Not a valid mesh bound to this parameters");

	auto NormalUniform = CurrentMesh->GetMaterial().GetUniformByName("Normal");
	AssertWithMessage(NormalUniform, "Have you linked the material data in the mesh ?");
	NormalUniform->TypeData.mat3Val = CurrentMesh->GetNormal();

	auto ModelUniform = CurrentMesh->GetMaterial().GetUniformByName("Model");
	AssertWithMessage(ModelUniform, "Have you linked the material data in the mesh ?");
	ModelUniform->TypeData.mat4Val = CurrentMesh->GetModel();

	return true;
}

bool DefaultMeshMaterialParameters::UnbindParameters(Mesh * CurrentMesh)
{
	AssertWithMessage(CurrentMesh, "Not a valid mesh bound to this parameters");

	return true;

}

bool DefaultMeshMaterialParameters::UnlinkParameters(Mesh * CurrentMesh)
{
	AssertWithMessage(CurrentMesh, "Not a valid mesh bound to this parameters");

	return true;
}

bool DefaultMeshMaterialParameters::IsValidForMaterial(const Material & CurrentMaterial)
{

	if (CurrentMaterial.Program)
	{
		if (CurrentMaterial.ProgramHasResource(GL_UNIFORM, "Normal") && CurrentMaterial.ProgramHasResource(GL_UNIFORM, "Model"))
		{
			return true;
		}
	}

	Logger::GetLogger().LogString("Material parameters are not valid for material", LogType::ERROR);

	return false;
}

bool DeferredBlinnPhongMeshMaterialParameters::LinkParameters(Mesh * CurrentMesh)
{
	AssertWithMessage(CurrentMesh, "Not a valid mesh bound to this parameters");

	UniformTypeData NormalData;
	NormalData.mat3Val = CurrentMesh->GetNormal();
	CurrentMesh->GetMaterial().AddUniform({ "Normal", NormalData , UniformType::Mat3 });

	UniformTypeData ModelData;
	ModelData.mat4Val = CurrentMesh->GetModel();
	CurrentMesh->GetMaterial().AddUniform({ "Model", ModelData , UniformType::Mat4 });

	UniformTypeData DiffuseData;
	DiffuseData.vec4Val = Diffuse;
	CurrentMesh->GetMaterial().AddUniform({ "material.Diffuse", DiffuseData, UniformType::Vec4 });

	UniformTypeData SpecularData;
	SpecularData.vec4Val = Specular;
	CurrentMesh->GetMaterial().AddUniform({ "material.Specular", SpecularData, UniformType::Vec4 });

	UniformTypeData ShininessData;
	ShininessData.floatVal = Shininess;
	CurrentMesh->GetMaterial().AddUniform({ "material.Shininess", ShininessData, UniformType::Float });

	UniformTypeData ColorTypeData;
	ColorTypeData.floatVal = static_cast<int>(DiffuseColorType);
	CurrentMesh->GetMaterial().AddUniform({ "material.DiffuseType", ColorTypeData, UniformType::Float });

	return true;
}

bool DeferredBlinnPhongMeshMaterialParameters::BindParameters(Mesh * CurrentMesh)
{
	AssertWithMessage(CurrentMesh, "Not a valid mesh bound to this parameters");

	auto NormalUniform = CurrentMesh->GetMaterial().GetUniformByName("Normal");
	AssertWithMessage(NormalUniform, "Have you linked the material data in the mesh ?");
	NormalUniform->TypeData.mat3Val = CurrentMesh->GetNormal();

	auto ModelUniform = CurrentMesh->GetMaterial().GetUniformByName("Model");
	AssertWithMessage(ModelUniform, "Have you linked the material data in the mesh ?");
	ModelUniform->TypeData.mat4Val = CurrentMesh->GetModel();

	auto DiffuseUniform = CurrentMesh->GetMaterial().GetUniformByName("material.Diffuse");
	AssertWithMessage(DiffuseUniform, "Have you linked the material data in the mesh ?");
	DiffuseUniform->TypeData.vec4Val = Diffuse;

	auto SpecularUniform = CurrentMesh->GetMaterial().GetUniformByName("material.Specular");
	AssertWithMessage(SpecularUniform, "Have you linked the material data in the mesh ?");
	SpecularUniform->TypeData.vec4Val = Specular;

	auto ShininessUniform = CurrentMesh->GetMaterial().GetUniformByName("material.Shininess");
	AssertWithMessage(ShininessUniform, "Have you linked the material data in the mesh ?");
	ShininessUniform->TypeData.floatVal = Shininess;

	auto ColorTypeDataUniform = CurrentMesh->GetMaterial().GetUniformByName("material.DiffuseType");
	AssertWithMessage(ColorTypeDataUniform, "Have you linked the material data in the mesh ?");
	ColorTypeDataUniform->TypeData.floatVal = static_cast<int>(DiffuseColorType);

	return true;
}

bool DeferredBlinnPhongMeshMaterialParameters::UnbindParameters(Mesh * CurrentMesh)
{
	AssertWithMessage(CurrentMesh, "Not a valid mesh bound to this parameters");

	return true;
}

bool DeferredBlinnPhongMeshMaterialParameters::UnlinkParameters(Mesh * CurrentMesh)
{
	AssertWithMessage(CurrentMesh, "Not a valid mesh bound to this parameters");

	return true;
}

bool DeferredBlinnPhongMeshMaterialParameters::IsValidForMaterial(const Material & CurrentMaterial)
{
	if (CurrentMaterial.Program)
	{
		if (CurrentMaterial.ProgramHasResource(GL_UNIFORM, "Normal") && 
			CurrentMaterial.ProgramHasResource(GL_UNIFORM, "Model") &&
			CurrentMaterial.ProgramHasResource(GL_UNIFORM, "material.Diffuse") &&
			CurrentMaterial.ProgramHasResource(GL_UNIFORM, "material.Specular") &&
			CurrentMaterial.ProgramHasResource(GL_UNIFORM, "material.Shininess") &&
			CurrentMaterial.ProgramHasResource(GL_UNIFORM, "material.DiffuseType")
			)
		{
			return true;
		}
	}

	Logger::GetLogger().LogString("Material parameters are not valid for material", LogType::ERROR);

	return false;
}

