#pragma once
#include <memory>
#include <map>
#include "Renderer/Mesh.h"
#include "Resources/Importer.h"

class MeshManager
{
public:
	static MeshManager & GetMeshManager();
	std::shared_ptr<Mesh> CreateFromFile(const std::string Filename);
	std::shared_ptr<Mesh> CreateFullScreenQuad();

private:
	MeshManager();
	//~MeshManager();

	std::vector<std::unique_ptr<Importer>> Importers;

	std::map<std::size_t, std::shared_ptr<VertexData>> LoadedFilesCache;
};