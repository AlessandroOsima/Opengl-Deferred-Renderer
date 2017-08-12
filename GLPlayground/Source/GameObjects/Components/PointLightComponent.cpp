#include "stdafx.h"
#include "PointLightComponent.h"
#include "GameObjects/Components/Transform.h"
#include <glm/gtx/matrix_decompose.hpp>

PointLightComponent::PointLightComponent()
{
}


PointLightComponent::~PointLightComponent()
{
}

void PointLightComponent::Start()
{

}

void PointLightComponent::Update(float DeltaTime)
{
}

void PointLightComponent::LateUpdate(float DeltaTime)
{
	assert(Owner);

	Component * cmp = Owner->GetComponentOfType(ComponentsType::Transform);

	if (cmp)
	{
		Transform * tr = static_cast<Transform *>(cmp);

		glm::vec3 translation = glm::vec3(tr->GetTranslate()[3]);

		SetPosition(translation);

	}
	else
	{
		Logger::GetLogger().LogString("No transform component found in GameObject", LogType::ERROR);
	}
}

void PointLightComponent::End()
{

}

void PointLightComponent::SetPosition(glm::vec3 Position)
{
	AssertWithMessage(LogicScene, "No Logic Scene Mapped to component");

	auto & RenderScene = LogicScene->GetRenderScene();

	RenderScene.Lights.PointLights[MappedLightIdx].SetPosition(Position);
}

glm::vec3 PointLightComponent::GetPosition()
{
	AssertWithMessage(LogicScene, "No Logic Scene Mapped to component");

	auto & RenderScene = LogicScene->GetRenderScene();

	return RenderScene.Lights.PointLights[MappedLightIdx].Position;
}

void PointLightComponent::Enable(bool Enable)
{
	AssertWithMessage(LogicScene, "No Logic Scene Mapped to component");

	auto & RenderScene = LogicScene->GetRenderScene();

	RenderScene.Lights.PointLights[MappedLightIdx].SetEnabled(Enable);
}

bool PointLightComponent::IsEnabled()
{
	AssertWithMessage(LogicScene, "No Logic Scene Mapped to component");

	auto & RenderScene = LogicScene->GetRenderScene();

	return RenderScene.Lights.PointLights[MappedLightIdx].GetEnabled();
}

void PointLightComponent::SetColor(glm::vec4 Color)
{
	AssertWithMessage(LogicScene, "No Logic Scene Mapped to component");

	auto & RenderScene = LogicScene->GetRenderScene();

	RenderScene.Lights.PointLights[MappedLightIdx].Color = Color;
}

glm::vec4 PointLightComponent::GetColor()
{
	AssertWithMessage(LogicScene, "No Logic Scene Mapped to component");

	auto & RenderScene = LogicScene->GetRenderScene();

	return RenderScene.Lights.PointLights[MappedLightIdx].Color;
}

void PointLightComponent::MapTo(int LightIdx)
{
	AssertWithMessage(LogicScene, "No Logic Scene Mapped to component");

	auto & RenderScene = LogicScene->GetRenderScene();

	AssertWithMessage(LightIdx < MaxLights, "New Light idx is more that max lights");

	MappedLightIdx = LightIdx;
}

unsigned PointLightComponent::MappedTo()
{
	return MappedLightIdx;
}


