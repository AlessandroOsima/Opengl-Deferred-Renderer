#include "stdafx.h"
#include "RenderableDebug.h"
#include "GameObjects/Components/Renderable.h"
#include "GameObjects/Components/Transform.h"


RenderableDebug::RenderableDebug()
{
}


RenderableDebug::~RenderableDebug()
{
}

void RenderableDebug::Start()
{

}

void RenderableDebug::Update(float DeltaTime)
{

}

void RenderableDebug::LateUpdate(float DeltaTime)
{
	assert(Owner);

	Renderable * renderable = Owner->GetComponentOfTypeAs<Renderable>(ComponentsType::Renderable);
	Transform * transform = Owner->GetComponentOfTypeAs<Transform>(ComponentsType::Transform);

	if (renderable && transform)
	{
		if (LogicScene && renderable->HasMesh() && renderable->IsRendering())
		{
			auto mesh = renderable->GetMesh();

			for (auto & verts : mesh->GetVertices())
			{
				glm::vec3 worldVert = transform->GetTranslate() * transform->GetScale() * transform->GetRotate() * glm::vec4(verts.Position, 1);

				LogicScene->GetRenderScene().GetPrimitiveDrawer().DrawLine(worldVert, worldVert + (mesh->GetNormal() * glm::normalize(verts.Normal) * 1.f), { 0,0,1,1 }, { 1,0,0,1 });
			}
		}
	}
	else
	{
		Logger::GetLogger().LogString("[RenderableDebug]No transform or renderable component found in GameObject", LogType::ERROR);
	}
}

void RenderableDebug::End()
{

}
