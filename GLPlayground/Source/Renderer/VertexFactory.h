#pragma once
#include "Renderer/Mesh.h"

class VertexFactory
{
public:
	
	static std::shared_ptr<Mesh> GenerateQuad();
	static std::shared_ptr<Mesh> GenerateCube();
private:


};

