#include "stdafx.h"
#include <GL/glew.h>
#include "TextureManager.h"
#include <memory>
#include <string>
#include "Managers/ResourceManager.h"



TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}

bool TextureManager::CreateTextureFromFile(const std::string & TextureName, size_t & TextureID)
{
	bool alreadyCreated = false;

	GetTextureAndIDFromName(TextureName, TextureID);

	if (alreadyCreated)
	{
		return true;
	}

	std::shared_ptr<Texture> texture = std::make_shared<Texture>();

	std::string txStr = ResourceManager::GetTexturesFolder();
	if (!texture->LoadFromFile(txStr += TextureName))
	{
		return false;
	}

	std::size_t hash = std::hash<std::string>{}(TextureName);
	TextureID = hash;

	Textures[hash] = std::move(texture);

	return true;
}

bool TextureManager::CreateTexture(const std::string & TextureName, unsigned int Format, uint32_t Width, uint32_t Height, size_t & TextureID)
{
	if (GetTextureAndIDFromName(TextureName, TextureID) != nullptr)
	{
		return true;
	}

	std::shared_ptr<Texture> texture = std::make_shared<Texture>();

	texture->GenerateTextureWithSize(Width, Height, Format);

	std::size_t hash = std::hash<std::string>{}(TextureName);
	TextureID = hash;

	Textures[hash] = std::move(texture);

	return true;
}

std::shared_ptr<Texture> TextureManager::GetTextureFromID(size_t TextureID)
{
	std::map<std::size_t, std::shared_ptr<Texture>>::iterator it = Textures.find(TextureID);

	if (it == Textures.end())
	{
		return nullptr;
	}

	return Textures[TextureID];
}

std::shared_ptr<Texture> TextureManager::GetTextureAndIDFromName(const std::string & TextureName, size_t & ID)
{
	std::size_t hash = std::hash<std::string>{}(TextureName);
	ID = hash;
	
	return GetTextureFromID(hash);
}

size_t TextureManager::GetIDFromName(const std::string & TextureName)
{

	std::size_t hash = std::hash<std::string>{}(TextureName);
	std::map<std::size_t, std::shared_ptr<Texture>>::iterator it = Textures.find(hash);

	if (it == Textures.end())
	{
		return 0;
	}

	return hash;
}

std::shared_ptr<Texture> TextureManager::GetTextureFromName(const std::string & TextureName)
{
	std::size_t hash = std::hash<std::string>{}(TextureName);

	return GetTextureFromID(hash);
}

void TextureManager::DestroyTexture(size_t ID)
{
	if (!TextureExist(ID))
	{
		return;
	}

	Textures.erase(ID);
}

bool TextureManager::TextureExist(size_t ID)
{
	std::map<std::size_t, std::shared_ptr<Texture>>::iterator it = Textures.find(ID);

	if (it == Textures.end())
	{
		return false;
	}

	return true;

}
