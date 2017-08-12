#pragma once
#include "Renderer/FontRenderer.h"
#include "Renderer/Mesh.h"
#include <memory>
#include "Renderer/GLRenderer.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/RenderingInfo.h"
#include <limits>
#include <list>
#include "Renderer/PrimitiveDrawer.h"
#include "Renderer/Lights/Lights.h"
#include "Renderer/Sampler.h"

struct MeshStorageInfo
{
	std::shared_ptr<Mesh> Mesh;
};

using RenderableMeshLocation = std::list<MeshStorageInfo>::iterator;

class RenderableScene
{

public:
	RenderableScene(GLRenderer & Renderer);
	~RenderableScene();

	void Initialize();

	void RenderScene(float DeltaTime);

	void DeInitialize();

	RenderableMeshLocation AddMesh(std::shared_ptr<Mesh> MeshToAdd);
	
	void RemoveMesh(RenderableMeshLocation Location);

	inline void SetProjection(const glm::mat4 & Projection)
	{
		UniformMatricesBuffer.Projection = Projection;
	}

	inline void SetDepthEnabled(bool Enabled)
	{
		Renderer.EnableDepthTest(Enabled);
	}

	inline void SetView(const glm::mat4 & View)
	{
		UniformMatricesBuffer.View = View;
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

	inline PrimitiveDrawer & GetPrimitiveDrawer()
	{
		return Drawer;
	}

	SceneLights Lights;

private:


	GLRenderer & Renderer;

	uint32_t UniformMatricesBufferID;
	UniformMatrices UniformMatricesBuffer;

	Framebuffer OffscreenFramebuffer;
	size_t OffscreenFBColorAttachment;

	Material BaseMaterial;

	std::map<size_t, FontRenderer> FontRenderers;

	std::list<MeshStorageInfo> MeshList;

	PrimitiveDrawer Drawer;

	uint32_t UniformLightsBufferID;

	Framebuffer GBuffer;

	Texture Position;
	Texture Normals;
	Texture Diffuse;
	Texture Specular;
	Texture Ambient;

	Texture Depth;

	std::shared_ptr<Mesh> FullscreenLightpassQuad;

	Sampler PositionSampler;
	Sampler NormalsSampler;
	Sampler DiffuseSampler;
	Sampler SpecularSampler;
	Sampler AmbientSampler;

};

