#include "stdafx.h"
#include "VertexFactory.h"


std::shared_ptr<Mesh> VertexFactory::GenerateQuad()
{
	static std::vector<Vertex> verts = {  //Vertices
		{ glm::vec3(1.f, 1.f, 0), glm::vec4(0, 0, 1, 1),    glm::vec3(1.f, 1.f, +1), glm::vec2(1, 1) }, //0
		{ glm::vec3(1.f,  -1.f, 0), glm::vec4(0, 1, 0, 1),  glm::vec3(1.f, 1.f, +1), glm::vec2(1,0) },  //1
		{ glm::vec3(-1.f, 1.f, 0), glm::vec4(1, 0, 0, 1),   glm::vec3(1.f, 1.f, +1), glm::vec2(0,1) }, //2
		{ glm::vec3(-1.f,   -1.f, 0), glm::vec4(0, 0, 1, 1),glm::vec3(1.f, 1.f, +1), glm::vec2(0,0) }  //3
	};
		//Indices
	static std::vector<Index> indices = 
	{
		    0,
		    1,
			2,
			2,
			1,
			3
	};

	static std::shared_ptr<VertexData> vData = std::make_shared<VertexData>(verts, indices);

	std::shared_ptr<Mesh> mesh(new Mesh(vData));

	return mesh;
}

std::shared_ptr<Mesh> VertexFactory::GenerateCube()
{
	static std::vector<Vertex> verts = {  //Vertices
		//FRONT
		{ glm::vec3(1.f, 1.f, +1), glm::vec4(0, 0, 1, 1), glm::vec3(1.f, 1.f, +1), glm::vec2(1, 1) }, //0
		{ glm::vec3(1.f,  -1.f, +1), glm::vec4(0, 1, 0, 1), glm::vec3(1.f, 1.f, +1), glm::vec2(1,0) },  //1
		{ glm::vec3(-1.f, 1.f, +1), glm::vec4(1, 0, 0, 1), glm::vec3(1.f, 1.f, +1), glm::vec2(0,1) }, //2
		{ glm::vec3(-1.f,   -1.f, +1), glm::vec4(0, 0, 1, 1), glm::vec3(1.f, 1.f, +1), glm::vec2(0,0) },  //3

		//BACK
		{ glm::vec3(1.f, 1.f, -1), glm::vec4(0, 0, 1, 1), glm::vec3(1.f, 1.f, +1), glm::vec2(1, 1) }, //4
		{ glm::vec3(1.f,  -1.f, -1), glm::vec4(0, 1, 0, 1), glm::vec3(1.f, 1.f, +1), glm::vec2(1,0) },  //5
		{ glm::vec3(-1.f, 1.f, -1), glm::vec4(1, 0, 0, 1), glm::vec3(1.f, 1.f, +1), glm::vec2(0,1) }, //6
		{ glm::vec3(-1.f,   -1.f, -1), glm::vec4(0, 0, 1, 1), glm::vec3(1.f, 1.f, +1), glm::vec2(0,0) },  //7

		//SIDE R
	    { glm::vec3(1.f, 1.f, +1), glm::vec4(0, 0, 1, 1), glm::vec3(1.f, 1.f, +1), glm::vec2(0, 1) }, //8 0
	    { glm::vec3(1.f,  -1.f, +1), glm::vec4(0, 1, 0, 1), glm::vec3(1.f, 1.f, +1), glm::vec2(0,0) },  //9 1
	    { glm::vec3(1.f, 1.f, -1), glm::vec4(1, 0, 0, 1), glm::vec3(1.f, 1.f, +1), glm::vec2(1,1) }, //10 2 
	    { glm::vec3(1.f,   -1.f, -1), glm::vec4(0, 0, 1, 1), glm::vec3(1.f, 1.f, +1), glm::vec2(1,0) },  //11 3

	    //SIDE L
		{ glm::vec3(-1.f, 1.f, +1), glm::vec4(0, 0, 1, 1), glm::vec3(1.f, 1.f, +1), glm::vec2(0, 1) }, //12 0
		{ glm::vec3(-1.f,  -1.f, +1), glm::vec4(0, 1, 0, 1), glm::vec3(1.f, 1.f, +1), glm::vec2(0,0) },  //13 1
		{ glm::vec3(-1.f, 1.f, -1), glm::vec4(1, 0, 0, 1), glm::vec3(1.f, 1.f, +1), glm::vec2(1,1) }, //14 2 
		{ glm::vec3(-1.f,  -1.f, -1), glm::vec4(0, 0, 1, 1), glm::vec3(1.f, 1.f, +1), glm::vec2(1,0) },  //15 3

	    //UP
		{ glm::vec3(-1.f, 1.f, +1), glm::vec4(0, 0, 1, 1),glm::vec3(1.f, 1.f, +1),glm::vec2(0,1) }, //16 0
		{ glm::vec3(1.f,  1.f, +1), glm::vec4(0, 1, 0, 1),glm::vec3(1.f, 1.f, +1),glm::vec2(1,1) },  //17 1
		{ glm::vec3(-1.f, 1.f, -1), glm::vec4(1, 0, 0, 1),glm::vec3(1.f, 1.f, +1),glm::vec2(0,0) }, //18 2 
		{ glm::vec3(1.f,  1.f, -1), glm::vec4(0, 0, 1, 1),glm::vec3(1.f, 1.f, +1),glm::vec2(1,0) },  //19 3

	    //DOWN
		{ glm::vec3(-1.f, -1.f, +1), glm::vec4(0, 0, 1, 1),glm::vec3(1.f, 1.f, +1), glm::vec2(0,1) }, //20 0
		{ glm::vec3(1.f,  -1.f, +1), glm::vec4(0, 1, 0, 1),glm::vec3(1.f, 1.f, +1), glm::vec2(1,1) },  //21 1
		{ glm::vec3(-1.f, -1.f, -1), glm::vec4(1, 0, 0, 1),glm::vec3(1.f, 1.f, +1), glm::vec2(0,0) }, //22 2 
		{ glm::vec3(1.f,  -1.f, -1), glm::vec4(0, 0, 1, 1),glm::vec3(1.f, 1.f, +1), glm::vec2(1,0) },  //23 3
														  
	};
	//Indices
	static std::vector<Index> indices =
	{
		0,
		1,
		2,

		2,
		1,
		3,

		4,
		5,
		6,

		6,
		5,
		7,

		8,
		9,
		10,

		10,
		9,
		11,

		12,
		13,
		14,

		14,
		13,
		15,

		16,
		17,
		18,


		18,
		17,
		19,

		20,
		21,
		22,

		22,
		21,
		23
	};

	static std::shared_ptr<VertexData> vData = std::make_shared<VertexData>(verts, indices);

	std::shared_ptr<Mesh> mesh(new Mesh(vData));

	return mesh;
}
