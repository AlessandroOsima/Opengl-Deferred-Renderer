#pragma once
#include "Renderer/Texture.h"
#include <map>
#include <functional>
#include <memory>

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	bool CreateTextureFromFile(const std::string & TextureName, size_t & TextureID);
	bool CreateTexture(const std::string & TextureName, unsigned int Format, uint32_t Width, uint32_t Height, size_t & TextureID);

	std::shared_ptr<Texture> GetTextureFromID(size_t TextureID);

	std::shared_ptr<Texture> GetTextureAndIDFromName(const std::string & TextureName, size_t & ID);
	size_t GetIDFromName(const std::string & TextureName);
	std::shared_ptr<Texture> GetTextureFromName(const std::string & TextureName);
	void DestroyTexture(size_t ID);
	bool TextureExist(size_t ID);

	static TextureManager & GetTextureManager()
	{
		static TextureManager Manager;
		return Manager;
	}

private:
	std::map<std::size_t, std::shared_ptr<Texture>> Textures;
};

