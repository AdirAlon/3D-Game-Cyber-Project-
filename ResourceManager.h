#pragma once
#include <unordered_map>
#include <string>
#include "prereq.h"
#include "Resource.h"

class ResourceManager
{
public:
	ResourceManager();
	virtual ~ResourceManager();
	RPtr createResourceFromFile(const wchar_t* file);

protected:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* file) = 0;
private:
	std::unordered_map<std::wstring, RPtr> m_resources;
};

