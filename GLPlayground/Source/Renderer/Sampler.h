#pragma once
//Currently this binding method uses the layout(binding=0) in the shader that sets the texture unit to which the sampler is to be bound

class Sampler
{
public:
	Sampler();
	void Bind(unsigned int Unit);
	void UnBind();
	~Sampler();
private:
	unsigned int UnitLocation;
	uint32_t SamplerID;
};

