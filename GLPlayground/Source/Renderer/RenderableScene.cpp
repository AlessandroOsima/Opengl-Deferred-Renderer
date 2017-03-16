#include "stdafx.h"
#include "RenderableScene.h"
#include "Managers/ShaderManager.h"
#include "Managers/TextureManager.h"
#include <limits>
#include "GLUtilities.h"
#include <glm/gtc/matrix_transform.hpp>

RenderableScene::RenderableScene(GLRenderer & Renderer) : Renderer(Renderer), BaseMaterial(0, 0)
{
}

RenderableScene::~RenderableScene()
{
	glDeleteBuffers(1, &UniformMatricesBufferID);
}

void RenderableScene::Initialize()
{
	glCreateBuffers(1, &UniformMatricesBufferID);
	glNamedBufferStorage(UniformMatricesBufferID, sizeof(UniformMatricesBuffer), &UniformMatricesBuffer, GL_DYNAMIC_STORAGE_BIT);

	WindowInfo info;
	Renderer.GetCurrentWindowInfo(info);

	CurrentProjection = glm::ortho((float)0, (float)info.Width, (float)0, (float)info.Height, 0.f, 100.f);

	ShaderManager::GetShaderManager().OnShaderAdded = [&](size_t HashedProgram)
	{
		constexpr unsigned int MatricesBindingLocation = 0;
		constexpr char * MatricesUniformName = "Matrices";

		bool found = false;
		ShaderProgram & program = ShaderManager::GetShaderManager().GetShader(HashedProgram, found);

		if (found)
		{
			program.BindBufferToUniform(UniformMatricesBufferID, MatricesBindingLocation, MatricesUniformName);
		}

		glNamedBufferSubData(UniformMatricesBufferID, 0, sizeof(glm::mat4), &CurrentProjection);

		glNamedBufferSubData(UniformMatricesBufferID, sizeof(glm::mat4), sizeof(glm::mat4), &CurrentView);
	};

	size_t baseShaderHash;
	if (ShaderManager::GetShaderManager().CreateShader("base", "base.vs", "base.fs", baseShaderHash))
	{
		BaseMaterial = { 0, baseShaderHash };
		BaseMaterial.CreateObjects();
	}
	else
	{
		Logger::GetLogger().LogString("Unable to create base material for Renderable Scene", LogType::ERROR);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void RenderableScene::RenderScene(float DeltaTime)
{

	WindowInfo info;
	Renderer.GetCurrentWindowInfo(info);

	Renderer.Clear();


	for (auto & mesh : MeshList)
	{
		//Render with only one pass
		if (mesh.Location == PassList.end())
		{
			Logger::GetLogger().LogString("mesh does not have a valid renderpass", LogType::ERROR);
		}
		else
		{
			bool found;

			Texture & colorTarget = TextureManager::GetTextureManager().GetTextureFromID(mesh.Location->GetOffscreenTexture(), found);
			Texture & textureAttachment = TextureManager::GetTextureManager().GetTextureFromID(mesh.Location->GetAttachmentTexture(), found);

			size_t attachmentTexture = mesh.Location->RenderPasses[0].GetMaterial().DiffuseTexture;
			size_t original;

			//Do all the addictive passes
			for (int i = 0; i < mesh.Location->RenderPasses.size(); i++)
			{
				//The viewport is the size of the texture we are rendering
				glViewport(0, 0, colorTarget.GetTextureInfo().Width, colorTarget.GetTextureInfo().Height);

				//Bind the attachment texture to the framebuffer as a color target
				OffscreenFramebuffer.BindTextureToFramebuffer(colorTarget, FrameBufferAttachmentType::COLOR);
				OffscreenFramebuffer.BindFramebuffer(FramebufferBindType::FRAMEBUFFER);

				//Setup the shaders for the current RenderPass
				Renderer.Clear();

				mesh.Mesh->Bind();

				if (mesh.Location->RenderPasses[i].UsePreviousPassAsAttachment)
				{
					original = mesh.Location->RenderPasses[i].GetMaterial().DiffuseTexture;
					mesh.Location->RenderPasses[i].GetMaterial().DiffuseTexture = attachmentTexture;
				}

				mesh.Location->RenderPasses[i].GetMaterial().Bind();
				mesh.Location->RenderPasses[i].BindUniforms();

				bool ShaderFound;

				ShaderProgram & program = ShaderManager::GetShaderManager().GetShader(mesh.Location->RenderPasses[i].GetMaterial().Program, ShaderFound);

				AssertWithMessage(ShaderFound, "Unable to find shader");

				constexpr unsigned int MatricesBindingLocation = 0;
				constexpr char * MatricesUniformName = "Matrices";
				program.BindBufferToUniform(UniformMatricesBufferID, MatricesBindingLocation, MatricesUniformName);

				//When we render to an offscreen texture the model, view and projection matrices are all identity matrices because there is no need for transformations
				glNamedBufferSubData(UniformMatricesBufferID, 0, sizeof(glm::mat4), &glm::mat4(1));
				glNamedBufferSubData(UniformMatricesBufferID, sizeof(glm::mat4), sizeof(glm::mat4), &glm::mat4(1));
				glNamedBufferSubData(UniformMatricesBufferID, sizeof(glm::mat4) * 2, sizeof(glm::mat4), &glm::mat4(1));

				Renderer.DrawMesh(*mesh.Mesh);

				mesh.Location->RenderPasses[i].GetMaterial().UnBind();
				mesh.Mesh->Unbind();

				if (mesh.Location->RenderPasses[i].UsePreviousPassAsAttachment)
				{
					//Copy the framebuffer color target to another texture to use as input for the fragment shader in the RenderPass
					glBindTexture(GL_TEXTURE_2D, textureAttachment.GetID());
					glCheckFunction(glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, colorTarget.GetTextureInfo().Width, colorTarget.GetTextureInfo().Height));
					glBindTexture(GL_TEXTURE_2D, 0);

					mesh.Location->RenderPasses[i].GetMaterial().DiffuseTexture = original;
					attachmentTexture = mesh.Location->GetAttachmentTexture();
				}

				if (mesh.Location->RenderPasses[i].RenderOnMainFramebuffer)
				{
					//Render the mesh in the main framebuffer
					OffscreenFramebuffer.UnbindFramebufferAttachment(FrameBufferAttachmentType::COLOR);
					OffscreenFramebuffer.UnBindFramebuffer();

					glViewport(0, 0, info.Width, info.Height);

					mesh.Mesh->Bind();

					BaseMaterial.DiffuseTexture = attachmentTexture;
					BaseMaterial.Bind();

					//Since we are rendering on the main window we need to use the correct matrices
					glNamedBufferSubData(UniformMatricesBufferID, 0, sizeof(glm::mat4), &CurrentProjection);
					glNamedBufferSubData(UniformMatricesBufferID, sizeof(glm::mat4), sizeof(glm::mat4), &CurrentView);
					glNamedBufferSubData(UniformMatricesBufferID, sizeof(glm::mat4) * 2, sizeof(glm::mat4), &mesh.Mesh->GetModel());

					Renderer.DrawMesh(*mesh.Mesh);

					BaseMaterial.UnBind();
					mesh.Mesh->Unbind();

				}
			}

			OffscreenFramebuffer.UnbindFramebufferAttachment(FrameBufferAttachmentType::COLOR);
			OffscreenFramebuffer.UnBindFramebuffer();
		}
	}

	OffscreenFramebuffer.UnbindFramebufferAttachment(FrameBufferAttachmentType::COLOR);
	OffscreenFramebuffer.UnBindFramebuffer();

	//Restore the viewport and render all the TextRenderers
	glViewport(0, 0, info.Width, info.Height);


	for (auto & fontRenderer : FontRenderers)
	{
		fontRenderer.second.Render(Renderer, DeltaTime);
	}

	Renderer.Present();
}

void RenderableScene::DeInitialize()
{
	for (auto & fontRenderer : FontRenderers)
	{
		fontRenderer.second.DeInit();
	}

	BaseMaterial.RemoveObjects();
}

RenderableMeshLocation RenderableScene::AddMesh(std::shared_ptr<Mesh> MeshToAdd)
{
	MeshStorageInfo info;
	info.Mesh = MeshToAdd;
	info.Location = PassList.end();

	MeshList.push_back(info);
	return --MeshList.end();
}

RenderableMeshLocation RenderableScene::AddMeshMultipass(std::shared_ptr<Mesh> MeshToAdd, RenderPassGroup && PassesToAdd)
{
	RenderablePassLocation passLoc = AddRenderPassGroup(std::move(PassesToAdd));
	RenderableMeshLocation meshLoc = AddMesh(MeshToAdd);

	LinkMeshMultiPass(meshLoc, passLoc);

	return meshLoc;
}

void RenderableScene::RemoveMesh(RenderableMeshLocation Location)
{
	MeshList.erase(Location);
}


RenderablePassLocation RenderableScene::AddRenderPassGroup(RenderPassGroup && PassToAdd)
{
	PassToAdd.Init();
	PassList.push_back(std::move(PassToAdd));
	return --PassList.end();
}

void RenderableScene::RemoveRenderPassGroup(RenderablePassLocation Location)
{
	PassList.erase(Location);
}

bool RenderableScene::LinkMeshMultiPass(RenderableMeshLocation Mesh, RenderablePassLocation Pass)
{

	Mesh->Location = Pass;

	return true;
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
