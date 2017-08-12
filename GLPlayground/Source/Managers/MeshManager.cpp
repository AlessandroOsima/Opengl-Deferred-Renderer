#include "stdafx.h"
#include "MeshManager.h"
#include <vector>
#include <algorithm>
#include <limits>
#include "Resources/Obj/ObjImporter.h"

MeshManager & MeshManager::GetMeshManager()
{
	static MeshManager manager;

	return manager;
}

std::shared_ptr<Mesh> MeshManager::CreateFromFile(const std::string Filename)
{
	size_t hashedId = std::hash<std::string>{}(Filename);

	std::map<size_t, std::shared_ptr<VertexData>>::iterator it = LoadedFilesCache.find(hashedId);

	if (it == LoadedFilesCache.end())
	{
		std::string extension = Filename.substr(Filename.find_last_of(".") + 1);

		for (auto & imp : Importers)
		{
			if (imp->SupportsExtension(extension))
			{
				std::shared_ptr<VertexData> vData = imp->ParseResourceFile(Filename);

				LoadedFilesCache.insert(std::pair<size_t, std::shared_ptr<VertexData>>(hashedId, vData));

				return std::make_shared<Mesh>(vData);
			}
		}
	}
	else
	{
		return std::make_shared<Mesh>(it->second);
	}

	return nullptr;
}

std::shared_ptr<Mesh> MeshManager::CreateFullScreenQuad()
{
	std::vector<Vertex> vertices = {
		{ {-1,1,0}, {1,1,1,1}, {0,0,1}, {0,1}},
		{ { 1,1,0 }, { 1,1,1,1 }, { 0,0,1 }, {1,1}},
		{ { 1,-1,0 }, { 1,1,1,1 }, { 0,0,1 }, { 1,0 }},
		{ { -1,-1,0 }, { 1,1,1,1 }, { 0,0,1 }, { 0,0 }},
	};

	std::vector<Index> indices = {0,1,2,0,2,3};

	std::shared_ptr<VertexData> vData = std::make_shared<VertexData>(vertices, indices);

	return std::make_shared<Mesh>(vData);
}

MeshManager::MeshManager()
{

	Importers.push_back(std::make_unique<ObjImporter>());
}
