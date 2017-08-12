#include "stdafx.h"
#include "Renderable.h"
#include "Logger/Logger.h"
#include "GameObjects/Components/Transform.h"
#include "glm/gtc/random.hpp"

Renderable::Renderable()
{
}


Renderable::~Renderable()
{
}

void Renderable::Start()
{

}

void Renderable::Update(float DeltaTime)
{

}

void Renderable::LateUpdate(float DeltaTime)
{
	assert(Owner);

	Component * cmp = Owner->GetComponentOfType(ComponentsType::Transform);

	if (cmp)
	{
		Transform * tr = static_cast<Transform *>(cmp);

		Mesh->SetModel(tr->GetWorld());

	}
	else
	{
		Logger::GetLogger().LogString("No transform component found in GameObject", LogType::ERROR);
	}
}

void Renderable::End()
{
	if (LogicScene && MeshIsRendering)
	{
		LogicScene->GetRenderScene().RemoveMesh(Loc);
	}
}

