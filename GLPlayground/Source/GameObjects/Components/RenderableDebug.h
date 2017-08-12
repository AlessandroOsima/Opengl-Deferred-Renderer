#pragma once

#include "GameObjects/Component.h"
#include "Renderer/Mesh.h"
#include <memory>
#include "Renderer/RenderableScene.h"
#include "GameObjects/Scene.h"
#include "Renderer/RenderingInfo.h"


class RenderableDebug : public Component
{
public:
	RenderableDebug();
	virtual ~RenderableDebug();

	virtual void Start() override;
	virtual void Update(float DeltaTime) override;
	virtual void LateUpdate(float DeltaTime) override;
	virtual void End() override;

	virtual ComponentsType GetType()
	{
		return ComponentsType::RenderableDebug;
	}

};

