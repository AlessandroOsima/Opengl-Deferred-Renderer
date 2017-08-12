#include "stdafx.h"
#include "PointLightObject.h"
#include "GameObjects/Components/PointLightComponent.h"
#include "GameObjects/Components/Renderable.h"
#include "GameObjects/Components/Transform.h"
#include "Renderer/VertexFactory.h"
#include "Managers/InputManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include "GameObjects/Components/DisComponent.h"


void PointLightObject::Start()
{
	Object::Start();

	CurrentPointLight = AddComponent<PointLightComponent>(std::make_unique<PointLightComponent>());

	CurrentPointLight->Enable(true);

	glm::vec4 lightColor = { 1.0f,1.0f,0.8f,1.0f };

	CurrentPointLight->SetColor(lightColor);

	CurrentTransform = AddComponent<Transform>(std::make_unique<Transform>());

	Renderable* rend = AddComponent<Renderable>(std::make_unique<Renderable>());

	auto mesh = VertexFactory::GenerateCube();
	rend->SetMesh(mesh);

	auto & material = rend->GetMesh()->GetMaterial();
	
	size_t DeferredLightingHash;
	ShaderManager::GetShaderManager().CreateShader("deferredLightingGeometry", "deferredLightingGeometry.vs", "deferredLightingGeometry.fs", DeferredLightingHash);
	material.Program = DeferredLightingHash;

	std::unique_ptr<DeferredBlinnPhongMeshMaterialParameters> materialParams = std::make_unique<DeferredBlinnPhongMeshMaterialParameters>();

	materialParams->Diffuse = lightColor;
	materialParams->Specular = glm::vec4(1);
	materialParams->Shininess = 1;
	materialParams->DiffuseColorType = DiffuseType::Emissive;

	mesh->SetMaterialParameters(std::move(materialParams));
	mesh->LinkMaterialData();

	AddComponent<DisComponent>(std::make_unique<DisComponent>());

	InputManager::GetInputManager() += [&](KeyState keyState, KeyCode keyCode)
	{

		if (!LogicScene)
		{
			return;
		}

		//TODO : this should accumulate a move vector to be applied on update, but its debug code so fuck it

		const float translateAmount = 0.5f;
		if (keyCode == KeyCode::D && keyState == KeyState::PRESSED)
		{
			CurrentTransform->SetTranslate(glm::translate(CurrentTransform->GetTranslate(), glm::vec3(translateAmount, 0.f, 0.f)));
		}

		if (keyCode == KeyCode::A && keyState == KeyState::PRESSED)
		{
			CurrentTransform->SetTranslate(glm::translate(CurrentTransform->GetTranslate(), glm::vec3(-translateAmount, 0.f, 0.f)));
		}

		if (keyCode == KeyCode::W && keyState == KeyState::PRESSED)
		{
			CurrentTransform->SetTranslate(glm::translate(CurrentTransform->GetTranslate(), glm::vec3(0.f, translateAmount, 0.f)));
		}

		if (keyCode == KeyCode::S && keyState == KeyState::PRESSED)
		{
			CurrentTransform->SetTranslate(glm::translate(CurrentTransform->GetTranslate(), glm::vec3(0.f, -translateAmount, 0.f)));
		}

		if (keyCode == KeyCode::Q && keyState == KeyState::PRESSED)
		{
			CurrentTransform->SetTranslate(glm::translate(CurrentTransform->GetTranslate(), glm::vec3(0.f, 0, -translateAmount)));
		}

		if (keyCode == KeyCode::E && keyState == KeyState::PRESSED)
		{
			CurrentTransform->SetTranslate(glm::translate(CurrentTransform->GetTranslate(), glm::vec3(0.f, 0, translateAmount)));
		}
		
		std::stringstream str;

		str << " Pos is " << CurrentPointLight->GetPosition().x << " " << CurrentPointLight->GetPosition().y << " " << CurrentPointLight->GetPosition().z << std::ends;
		Logger::GetLogger().LogString(str.str());

	};
}

void PointLightObject::Update(float DeltaTime)
{
	Object::Update(DeltaTime);

	Renderable * rend = GetComponentOfTypeAs<Renderable>(ComponentsType::Renderable);
	DeferredBlinnPhongMeshMaterialParameters * params = static_cast<DeferredBlinnPhongMeshMaterialParameters *>(rend->GetMesh()->GetMaterialParameters());
	params->Diffuse = GetPointLight()->GetColor();
}

void PointLightObject::End()
{
	Object::End();

}

PointLightObject::PointLightObject(int LightIdx) : LightIdx(LightIdx)
{
}


PointLightObject::~PointLightObject()
{
}
