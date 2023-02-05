#include "MeshManager.h"
#include "Mesh.h"
MeshManager::MeshManager()
{
}

MeshManager::~MeshManager()
{
}

MPtr MeshManager::createMeshFromFile(const wchar_t* path)
{
    return std::static_pointer_cast<Mesh>(createResourceFromFile(path));
}

Resource* MeshManager::createResourceFromFileConcrete(const wchar_t* file)
{
    Mesh* mesh = nullptr;
    try
    {
        mesh = new Mesh(file);
    }
    catch (...) {}
    return mesh;
}
