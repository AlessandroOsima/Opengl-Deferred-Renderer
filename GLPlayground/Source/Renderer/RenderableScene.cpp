#include "stdafx.h"
#include "RenderableScene.h"
#include "Managers/ShaderManager.h"
#include "Managers/TextureManager.h"
#include "Managers/MeshManager.h"
#include <limits>
#include "GLUtilities.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cstring>

RenderableScene::RenderableScene(GLRenderer & Renderer) : Renderer(Renderer), BaseMaterial()
{
}

RenderableScene::~RenderableScene()
{
	glDeleteBuffers(1, &UniformMatricesBufferID);
}

void RenderableScene::Initialize()
{
	Renderer.SetClearColor({0,0,0,1});
	glCreateBuffers(1, &UniformMatricesBufferID);
	glNamedBufferStorage(UniformMatricesBufferID, sizeof(UniformMatricesBuffer), &UniformMatricesBuffer, GL_DYNAMIC_STORAGE_BIT);

	std::memset(&Lights, 0, sizeof(Lights));

	glCreateBuffers(1, &UniformLightsBufferID);
	glNamedBufferStorage(UniformLightsBufferID, sizeof(Lights), &Lights, GL_DYNAMIC_STORAGE_BIT);

	Lights.Ambient = glm::vec4(0.05f, 0.05f, 0.05f, 1);

	/*Lights.DirectionalLights[0].Direction = glm::vec4(0.f, 0.f, -1.f, 0);
	Lights.DirectionalLights[0].Direction = glm::normalize(Lights.DirectionalLights[0].Direction);
	Lights.DirectionalLights[0].SetEnabled(true);
	Lights.DirectionalLights[0].Color = glm::vec4(1, 1.f, 0.8f, 1);*/

	/*Lights.PointLights[0].Position = glm::vec4(0.f, 0.f, 0.f, 0);
	Lights.PointLights[0].SetEnabled(true);
	Lights.PointLights[0].Color = glm::vec4(1, 1.f, 0.8f, 1);*/

	/*Lights.DirectionalLights[1].Direction = glm::vec4(-1, 0, 0, 1);
	Lights.DirectionalLights[1].SetEnabled(true);
	Lights.DirectionalLights[1].Color = glm::vec4(1, 0, 0, 1);*/
	

	WindowInfo info;
	Renderer.GetCurrentWindowInfo(info);

	UniformMatricesBuffer.Projection = glm::ortho((float)0, (float)info.Width, (float)0, (float)info.Height, 0.f, 100.f);

	ShaderManager::GetShaderManager().OnShaderAdded = [&](size_t HashedProgram)
	{
		constexpr unsigned int MatricesBindingLocation = 0;
		constexpr char * MatricesUniformName = "Matrices";

		constexpr unsigned int LightsBindingLocation = 2;
		constexpr char * LightsUniformName = "lights";

		bool found = false;
		ShaderProgram & program = ShaderManager::GetShaderManager().GetShader(HashedProgram, found);

		if (found)
		{
			program.BindBufferToUniform(UniformMatricesBufferID, MatricesBindingLocation, MatricesUniformName);

			program.BindBufferToUniform(UniformLightsBufferID, LightsBindingLocation, LightsUniformName);
		}

		glNamedBufferSubData(UniformMatricesBufferID, 0, sizeof(glm::mat4), &UniformMatricesBuffer.Projection);

		glNamedBufferSubData(UniformMatricesBufferID, sizeof(glm::mat4), sizeof(glm::mat4), &UniformMatricesBuffer.View);

		//glNamedBufferSubData(UniformMatricesBufferID, sizeof(glm::mat4), sizeof(glm::mat4), &UniformMatricesBuffer.View);
	};

	size_t baseShaderHash;
	if (ShaderManager::GetShaderManager().CreateShader("base", "base.vs", "base.fs", baseShaderHash))
	{
		BaseMaterial.Program = baseShaderHash;
		BaseMaterial.CreateObjects();
	}
	else
	{
		Logger::GetLogger().LogString("Unable to create base material for Renderable Scene", LogType::ERROR);
	}

	//glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SetDepthEnabled(true);

	Drawer.Init();

	Position.GenerateTextureWithSize(info.Width, info.Height, GL_RGBA16F);
	Normals.GenerateTextureWithSize(info.Width, info.Height, GL_RGBA16F);
	Diffuse.GenerateTextureWithSize(info.Width, info.Height, GL_RGBA16F);
	Specular.GenerateTextureWithSize(info.Width, info.Height, GL_RGBA16F);
	Ambient.GenerateTextureWithSize(info.Width, info.Height, GL_RGBA16F);
	Depth.GenerateTextureWithSize(info.Width, info.Height, GL_DEPTH24_STENCIL8);

	GBuffer.BindTextureToFramebuffer(Position, GL_COLOR_ATTACHMENT0, 0);
	GBuffer.BindTextureToFramebuffer(Normals, GL_COLOR_ATTACHMENT1, 0);
	GBuffer.BindTextureToFramebuffer(Diffuse, GL_COLOR_ATTACHMENT2, 0);
	GBuffer.BindTextureToFramebuffer(Specular, GL_COLOR_ATTACHMENT3, 0);
	GBuffer.BindTextureToFramebuffer(Ambient, GL_COLOR_ATTACHMENT4, 0);

	GBuffer.BindTextureToFramebuffer(Depth, GL_DEPTH_ATTACHMENT, 0);

	FullscreenLightpassQuad = MeshManager::GetMeshManager().CreateFullScreenQuad();

	size_t deferredLightpassMaterialID = -1;
	if (ShaderManager::GetShaderManager().CreateShader("deferredLightingLightpass", "deferredLightingLightpass.vs", "deferredLightingLightpass.fs", deferredLightpassMaterialID))
	{
		Material DeferredLightpassMaterial;
		DeferredLightpassMaterial.Program = deferredLightpassMaterialID;
		//DeferredLightpassMaterial.CreateObjects();
		FullscreenLightpassQuad->SetMaterial(std::move(DeferredLightpassMaterial));
	}
	else
	{
		Logger::GetLogger().LogString("Unable to create deferredLightingLightpass shader", LogType::ERROR);
	}
}

void RenderableScene::RenderScene(float DeltaTime)
{

	WindowInfo info;
	Renderer.GetCurrentWindowInfo(info);

	GBuffer.BindFramebuffer(FramebufferBindType::DRAW);
	
#ifdef _DEBUG

	if (!GBuffer.IsComplete())
	{
		Logger::GetLogger().LogString("GBuffer is not complete", LogType::ERROR);
	}

#endif

	GLenum targets[5] = { GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
	glDrawBuffers(5, targets);
	
	Renderer.Clear();

	glViewport(0, 0, info.Width, info.Height);


	//geometry pass

	//Since we are rendering on the main window we need to use the correct matrices
	glNamedBufferSubData(UniformMatricesBufferID, 0, sizeof(UniformMatricesBuffer), &UniformMatricesBuffer);

	glNamedBufferSubData(UniformLightsBufferID, 0, sizeof(Lights), &Lights);

	for (auto & mesh : MeshList)
	{

		mesh.Mesh->Bind();

		Renderer.DrawMesh(*mesh.Mesh);

		mesh.Mesh->UnBind();

	}

	GBuffer.UnBindFramebuffer();
	
	//Lightpass

	Renderer.Clear();
	
	Position.Bind(0);
	PositionSampler.Bind(0);
	Normals.Bind(1);
	NormalsSampler.Bind(1);
	Diffuse.Bind(2);
	DiffuseSampler.Bind(2);
	Specular.Bind(3);
	SpecularSampler.Bind(3);
	Ambient.Bind(4);
	AmbientSampler.Bind(4);
	
	UniformTypeData cameraPosition;
	cameraPosition.vec3Val = glm::vec3(0,0,0);
	FullscreenLightpassQuad->GetMaterial().AddUniform({ "pv", cameraPosition, UniformType::Vec3 });
	FullscreenLightpassQuad->Bind();

	Renderer.DrawMesh(*FullscreenLightpassQuad);

	Position.UnBind();
	PositionSampler.UnBind();
	Normals.UnBind();
	NormalsSampler.UnBind();
	Diffuse.UnBind();
	DiffuseSampler.UnBind();
	Specular.UnBind();
	SpecularSampler.UnBind();
	Ambient.UnBind();
	AmbientSampler.UnBind();

	FullscreenLightpassQuad->UnBind();

	Drawer.Render();

	for (auto & fontRenderer : FontRenderers)
	{
		fontRenderer.second.Render(Renderer, DeltaTime);
	}

	Renderer.Present();
}

void RenderableScene::DeInitialize()
{

	GBuffer.UnBindFramebuffer();
	GBuffer.UnbindFramebufferAttachment(GL_COLOR_ATTACHMENT0);
	GBuffer.UnbindFramebufferAttachment(GL_COLOR_ATTACHMENT1);
	GBuffer.UnbindFramebufferAttachment(GL_COLOR_ATTACHMENT2);
	GBuffer.UnbindFramebufferAttachment(GL_COLOR_ATTACHMENT3);
	GBuffer.UnbindFramebufferAttachment(GL_COLOR_ATTACHMENT4);
	GBuffer.UnbindFramebufferAttachment(GL_DEPTH_ATTACHMENT);

	for (auto & fontRenderer : FontRenderers)
	{
		fontRenderer.second.DeInit();
	}

	BaseMaterial.RemoveObjects();

	Drawer.DeInit();
}

RenderableMeshLocation RenderableScene::AddMesh(std::shared_ptr<Mesh> MeshToAdd)
{
	MeshStorageInfo info;  
	info.Mesh = MeshToAdd;

	MeshList.push_back(info);
	return --MeshList.end();
}

void RenderableScene::RemoveMesh(RenderableMeshLocation Location)
{
	MeshList.erase(Location);
}

size_t RenderableScene::CreateFontRenderer(std::string FontName)
{
	size_t hash = std::hash<std::string>{}(FontName);

	std::map<std::size_t, FontRenderer>::iterator it = FontRenderers.find(hash);

	if (it != FontRenderers.end())
	{
		return hash;
	}

	FontRenderer FRenderer;

	WindowInfo info;
	Renderer.GetCurrentWindowInfo(info);

	FRenderer.Init(FontName, info);

	FontRenderers[hash] = std::move(FRenderer);

	return hash;
}

void RenderableScene::DestroyFontRenderer(size_t FontRendererID)
{
	FontRenderer * fRenderer = GetFontRenderer(FontRendererID);

	if (fRenderer)
	{
		FontRenderers.erase(FontRendererID);
	}
}
