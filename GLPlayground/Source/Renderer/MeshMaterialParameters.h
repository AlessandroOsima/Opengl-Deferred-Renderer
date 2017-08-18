#pragma once
#include "Renderer/Material.h"

class Mesh;

//This class sets all the variable uniforms and other parameters for a mesh
class BaseMeshMaterialParameters
{
public:

	virtual bool LinkParameters(Mesh * CurrentMesh) = 0;
	virtual bool BindParameters(Mesh * CurrentMesh) = 0;
	virtual bool UnbindParameters(Mesh * CurrentMesh) = 0;
	virtual bool UnlinkParameters(Mesh * CurrentMesh) = 0;

	virtual bool IsValidForMaterial(const Material & CurrentMaterial) = 0;

	virtual ~BaseMeshMaterialParameters();
};

//For mesh without material parameters (aka some debug shaders)
class EmptytMeshMaterialParameters : public BaseMeshMaterialParameters
{
public:

	virtual bool LinkParameters(Mesh * CurrentMesh);
	virtual bool BindParameters(Mesh * CurrentMesh);
	virtual bool UnbindParameters(Mesh * CurrentMesh);
	virtual bool UnlinkParameters(Mesh * CurrentMesh);

	virtual bool IsValidForMaterial(const Material & CurrentMaterial);
};

//For Base shaders with simple Normal and Model matrices uniforms
class DefaultMeshMaterialParameters : public BaseMeshMaterialParameters
{
public:

	virtual bool LinkParameters(Mesh * CurrentMesh);
	virtual bool BindParameters(Mesh * CurrentMesh);
	virtual bool UnbindParameters(Mesh * CurrentMesh);
	virtual bool UnlinkParameters(Mesh * CurrentMesh);

	virtual bool IsValidForMaterial(const Material & CurrentMaterial);
};

//Standard shader for the geometry pass of the blinn-phong BRDF
class DeferredBlinnPhongMeshMaterialParameters : public BaseMeshMaterialParameters
{

public:

	//If we have a valid texture use it instead of the diffuse color 
	glm::vec4 Diffuse = glm::vec4(0.95f, 0.95f, 0.0f, 1.0f);
	size_t DiffuseTexture = Texture::INVALID_ID;


	glm::vec4 Specular = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);

	float Shininess = 300;

	DiffuseType DiffuseColorType = DiffuseType::Diffuse;

	virtual bool LinkParameters(Mesh * CurrentMesh);
	virtual bool BindParameters(Mesh * CurrentMesh);
	virtual bool UnbindParameters(Mesh * CurrentMesh);
	virtual bool UnlinkParameters(Mesh * CurrentMesh);

	virtual bool IsValidForMaterial(const Material & CurrentMaterial);

};