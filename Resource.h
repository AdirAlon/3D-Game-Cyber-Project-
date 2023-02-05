#pragma once
#include <string>

class Resource
{
public: 
	Resource(const wchar_t* path);
	virtual ~Resource();
protected:
	std::wstring m_path;
};

