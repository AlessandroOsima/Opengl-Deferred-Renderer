#pragma once
#include "GameObjects/Object.h"
class TexturedObject :
	public Object
{
public:
	TexturedObject(const std::string & Filename);
	TexturedObject();

	void Start() override;
	void Update(float DeltaTime) override;
	void End() override;

private:
	float radRotate = 0;
	bool LoadFromFile = false;
	std::string Filename = "";
};

