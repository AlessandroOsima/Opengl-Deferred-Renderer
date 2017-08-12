#include "stdafx.h"
#include "Sampler.h"
#include <GL/glew.h>

Sampler::Sampler()
{
	glCreateSamplers(1, &SamplerID);
}


void Sampler::Bind(unsigned int Unit)
{
	UnitLocation = Unit;
	glBindSampler(Unit, SamplerID);
}

void Sampler::UnBind()
{
	//Just placeholder, it actually doesn't make sense to unbind a sampler. Just change the program
}

Sampler::~Sampler()
{
	glDeleteSamplers(1, &SamplerID);
}
