#pragma once
#include "ResourceManager.h"

class MeshManager: public ResourceManager
{
public:
	MeshManager();
	~MeshManager();
	MPtr createMeshFromFile(const wchar_t* path);
protected:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* file);
};

