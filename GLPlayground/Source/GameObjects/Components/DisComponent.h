#pragma once
#include "GameObjects/Component.h"

class DisComponent : public Component
{
public:

	virtual void Start() override;
	virtual void Update(float DeltaTime) override;
	virtual void LateUpdate(float DeltaTime) override;
	virtual void End() override;

	virtual ComponentsType GetType()
	{
		return ComponentsType::DisComponent;
	}
private :
	bool Change = false;
	float DeltaAccumulator = 0;
	float Limit = 1;

};



