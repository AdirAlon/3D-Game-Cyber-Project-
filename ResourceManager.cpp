#include "ResourceManager.h"
#include <filesystem>

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

RPtr ResourceManager::createResourceFromFile(const wchar_t* file)
{
    std::wstring path = std::filesystem::absolute(file);
    auto it = m_resources.find(path);
    if (it != m_resources.end())
        return it->second;

    Resource* rRes = this->createResourceFromFileConcrete(path.c_str());
    if (rRes)
    {
        RPtr res(rRes);
        m_resources[path] = res;
        return res;
    }

    return nullptr;
}
