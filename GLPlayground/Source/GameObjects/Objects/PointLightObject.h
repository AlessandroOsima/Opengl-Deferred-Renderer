#pragma once
#include "GameObjects/Object.h"

class PointLightComponent;
class Transform;

class PointLightObject : public Object
{
public:

	void Start() override;
	void Update(float DeltaTime) override;
	void End() override;

	inline PointLightComponent * GetPointLight()
	{
		return CurrentPointLight;
	}

	PointLightObject(int LightIdx);
	virtual ~PointLightObject();

private:
	PointLightComponent * CurrentPointLight;
	Transform * CurrentTransform = nullptr;
	int LightIdx;
};

