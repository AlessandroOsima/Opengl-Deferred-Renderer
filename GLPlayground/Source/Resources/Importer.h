#pragma once
#include <memory>
#include "Renderer/VertexData.h"

class Importer
{
public:
	virtual std::shared_ptr<VertexData> ParseResourceFile(const std::string & Filename) = 0;
	virtual bool SupportsExtension(const std::string Extension) = 0;
private:
	std::string supportedExtension;
};

