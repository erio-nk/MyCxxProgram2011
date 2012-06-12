/*!
	@file	Graphics/Direct3D10/Device.cpp

	@author	E.Nakayama
*/

#include "Marda/Graphics/Direct3D10.h"
#include "Marda/Graphics/Direct3D10/Core.h"
#include "Marda/Graphics/Direct3D10/Shader.h"

using namespace Marda;
using namespace Direct3D10;

// ----------------------------------------------------------------------------
Renderer::Renderer()
	: _core(NULL)
	, _device(NULL)
{
}

Renderer::~Renderer()
{
	// �f�X�g���N�^�ł́A���z�֐��ł��� Release �֐���K�؂ɌĂяo���Ȃ��ׁA
	// �{�N���X�ō쐬�������݂̂̂��������B�T�u�N���X�̃f�X�g���N�^�� Release �֐����Ăяo����Ă����͖����B
	_Release();
}

//! �I�������B
void 
Renderer::Release()
{
	_Release();
}

void
Renderer::_Release()
{
	if (_blendStateHandleDefault != BlendStateHandle::INVALID)
	{
		_core->ReleaseBlendState(_blendStateHandleDefault);
		_blendStateHandleDefault = BlendStateHandle::INVALID;
	}
	if (_rasterizerStateHandleDefault != RasterizerStateHandle::INVALID)
	{
		_core->ReleaseRasterizerState(_rasterizerStateHandleDefault);
		_rasterizerStateHandleDefault = DepthStencilStateHandle::INVALID;
	}
	if (_depthStencilStateHandleDefault != DepthStencilStateHandle::INVALID)
	{
		_core->ReleaseDepthStencilState(_depthStencilStateHandleDefault);
		_depthStencilStateHandleDefault = DepthStencilStateHandle::INVALID;
	}
	if (_fontHandleDefault != FontHandle::INVALID)
	{
		_core->ReleaseFont(_fontHandleDefault);
		_fontHandleDefault = FontHandle::INVALID;
	}
	_device = NULL;
	_core = NULL;
}
	
//! �����������B
bool 
Renderer::Create(Device& device)
{
	_core = &device.GetCore();
	_device = &device;
	
	_renderTargetHandleDefault = _core->GetBackBuffer();
	_depthStencilBufferHandleDefault = _core->GetDepthStencilBuffer();
	SetDefaultRenderTargets();
	
	// �f�t�H���g�̃X�e�[�g�I�u�W�F�N�g�̍쐬�B
	if (failed(CreateDefaultStates()))
		return false;
	
	// �f�t�H���g�̃t�H���g�I�u�W�F�N�g�̍쐬�B
	if (failed(CreateDefaultFont()))
		return false;
	
	return true;
}

//! �f�t�H���g�X�e�[�g�̍쐬�B
bool 
Renderer::CreateDefaultStates()
{
	// �f�t�H���g�̃u�����f�B���O�X�e�[�g�̍쐬�B
	D3D10_BLEND_DESC bsDesc;
	bsDesc.AlphaToCoverageEnable = FALSE;
	for (size_t i = 0; i < _countof(bsDesc.BlendEnable); ++i)
	{
		bsDesc.BlendEnable[i] = FALSE;
	}
	bsDesc.SrcBlend = D3D10_BLEND_ONE;
	bsDesc.DestBlend = D3D10_BLEND_ZERO;
	bsDesc.BlendOp = D3D10_BLEND_OP_ADD;
	bsDesc.SrcBlendAlpha = D3D10_BLEND_ONE;
	bsDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
	bsDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
	for (size_t i = 0; i < _countof(bsDesc.RenderTargetWriteMask); ++i)
	{
		bsDesc.RenderTargetWriteMask[i] = D3D10_COLOR_WRITE_ENABLE_ALL;
	}
	_blendStateHandleDefault = _core->CreateBlendState(bsDesc);
	SetDefaultBlendState();

	// �f�t�H���g�̃��X�^���C�U�X�e�[�g�̍쐬�B
	D3D10_RASTERIZER_DESC rsDesc;
	rsDesc.FillMode = D3D10_FILL_SOLID;
	rsDesc.CullMode = D3D10_CULL_BACK;
	rsDesc.FrontCounterClockwise = FALSE;
	rsDesc.DepthBias = 0;
	rsDesc.DepthBiasClamp = 0.f;
	rsDesc.SlopeScaledDepthBias = 0.f;
	rsDesc.DepthClipEnable = TRUE;
	rsDesc.ScissorEnable = FALSE;
	rsDesc.MultisampleEnable = FALSE;
	rsDesc.AntialiasedLineEnable = FALSE;
	_rasterizerStateHandleDefault = _core->CreateRasterizerState(rsDesc);
	SetDefaultRasterizerState();

	// �f�t�H���g�̐[�x�X�e���V���X�e�[�g�̍쐬�B
	D3D10_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D10_COMPARISON_LESS;
	dsDesc.StencilEnable = FALSE;
	dsDesc.StencilReadMask = D3D10_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D10_DEFAULT_STENCIL_WRITE_MASK;
	dsDesc.FrontFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D10_COMPARISON_ALWAYS;
	dsDesc.BackFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D10_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D10_COMPARISON_ALWAYS;
	_depthStencilStateHandleDefault = _core->CreateDepthStencilState(dsDesc);
	if (_depthStencilStateHandleDefault == DepthStencilStateHandle::INVALID)
		return false;
	SetDefaultDepthStencilState();
	
	return true;
}

bool 
Renderer::CreateDefaultFont()
{
	FontDesc desc;
	desc.Height = 16;
	desc.Width = 0;
	desc.Weight = FW_NORMAL;
	desc.MipLevels = 1;
	desc.Italic = FALSE;
	desc.CharSet = DEFAULT_CHARSET;
	desc.OutputPrecision = OUT_DEFAULT_PRECIS;
	desc.Quality = DEFAULT_QUALITY;
	desc.PitchAndFamily = DEFAULT_PITCH || FF_DONTCARE;
	desc.pFaceName = _T("Arial");
	_fontHandleDefault = _core->CreateFont(desc);
	if (_fontHandleDefault == FontHandle::INVALID)
		return false;
	return true;
}

const Device*
Renderer::GetDevice() const
{
	return _device;
}

Device::NativeDevice*
Renderer::GetNativeDevice()
{
	return _device->GetNativeDevice();
}

//! �t���[���o�b�t�@�̓��e����ʂɏo�͂���B
HRESULT 
Renderer::Present()
{
	return _core->Present();
}

//! �f�t�H���g�̕`��^�[�Q�b�g���擾����B
RenderTargetHandle 
Renderer::GetDefaultRenderTarget()
{
	return _renderTargetHandleDefault;
}

//! �f�t�H���g�̐[�x�X�e���V���o�b�t�@���擾����B
DepthStencilBufferHandle 
Renderer::GetDefaultDepthStencilBuffer()
{
	return _depthStencilBufferHandleDefault;
}
		
//! ���݂̕`��^�[�Q�b�g���N���A����B
void 
Renderer::Clear(bool clearColor, bool clearDepth, bool clearStencil, const Color& color, float depth, uint8 stencil)
{
	if (clearColor)
	{
		_core->ClearRenderTarget(_renderTargetHandleCurrent, color);
	}
	
	DWORD clearDepthStencilFlag = 0;
	if (clearDepth) 
		clearDepthStencilFlag |= D3D10_CLEAR_DEPTH;
	if (clearStencil) 
		clearDepthStencilFlag |= D3D10_CLEAR_STENCIL;
	//if (clearDepthStencilFlag > 0)
	{
		_core->ClearDepthStencil(
			_depthStencilBufferHandleCurrent,
			clearDepthStencilFlag,
			depth,
			stencil
			);
	}
}
	
bool 
Renderer::SetRenderTargets(RenderTargetHandle renderTargetHandle, DepthStencilBufferHandle depthStencilBufferHandle)
{
	_renderTargetHandleCurrent = renderTargetHandle;
	_depthStencilBufferHandleCurrent = depthStencilBufferHandle;
	_core->SetRenderTargets(_renderTargetHandleCurrent, _depthStencilBufferHandleCurrent);
	return true;
}
	
void 
Renderer::SetDefaultRenderTargets()
{
	SetRenderTargets(_renderTargetHandleDefault, _depthStencilBufferHandleDefault);
}

void 
Renderer::SetViewport(SIZE sz)
{
	_core->SetViewport(sz);
}
	
// ----------------------------------------------------------------------------
// ���̓��C�A�E�g�֘A�B
void 
Renderer::SetInputLayout(InputLayoutHandle handle)
{
	_core->SetInputLayout(handle);
}

void 
Renderer::ClearInputLayout()
{
	_core->ClearInputLayout();
}

// ----------------------------------------------------------------------------
// �u�����h�X�e�[�g�֘A�B
void 
Renderer::SetBlendState(BlendStateHandle handle)
{
	_core->SetBlendState(handle);
}

void 
Renderer::SetDefaultBlendState()
{
	SetBlendState(_blendStateHandleDefault);
}

BlendStateHandle 
Renderer::GetDefaultBlendState() const
{
	return _blendStateHandleDefault;
}

// ----------------------------------------------------------------------------
// ���X�^���C�U�X�e�[�g�֘A�B
void 
Renderer::SetRasterizerState(RasterizerStateHandle handle)
{
	_core->SetRasterizerState(handle);
}

void 
Renderer::SetDefaultRasterizerState()
{
	SetRasterizerState(_rasterizerStateHandleDefault);
}

RasterizerStateHandle 
Renderer::GetDefaultRasterizerState() const
{
	return _rasterizerStateHandleDefault;
}

// ----------------------------------------------------------------------------
// �[�x�X�e���V���X�e�[�g�֘A�B
void 
Renderer::SetDepthStencilState(DepthStencilStateHandle handle)
{
	_core->SetDepthStencilState(handle);
}

void 
Renderer::SetDefaultDepthStencilState()
{
	SetDepthStencilState(_depthStencilStateHandleDefault);
}

DepthStencilStateHandle 
Renderer::GetDefaultDepthStencilState() const
{
	return _depthStencilStateHandleDefault;
}

// ----------------------------------------------------------------------------
// ���_�o�b�t�@�֘A�B
void 
Renderer::SetVertexBuffer(VertexBufferHandle handle)
{
	_core->SetVertexBuffer(0, handle);
}

void 
Renderer::SetVertexBuffer(unsigned int slot, VertexBufferHandle handle)
{
	_core->SetVertexBuffer(slot, handle);
}

void
Renderer::UpdateVertexBuffer(VertexBufferHandle handle, void* pNewData)
{
	_core->UpdateVertexBuffer(handle, pNewData);
}

// ----------------------------------------------------------------------------
// �C���f�b�N�X�o�b�t�@�֘A�B
void 
Renderer::SetIndexBuffer(IndexBufferHandle handle)
{
	_core->SetIndexBuffer(handle);
}
	
// ----------------------------------------------------------------------------
// �V�F�[�_�֘A�B
void 
Renderer::SetShader(ShaderHandle handle)
{
	_shaderHandleCurrent = handle;
}
	
void 
Renderer::ClearShader()
{
	_shaderHandleCurrent = ShaderHandle::INVALID;
	_core->ClearShader();
}

void 
Renderer::ApplyShader()
{
	_core->ApplyShader(_shaderHandleCurrent);
}
	
// ----------------------------------------------------------------------------
// �V�F�[�_�萔�o�b�t�@�֘A�B
unsigned int
Renderer::GetShaderConstantBufferIndex(ShaderHandle handle, const char* cbufName) const
{
	return _core->GetShaderConstantBufferIndex(handle, cbufName);
}

unsigned int
Renderer::GetShaderConstantBufferVariableIndex(ShaderHandle handle, unsigned int cbufIndex, const char* varName) const
{	
	return _core->GetShaderConstantBufferVariableIndex(handle, cbufIndex, varName);
}

unsigned int
Renderer::GetShaderConstantBufferVariableIndex(ShaderHandle handle, const char* cbufName, const char* varName) const
{
	unsigned int cbufIndex = _core->GetShaderConstantBufferIndex(handle, cbufName);
	return _core->GetShaderConstantBufferVariableIndex(handle, cbufIndex, varName);
}

unsigned int
Renderer::GetShaderGlobalVariableIndex(ShaderHandle handle, const char* varName) const
{	
	return _core->GetShaderGlobalVariableIndex(handle, varName);
}

void*
Renderer::GetShaderConstantBuffer(ShaderHandle handle, unsigned int cbufIndex)
{	
	return _core->GetShaderConstantBuffer(handle, cbufIndex);
}

void*
Renderer::GetShaderConstantBuffer(ShaderHandle handle, const char* cbufName)
{	
	unsigned int cbufIndex = _core->GetShaderConstantBufferIndex(handle, cbufName);
	return _core->GetShaderConstantBuffer(handle, cbufIndex);
}

void*
Renderer::GetShaderConstantBufferVariable(ShaderHandle handle, unsigned int cbufIndex, unsigned int varIndex)
{	
	return _core->GetShaderConstantBufferVariable(handle, cbufIndex, varIndex);
}

void*
Renderer::GetShaderConstantBufferVariable(ShaderHandle handle, unsigned int cbufIndex, const char* varName)
{	
	unsigned int varIndex = _core->GetShaderConstantBufferVariableIndex(handle, cbufIndex, varName);
	return _core->GetShaderConstantBufferVariable(handle, cbufIndex, varIndex);
}

void*
Renderer::GetShaderConstantBufferVariable(ShaderHandle handle, const char* cbufName, unsigned int varIndex)
{	
	unsigned int cbufIndex = _core->GetShaderConstantBufferIndex(handle, cbufName);
	return _core->GetShaderConstantBufferVariable(handle, cbufIndex, varIndex);
}

void*
Renderer::GetShaderConstantBufferVariable(ShaderHandle handle, const char* cbufName, const char* varName)
{	
	unsigned int cbufIndex = _core->GetShaderConstantBufferIndex(handle, cbufName);
	unsigned int varIndex = _core->GetShaderConstantBufferVariableIndex(handle, cbufIndex, varName);
	return _core->GetShaderConstantBufferVariable(handle, cbufIndex, varIndex);
}

void*
Renderer::GetShaderGlobalVariable(ShaderHandle handle, unsigned int varIndex)
{	
	return _core->GetShaderGlobalVariable(handle, varIndex);
}

void*
Renderer::GetShaderGlobalVariable(ShaderHandle handle, const char* varName)
{	
	unsigned int varIndex = _core->GetShaderGlobalVariableIndex(handle, varName);
	return _core->GetShaderGlobalVariable(handle, varIndex);
}

// ----------------------------------------------------------------------------
// �e�N�X�`���֘A�B
void 
Renderer::SetShaderTexture(ShaderHandle shaderHandle, unsigned int texIndex, TextureHandle textureHandle)
{
	_core->SetShaderTexture(shaderHandle, texIndex, textureHandle);
}

void 
Renderer::SetShaderTexture(ShaderHandle shaderHandle, const char* texName, TextureHandle textureHandle)
{
	_core->SetShaderTexture(shaderHandle, texName, textureHandle);
}

void 
Renderer::SetShaderTexture(ShaderHandle shaderHandle, const char* texName, unsigned int indexInArray, TextureHandle textureHandle)
{
	_core->SetShaderTexture(shaderHandle, texName, indexInArray, textureHandle);
}

void 
Renderer::SetShaderTextureArray(ShaderHandle shaderHandle, unsigned int texArrayIndex, TextureHandle textureArrayHandle)
{
	_core->SetShaderTextureArray(shaderHandle, texArrayIndex, textureArrayHandle);
}

void 
Renderer::SetShaderTextureArray(ShaderHandle shaderHandle, const char* texArrayName, TextureHandle textureArrayHandle)
{
	_core->SetShaderTextureArray(shaderHandle, texArrayName, textureArrayHandle);
}

void
Renderer::SetRenderTargetToShaderTexture(ShaderHandle shaderHandle, unsigned int texIndex, RenderTargetHandle renderTargetHandle)
{
	_core->SetRenderTargetToShaderTexture(shaderHandle, texIndex, renderTargetHandle);
}

void
Renderer::SetRenderTargetToShaderTexture(ShaderHandle shaderHandle, const char* texName, RenderTargetHandle renderTargetHandle)
{
	_core->SetRenderTargetToShaderTexture(shaderHandle, texName, renderTargetHandle);
}

void
Renderer::SetDepthStencilBufferToShaderTexture(ShaderHandle shaderHandle, unsigned int texIndex, DepthStencilBufferHandle depthStencilBufferHandle)
{
	_core->SetDepthStencilBufferToShaderTexture(shaderHandle, texIndex, depthStencilBufferHandle);
}

void
Renderer::SetDepthStencilBufferToShaderTexture(ShaderHandle shaderHandle, const char* texName, DepthStencilBufferHandle depthStencilBufferHandle)
{
	_core->SetDepthStencilBufferToShaderTexture(shaderHandle, texName, depthStencilBufferHandle);
}

// ----------------------------------------------------------------------------
// �T���v���[�X�e�[�g�֘A�B
void 
Renderer::SetShaderSamplerState(ShaderHandle shaderHandle, unsigned int sampIndex, SamplerStateHandle samplerHandle)
{
	_core->SetShaderSamplerState(shaderHandle, sampIndex, samplerHandle);
}

void 
Renderer::SetShaderSamplerState(ShaderHandle shaderHandle, const char* sampName, SamplerStateHandle samplerHandle)
{
	_core->SetShaderSamplerState(shaderHandle, sampName, samplerHandle);
}

void 
Renderer::SetShaderSamplerState(ShaderHandle shaderHandle, const char* sampName, unsigned int indexInArray, SamplerStateHandle samplerHandle)
{
	_core->SetShaderSamplerState(shaderHandle, sampName, indexInArray, samplerHandle);
}

	
// ----------------------------------------------------------------------------
// �t�H���g�֘A�B
FontHandle 
Renderer::GetDefaultFont() const
{
	return _fontHandleDefault;
}

void 
Renderer::DrawText(FontHandle handle, int x, int y, const Color& color, const TCHAR* format, ...)
{
	SetDefaultRasterizerState();
	va_list va;
	va_start(va, format);
	_core->DrawText(handle, x, y, color, format, va);
	va_end(va);
}

void 
Renderer::DrawText(int x, int y, const Color& color, const TCHAR* format, ...)
{
	SetDefaultRasterizerState();
	va_list va;
	va_start(va, format);
	_core->DrawText(_fontHandleDefault, x, y, color, format, va);
	va_end(va);
}
	

