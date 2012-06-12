/*!
	@file	Graphics/Direct3D10/Device.cpp

	@author	E.Nakayama
*/

#include "Marda/Graphics/Direct3D10.h"
#include "Marda/Graphics/Direct3D10/Core.h"

using namespace Marda;
using namespace Direct3D10;

namespace
{
}	// end namespace

// ----------------------------------------------------------------------------
Device::Device()
	: _core(new Core())
{
}

Device::~Device()
{
}

bool
Device::Create(HWND hWnd)
{
	return _core->Create(hWnd);
}

void
Device::Release()
{
	_core->Release();
}

bool 
Device::IsDeviceRemoved() const
{
	return _core->IsDeviceRemoved();
}

bool
Device::IsScreenOccluded() const
{
	return _core->IsScreenOccluded();
}

ID3D10Device*
Device::GetNativeDevice()
{
	return _core->GetNativeDevice();
}

Core&
Device::GetCore()
{
	return *_core;
}

RenderTargetHandle 
Device::GetBackBuffer() const
{
	return _core->GetBackBuffer();
}

DepthStencilBufferHandle
Device::GetDepthStencilBuffer() const
{
	return _core->GetDepthStencilBuffer();
}

bool
Device::ResizeBackBuffer(SIZE sz)
{
	return _core->ResizeBackBuffer(sz);
}

RenderTargetHandle 
Device::CreateRenderTarget(unsigned int width, unsigned int height, DXGI_FORMAT format, bool useShaderResource)
{
	return _core->CreateRenderTarget(width, height, format, useShaderResource);
}

DepthStencilBufferHandle 
Device::CreateDepthStencilBuffer(unsigned int width, unsigned int height, bool useShaderResource)
{
	return _core->CreateDepthStencilBuffer(width, height, useShaderResource);
}

InputLayoutHandle 
Device::CreateInputLayout(D3D10_INPUT_ELEMENT_DESC* inputElementDescs, UINT numInputElements, ShaderHandle shaderHandle)
{
	return _core->CreateInputLayout(inputElementDescs, numInputElements, shaderHandle);
}

void 
Device::ReleaseInputLayout(InputLayoutHandle handle)
{
	_core->ReleaseInputLayout(handle);
}

BlendStateHandle
Device::CreateBlendState(const D3D10_BLEND_DESC& desc)
{
	return _core->CreateBlendState(desc);
}

void
Device::ReleaseBlendState(BlendStateHandle handle)
{
	_core->ReleaseBlendState(handle);
}

const D3D10_BLEND_DESC*
Device::GetBlendStateDesc(BlendStateHandle handle) const
{
	return _core->GetBlendStateDesc(handle);
}

RasterizerStateHandle
Device::CreateRasterizerState(const D3D10_RASTERIZER_DESC& desc)
{
	return _core->CreateRasterizerState(desc);
}

void
Device::ReleaseRasterizerState(RasterizerStateHandle handle)
{
	_core->ReleaseRasterizerState(handle);
}

const D3D10_RASTERIZER_DESC*
Device::GetRasterizerStateDesc(RasterizerStateHandle handle) const
{
	return _core->GetRasterizerStateDesc(handle);
}


DepthStencilStateHandle
Device::CreateDepthStencilState(const D3D10_DEPTH_STENCIL_DESC& desc)
{
	return _core->CreateDepthStencilState(desc);
}

void
Device::ReleaseDepthStencilState(DepthStencilStateHandle handle)
{
	_core->ReleaseDepthStencilState(handle);
}

const D3D10_DEPTH_STENCIL_DESC*
Device::GetDepthStencilStateDesc(DepthStencilStateHandle handle) const
{
	return _core->GetDepthStencilStateDesc(handle);
}

VertexBufferHandle 
Device::CreateVertexBuffer(D3D10_USAGE usage, UINT CPUAccessFlags, size_t sizeVertex, size_t numVertices, void* pInitialData)
{
	return _core->CreateVertexBuffer(usage, CPUAccessFlags, sizeVertex, numVertices, pInitialData);
}

void 
Device::ReleaseVertexBuffer(VertexBufferHandle handle)
{
	_core->ReleaseVertexBuffer(handle);
}

//const D3D10_BUFFER_DESC* 
//Device::GetVertexBufferDesc(VertexBufferHandle handle) const
//{
//	return _core->GetVertexBufferDesc(handle);
//}

bool 
Device::GetVertexBufferPtr(VertexBufferHandle handle, BufferPtr& bufferPtr, D3D10_MAP mapType, UINT mapFlags)
{
	return _core->GetVertexBufferPtr(handle, bufferPtr, mapType, mapFlags);
}

IndexBufferHandle 
Device::CreateIndexBuffer(D3D10_USAGE usage, UINT CPUAccessFlags, size_t sizeIndex, size_t numIndices, void* pInitialData)
{
	return _core->CreateIndexBuffer(usage, CPUAccessFlags, sizeIndex, numIndices, pInitialData);
}

void 
Device::ReleaseIndexBuffer(IndexBufferHandle handle)
{
	_core->ReleaseIndexBuffer(handle);
}

//const D3D10_BUFFER_DESC* 
//Device::GetIndexBufferDesc(IndexBufferHandle handle) const
//{
//	return _core->GetIndexBufferDesc(handle);
//}

bool 
Device::GetIndexBufferPtr(IndexBufferHandle handle, BufferPtr& bufferPtr, D3D10_MAP mapType, UINT mapFlags)
{
	return _core->GetIndexBufferPtr(handle, bufferPtr, mapType, mapFlags);
}

ShaderHandle 
Device::CreateShader(
	const TCHAR* path,
	const char* vertexShaderFunction,
	const char* vertexShaderProfile,
	const char* geometryShaderFunction,
	const char* geometryShaderProfile,
	const char* pixelShaderFunction,
	const char* pixelShaderProfile
	)
{
	return _core->CreateShader(
		path,
		vertexShaderFunction,
		vertexShaderProfile,
		geometryShaderFunction,
		geometryShaderProfile,
		pixelShaderFunction,
		pixelShaderProfile
		);
}
void 
Device::ReleaseShader(ShaderHandle handle)
{
	_core->ReleaseShader(handle);
}

TextureHandle
Device::CreateTextureFromFile(const TCHAR* filePath)
{
	return _core->CreateTextureFromFile(filePath);
}

TextureHandle 
Device::CreateTextureArrayFromFiles(const TCHAR** filePaths, unsigned int numFilePaths)
{
	return _core->CreateTextureArrayFromFiles(filePaths, numFilePaths);
}

SamplerStateHandle
Device::CreateSamplerState(const D3D10_SAMPLER_DESC& desc)
{
	return _core->CreateSamplerState(desc);
}

void
Device::ReleaseSamplerState(SamplerStateHandle handle)
{
	_core->ReleaseSamplerState(handle);
}

const D3D10_SAMPLER_DESC*
Device::GetSamplerStateDesc(SamplerStateHandle handle) const
{
	return _core->GetSamplerStateDesc(handle);
}

FontHandle 
Device::CreateFont(const FontDesc& desc)
{
	return _core->CreateFont(desc);
}

void 
Device::ReleaseFont(FontHandle handle)
{
	_core->ReleaseFont(handle);
}

const FontDesc* 
Device::GetFontDesc(FontHandle handle) const
{
	return _core->GetFontDesc(handle);
}

INT 
Device::GetFontHeight(FontHandle handle) const
{
	return _core->GetFontHeight(handle);
}
