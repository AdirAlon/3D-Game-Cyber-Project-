#pragma once
#include <memory>

class SwapChain;
class DeviceContext;
class VertexBuffer;
class VertexShader;
class PixelShader;
class ConstantBuffer;
class IndexBuffer;
class RenderSystem;
class GrphicsEngine;
class ResourceManager;
class Resource;
class Texture;
class TextureManager;
class Mesh;
class MeshManager;
typedef std::shared_ptr<SwapChain> SCPtr;
typedef std::shared_ptr<DeviceContext> DCPtr;
typedef std::shared_ptr<VertexBuffer> VBPtr;
typedef std::shared_ptr<VertexShader> VSPtr;
typedef std::shared_ptr<PixelShader> PSPtr;
typedef std::shared_ptr<ConstantBuffer> CBPtr;
typedef std::shared_ptr<IndexBuffer> IBPtr;
typedef std::shared_ptr<Resource> RPtr;
typedef std::shared_ptr<Texture> TPtr;
typedef std::shared_ptr<Mesh> MPtr;