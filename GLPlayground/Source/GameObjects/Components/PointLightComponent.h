#pragma once
#include "GameObjects/Component.h"
#include "Renderer/RenderableScene.h"
#include "GameObjects/Scene.h"

class PointLightComponent : public Component
{

public:
	PointLightComponent();
	virtual ~PointLightComponent();

	virtual ComponentsType GetType()
	{
		return ComponentsType::Light;
	}

	virtual void Start() override;
	virtual void Update(float DeltaTime) override;
	virtual void LateUpdate(float DeltaTime) override;
	virtual void End() override;

	void SetPosition(glm::vec3 Position);
	glm::vec3 GetPosition();

	void Enable(bool Enable);
	bool IsEnabled();

	void SetColor(glm::vec4 Color);
	glm::vec4 GetColor();

	void MapTo(int LightIdx);
	unsigned MappedTo();

private:

	unsigned MappedLightIdx = 0;
	bool LightEnabled = false;
};

