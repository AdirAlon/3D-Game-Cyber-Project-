#pragma once
#include "ResourceManager.h"

class TextureManager: public ResourceManager
{
public:
	TextureManager();
	~TextureManager();
	TPtr createTextureFromFile(const wchar_t* path);
protected:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* file);
};

