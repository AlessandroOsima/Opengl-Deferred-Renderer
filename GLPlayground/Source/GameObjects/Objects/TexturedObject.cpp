#include "stdafx.h"
#include "TexturedObject.h"
#include "Renderer/RenderableScene.h"
#include "Managers/ResourceManager.h"
#include "Managers/TextureManager.h"
#include "Renderer/Texture.h"
#include "GameObjects/Components/Renderable.h"
#include "Managers/ShaderManager.h"
#include "Logger/Logger.h"
#include "GameObjects/Components/Transform.h"
#include "GameObjects/Objects/TexturedObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include "GameObjects/Components/Text.h"
#include "Renderer/VertexFactory.h"
#include "Managers/MeshManager.h"
#include "GameObjects/Components/RenderableDebug.h"
#include "GameObjects/Components/DisComponent.h"

TexturedObject::TexturedObject()
{

}

TexturedObject::TexturedObject(const std::string & Filename) : LoadFromFile(true), Filename(Filename)
{

}

void TexturedObject::Start()
{

	Object::Start();

	Renderable* rend = AddComponent<Renderable>(std::make_unique<Renderable>());
	//AddComponent<DisComponent>(std::make_unique<DisComponent>());

	//AddComponent(std::make_unique<RenderableDebug>());


	if (LoadFromFile)
	{
		std::string txStr = ResourceManager::GetMeshesFolder();
		auto mesh = MeshManager::GetMeshManager().CreateFromFile(txStr += Filename);
		rend->SetMesh(mesh);
	}
	else
	{
		auto mesh = VertexFactory::GenerateCube();
		rend->SetMesh(mesh);
	}

	AddComponent<Transform>(std::make_unique<Transform>());

	AddComponent<Text>(std::make_unique<Text>());
}

void TexturedObject::Update(float DeltaTime)
{
	Object::Update(DeltaTime);
}

void TexturedObject::End()
{
	Object::End();
}

