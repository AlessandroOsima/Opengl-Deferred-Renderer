#include "stdafx.h"
#include "glm/gtc/random.hpp"
#include "Renderer/MeshMaterialParameters.h"
#include "GameObjects/Components/Renderable.h"
#include "Managers/InputManager.h"
#include "GameObjects/Components/PointLightComponent.h"
#include "DisComponent.h"


void DisComponent::Start()
{
	InputManager::GetInputManager() += [&](KeyState keyState, KeyCode keyCode)
	{
		if (keyCode == KeyCode::Enter && keyState == KeyState::PRESSED)
		{
			Change ? Change = false : Change = true;

			if (!Change)
			{
				DeltaAccumulator =  0;
			}
		}

		if (keyCode == KeyCode::Up && keyState == KeyState::PRESSED)
		{
			Limit += 0.1f;
		}
        
		if (keyCode == KeyCode::Down && keyState == KeyState::PRESSED)
		{
			Limit -= 0.1f;
		}
	};
}

void DisComponent::Update(float DeltaTime)
{

	DeltaAccumulator += DeltaTime; 

	if (Change && DeltaAccumulator > Limit)
	{
		PointLightComponent * PointLigth = Owner->GetComponentOfTypeAs<PointLightComponent>(ComponentsType::Light);

		if (PointLigth)
		{
			PointLigth->SetColor(glm::linearRand(glm::vec4(0, 0, 0, 1), glm::vec4(1, 1, 1, 1)));
		}
		else
		{
			Renderable * rend = Owner->GetComponentOfTypeAs<Renderable>(ComponentsType::Renderable);

			DeferredBlinnPhongMeshMaterialParameters * parms = static_cast<DeferredBlinnPhongMeshMaterialParameters*>(rend->GetMesh()->GetMaterialParameters());

			parms->Diffuse = glm::linearRand(glm::vec4(0, 0, 0, 1), glm::vec4(1, 1, 1, 1));
		}


		DeltaAccumulator = 0;
	}
}

void DisComponent::LateUpdate(float DeltaTime)
{

}

void DisComponent::End()
{

}
