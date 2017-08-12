#include "stdafx.h"
#include "Scene.h"
#include "Renderer/RenderableScene.h"
#include "Managers/ResourceManager.h"
#include "Renderer/Texture.h"
#include "GameObjects/Components/Renderable.h"
#include "Managers/ShaderManager.h"
#include "Logger/Logger.h"
#include "GameObjects/Components/Transform.h"
#include "GameObjects/Objects/PointLightObject.h"
#include "GameObjects/Objects/TexturedObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Math/Filters.h"
#include "Managers/TextureManager.h"
#include <Managers/MeshManager.h>
#include "Managers/InputManager.h"
#include "GameObjects/Components/Text.h"
#include "Managers/InputManager.h"
#include <glm/gtx/rotate_vector.hpp>

Scene::Scene(RenderableScene & RenderScene) : RenderScene(RenderScene)
{

}


void Scene::Init()
{

	WindowInfo CurrentWindowInfo;
	RenderScene.GetCurrentWindowInfo(CurrentWindowInfo);

	glm::mat4 perspective = glm::perspective(glm::radians(45.f), (float)CurrentWindowInfo.Width / (float)CurrentWindowInfo.Height, 0.1f, 300.f);

	RenderScene.SetProjection(perspective);

	std::unique_ptr<Object> orginal = std::make_unique<TexturedObject>("apollocsm.obj");
	orginal->SetLogicScene(this);
	GameObjects.push_back(std::move(orginal));

	std::unique_ptr<Object> light = std::make_unique<PointLightObject>(0);
	light->SetLogicScene(this);
	GameObjects.push_back(std::move(light));

	/*std::unique_ptr<Object> obj2 = std::make_unique<TexturedObject>("ccube.obj");
	obj2->SetLogicScene(this);
	GameObjects.push_back(std::move(obj2));*/

	for (auto & gameObject : GameObjects)
	{
		gameObject->Start();
	}

	bool Found = false;

	size_t textureID;

	if (!TextureManager::GetTextureManager().CreateTextureFromFile("test.jpg", textureID))
	{
		Logger::GetLogger().LogString("Unable to find texture info for test.jpg", LogType::ERROR);
		return;
	}

	TextureInfo info = TextureManager::GetTextureManager().GetTextureFromID(textureID)->GetTextureInfo();

	int sizeX = info.Width;
	int sizeY = info.Height;

	constexpr float ScaleFactor = 2.14;
	int inWindowSizeX = sizeX / ScaleFactor;
	int inWindowSizeY = sizeY / ScaleFactor;

	//ORIGINAL
	Transform * originalTransform = static_cast<Transform*>(GameObjects[0]->GetComponentOfType(ComponentsType::Transform));
	//originalTransform->SetTranslate(glm::translate(glm::mat4(), glm::vec3( CurrentWindowInfo.Width / 2, CurrentWindowInfo.Height / 2, -0.1f)));

	//originalTransform->SetTranslate(glm::translate(glm::mat4(), glm::vec3(0,-10, -30.f)));
	//originalTransform->SetScale(glm::scale(glm::mat4(), glm::vec3(2.f, 2.f, 2.f)));


	//originalTransform->SetTranslate(glm::translate(glm::mat4(), glm::vec3(0.f, -10.f, -30.f)));
	//originalTransform->SetScale(glm::scale(glm::mat4(), glm::vec3(15.f, 15.f, 15.f)));

	originalTransform->SetTranslate(glm::translate(glm::mat4(), glm::vec3(0.f, -0.3f, -30.f)));
	originalTransform->SetScale(glm::scale(glm::mat4(), glm::vec3(0.2f, 0.2f, 0.2f)));


	/*originalTransform->SetTranslate(glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, -10.f)));
	originalTransform->SetScale(glm::scale(glm::mat4(), glm::vec3(6, 6, 6)));
	originalTransform->SetRotate(glm::rotate(glm::radians(90.f), glm::vec3(1.f, 1.f, 0.f)));*/

	Transform * lightTransform = static_cast<Transform*>(GameObjects[1]->GetComponentOfType(ComponentsType::Transform));

	lightTransform->SetTranslate(glm::translate(glm::mat4(), glm::vec3(10.0f, 5.f, -20.f)));


	//originalTransform->SetScale(glm::scale(glm::mat4(), glm::vec3(1.f, 1.f, 1.f)));

	//Transform * obj2Transform = static_cast<Transform*>(GameObjects[1]->GetComponentOfType(ComponentsType::Transform));
	//originalTransform->SetTranslate(glm::translate(glm::mat4(), glm::vec3( CurrentWindowInfo.Width / 2, CurrentWindowInfo.Height / 2, -0.1f)));

	//originalTransform->SetTranslate(glm::translate(glm::mat4(), glm::vec3(0,0, -5.f)));

	//obj2Transform->SetTranslate(glm::translate(glm::mat4(), glm::vec3(+1, -1, -5.f)));

	//originalTransform->SetScale(glm::scale(glm::mat4(), glm::vec3(1.f, 1.f, 1.f)));

	size_t DeferredLightingHash;

	if (ShaderManager::GetShaderManager().CreateShader("deferredLightingGeometry", "deferredLightingGeometry.vs", "deferredLightingGeometry.fs", DeferredLightingHash))
	{
		//Set Up Renderpasses on Objects
		{
			Renderable * originalRenderable = static_cast<Renderable*>(GameObjects[0]->GetComponentOfType(ComponentsType::Renderable));
			
			auto & material = originalRenderable->GetMesh()->GetMaterial();
			material.Program = DeferredLightingHash;

			//UniformTypeData Diffuse;
			//Diffuse.vec4Val = glm::vec4(0.95f, 0.95f, 0.0f, 1.0f);
			//material.AddUniform({"material.Diffuse", Diffuse, UniformType::Vec4});

			//UniformTypeData Specular;
			//Specular.vec4Val = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);
			//material.AddUniform({ "material.Specular", Specular, UniformType::Vec4 });

			//UniformTypeData Shininess;
			//Shininess.floatVal = 300;
			//material.AddUniform({ "material.Shininess", Shininess, UniformType::Float });

			//UniformTypeData ColorType;
			//ColorType.floatVal = static_cast<int>(DiffuseType::Diffuse);
			//material.AddUniform({ "material.DiffuseType", ColorType, UniformType::Float });

			auto MaterialParams = std::make_unique<DeferredBlinnPhongMeshMaterialParameters>();

			MaterialParams->Diffuse = glm::vec4(1, 1, 1, 1);

			originalRenderable->GetMesh()->SetMaterialParameters(std::move(MaterialParams));
			originalRenderable->GetMesh()->LinkMaterialData();
		}
	}
}

void Scene::Update(float DeltaTime)
{
	for (auto & gameObject : GameObjects)
	{
		gameObject->Update(DeltaTime);
	}
}

void Scene::DeInit()
{
	for (auto & gameObject : GameObjects)
	{
		gameObject->End();
	}
}

Scene::~Scene()
{
}
