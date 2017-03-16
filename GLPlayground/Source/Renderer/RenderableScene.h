#pragma once
#include "Renderer/FontRenderer.h"
#include "Renderer/Mesh.h"
#include <memory>
#include "Renderer/GLRenderer.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/RenderPass.h"
#include <limits>
#include <list>

struct MeshStorageInfo
{
	std::shared_ptr<Mesh> Mesh;
	std::list<RenderPassGroup>::iterator Location;
};

using RenderableMeshLocation = std::list<MeshStorageInfo>::iterator;
using RenderablePassLocation = std::list<RenderPassGroup>::iterator;


class RenderableScene
{

public:
	RenderableScene(GLRenderer & Renderer);
	~RenderableScene();

	void Initialize();

	void RenderScene(float DeltaTime);

	void DeInitialize();

	RenderableMeshLocation AddMesh(std::shared_ptr<Mesh> MeshToAdd);
	RenderableMeshLocation AddMeshMultipass(std::shared_ptr<Mesh> MeshToAdd, RenderPassGroup && PassesToAdd);

	void RemoveMesh(RenderableMeshLocation Location);

	RenderablePassLocation AddRenderPassGroup(RenderPassGroup && PassToAdd);
	void RemoveRenderPassGroup(RenderablePassLocation Location);

	bool LinkMeshMultiPass(RenderableMeshLocation Mesh, RenderablePassLocation Pass);

	inline void SetProjection(const glm::mat4 & Projection)
	{
		CurrentProjection = Projection;
	}

	inline void SetDepthEnabled(bool Enabled)
	{
		Renderer.EnableDepthTest(Enabled);
	}

	inline void SetView(const glm::mat4 & View)
	{
		CurrentView = View;
	}

	inline void GetCurrentWindowInfo(WindowInfo & CurrentWindowInfo)
	{
		return Renderer.GetCurrentWindowInfo(CurrentWindowInfo);
	}

	size_t CreateFontRenderer(std::string FontName);

	void DestroyFontRenderer(size_t FontRendererID);

	inline FontRenderer * GetFontRenderer(size_t FontRendererID)
	{
		std::map<std::size_t, FontRenderer>::iterator it = FontRenderers.find(FontRendererID);

		if (it == FontRenderers.end())
		{
			return nullptr;
		}

		return &FontRenderers[FontRendererID];
	}

private:

	GLRenderer & Renderer;

	uint32_t UniformMatricesBufferID;
	UniformMatrices UniformMatricesBuffer;

	glm::mat4 CurrentView;
	glm::mat4 CurrentProjection;

	Framebuffer OffscreenFramebuffer;
	size_t OffscreenFBColorAttachment;

	Material BaseMaterial;

	std::map<size_t, FontRenderer> FontRenderers;

	std::list<MeshStorageInfo> MeshList;
	std::list<RenderPassGroup> PassList;
};

