/*!
	@file	Graphics/Direct3D10/Device.cpp

	@author	E.Nakayama
*/

#include "Marda/Graphics/Color.h"
#include "Marda/Graphics/Direct3D10/Core.h"

using namespace Marda;
using namespace Direct3D10;
using namespace D3DXHelpers;

namespace
{
	inline bool IsObjectInvalid(void* pObject)
	{
		return is_null(pObject);
	}

	inline bool IsObjectInvalid(const RenderTarget& obj)
	{
		return is_null(obj._pRenderTargetView);
	}

	inline bool IsObjectInvalid(const DepthStencilBuffer& obj)
	{
		return is_null(obj._pDepthStencilView);
	}

	inline bool IsObjectInvalid(const VertexBuffer& obj)
	{
		return is_null(obj._pInterface);
	}

	inline bool IsObjectInvalid(const IndexBuffer& obj)
	{
		return is_null(obj._pInterface);
	}

	inline bool IsObjectInvalid(const Shader& shader)
	{
		return is_null(shader.GetVertexShaderBytecode());
	}

	inline bool IsObjectInvalid(const Font& obj)
	{
		return is_null(obj._pInterface);
	}

	template <class Handle, class ObjectList>
	inline bool ValidateHandle(Handle handle, const ObjectList& objects)
	{
		if (static_cast<int>(objects.size()) <= handle.Value() || handle.Value() < 0 || IsObjectInvalid(objects[handle.Value()]))
			#if defined(_DEBUG)
			throw InvalidHandleException();
			#else
			return false;
			#endif
		return true;
	}
	
	template <class Object, class List>
	int Push(const Object& obj, List& list)
	{
		list.push_back(obj);
		return static_cast<int>(list.size() - 1);
	}

}	// end namespace

// ----------------------------------------------------------------------------
Core::Core()
{
	_textures.reserve(256);
	_renderTargets.reserve(4);
	_depthStencilBuffers.reserve(1);
	_inputLayouts.reserve(1);
	_shaders.reserve(4);
	_fonts.reserve(2);
}

Core::~Core()
{
	Release();
}

// 終了処理。
void
Core::Release()
{
	std::for_each(_fonts.begin(), _fonts.end(), std::mem_fun_ref(&Font::Release));
	_fonts.clear();

	std::for_each(_shaders.begin(), _shaders.end(), std::mem_fun_ref(&Shader::Release));
	_shaders.clear();
	std::for_each(_textures.begin(), _textures.end(), safe_release<ID3D10ShaderResourceView>);
	_textures.clear();
	std::for_each(_samplerStates.begin(), _samplerStates.end(), safe_release<ID3D10SamplerState>);
	_samplerStates.clear();

	std::for_each(_vertexBuffers.begin(), _vertexBuffers.end(), std::mem_fun_ref(&VertexBuffer::Release));
	_vertexBuffers.clear();
	std::for_each(_indexBuffers.begin(), _indexBuffers.end(), std::mem_fun_ref(&IndexBuffer::Release));
	_indexBuffers.clear();

	std::for_each(_blendStates.begin(), _blendStates.end(), safe_release<ID3D10BlendState>);
	_blendStates.clear();
	std::for_each(_rasterizerStates.begin(), _rasterizerStates.end(), safe_release<ID3D10RasterizerState>);
	_rasterizerStates.clear();
	std::for_each(_depthStencilStates.begin(), _depthStencilStates.end(), safe_release<ID3D10DepthStencilState>);
	_depthStencilStates.clear();

	std::for_each(_renderTargets.begin(), _renderTargets.end(), std::mem_fun_ref(&RenderTarget::Release));
	_renderTargets.clear();
	std::for_each(_depthStencilBuffers.begin(), _depthStencilBuffers.end(), std::mem_fun_ref(&DepthStencilBuffer::Release));
	_depthStencilBuffers.clear();

	std::for_each(_inputLayouts.begin(), _inputLayouts.end(), safe_release<ID3D10InputLayout>);
	_inputLayouts.clear();
	
	safe_release(_pSwapChain);
	safe_release(_pDevice);
}

// バックバッファ、深度ステンシル絡みの解放処理。
void
Core::ReleaseBackBufferAndDepthStencil()
{
	if (_backBufferHandle != RenderTargetHandle::INVALID)
	{
		_renderTargets[_backBufferHandle.Value()].Release();
	}
	if (_depthStencilBufferHandle != DepthStencilBufferHandle::INVALID)
	{
		_depthStencilBuffers[_depthStencilBufferHandle.Value()].Release();
	}
}

// 初期化処理。
bool
Core::Create(HWND hWnd)
{
	// デバイスとスワップチェインの作成。
	RECT rc;
	SIZE sz;
	GetClientRect(hWnd, &rc);
	sz.cx = rc.right - rc.left;
	sz.cy = rc.bottom - rc.top;

#if 1
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = sz.cx;
	sd.BufferDesc.Height = sz.cy;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 1;
	sd.Windowed = TRUE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr = D3D10CreateDeviceAndSwapChain(
		NULL,
		D3D10_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		D3D10_SDK_VERSION,
		&sd,
		&_pSwapChain,
		&_pDevice
		);
	if (FAILED(hr))
	{
		return false;
	}
#else
	// TODO: デバイスとスワップチェインを分けて作ってアンチエイリアスの指定をする。
	HRESULT hr = D3D10CreateDevice(
		NULL,
		D3D10_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		D3D10_SDK_VERSION,
		&_pDevice
		);
	if (FAILED(hr))
	{
		return false;
	}
	
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_SAMPLE_DESC MSAA;
	for (int i = 0; i <= D3D10_MAX_MULTISAMPLE_SAMPLE_COUNT; i++)
	{
		UINT Quality;
		if (SUCCEEDED(_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, i, &Quality))
		{
			if (0 < Quality) 
			{
				MSAA.Count = i;
				MSAA.Quality = Quality - 1;
			}
		}
	}
	
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = sz.cx;
	sd.BufferDesc.Height = sz.cy;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 1;
	sd.Windowed = TRUE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	
#endif

	// バックバッファと深度ステンシルバッファの作成。
	if (failed(CreateBackBufferAndDepthStencil(sz)))
	{
		return false;
	}

	return true;
}

//! バックバッファ、深度ステンシル関連の初期化。
bool
Core::CreateBackBufferAndDepthStencil(SIZE sz)
{
	HRESULT hr;

	// バックバッファの取得とレンダーターゲットビューの作成。
	{
		auto_interface<ID3D10Texture2D> pBackBuffer;
		ID3D10Texture2D* pBackBufferTmp;
		
		hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBufferTmp);
		if (FAILED(hr))
		{
			return false;
		}
		pBackBuffer.reset(pBackBufferTmp);
		
		ID3D10RenderTargetView* pRenderTargetView;
		hr = _pDevice->CreateRenderTargetView(pBackBuffer.get(), NULL, &pRenderTargetView);
		if (FAILED(hr))
		{
			return false;
		}

		if (_backBufferHandle == RenderTargetHandle::INVALID)
		{
			RenderTarget renderTarget;
			renderTarget._pRenderTargetView = pRenderTargetView;
			_backBufferHandle = Push(renderTarget, _renderTargets);
		}
		else
		{
			_renderTargets[_backBufferHandle.Value()]._pRenderTargetView = pRenderTargetView;
		}
	}

	// 深度ステンシルバッファの作成。
	//#define ENABLE_DEFAULT_DEPTH_STENCIL_BUFFER_FOR_SHADER_RESOURCE
	{
		D3D10_TEXTURE2D_DESC descDepth;
		descDepth.Width = sz.cx;
		descDepth.Height = sz.cy;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		#if defined(ENABLE_DEFAULT_DEPTH_STENCIL_BUFFER_FOR_SHADER_RESOURCE)
		descDepth.Format = DXGI_FORMAT_R32_TYPELESS;
		#else
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		#endif
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D10_USAGE_DEFAULT;
		descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
		#if defined(ENABLE_DEFAULT_DEPTH_STENCIL_BUFFER_FOR_SHADER_RESOURCE)
		descDepth.BindFlags |= D3D10_BIND_SHADER_RESOURCE;
		#endif
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		auto_interface<ID3D10Texture2D> pDepthStencilBuffer;
		ID3D10Texture2D* pDepthStencilBufferTmp;
		hr = _pDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencilBufferTmp);
		if (FAILED(hr))
		{
			return false;
		}
		pDepthStencilBuffer.reset(pDepthStencilBufferTmp);
		
		// 深度ステンシルビューの作成。
		D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		auto_interface<ID3D10DepthStencilView> pDepthStencilView_ScopeGuard;
		{
			ID3D10DepthStencilView* pDepthStencilViewTmp;
			hr = _pDevice->CreateDepthStencilView(pDepthStencilBuffer.get(), &descDSV, &pDepthStencilViewTmp);
			if (FAILED(hr))
			{
				return false;
			}
			pDepthStencilView_ScopeGuard.reset(pDepthStencilViewTmp);
		}
		
		// シェーダリソースビューの作成。
		D3D10_SHADER_RESOURCE_VIEW_DESC descSRV;
		ZeroMemory(&descSRV, sizeof(descSRV));
		descSRV.Format = DXGI_FORMAT_R32_FLOAT;
		descSRV.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
		descSRV.Texture2D.MostDetailedMip = 0;
		descSRV.Texture2D.MipLevels = -1;//1;
		auto_interface<ID3D10ShaderResourceView> pShaderResourceView_ScopeGuard;
		#if defined(ENABLE_DEFAULT_DEPTH_STENCIL_BUFFER_FOR_SHADER_RESOURCE)
		{
			ID3D10ShaderResourceView* pShaderResourceViewTmp;
			hr = _pDevice->CreateShaderResourceView(pDepthStencilBuffer.get(), &descSRV, &pShaderResourceViewTmp);
			if (FAILED(hr))
			{
				return false;
			}
			pShaderResourceView_ScopeGuard.reset(pShaderResourceViewTmp);
		}
		#endif
		
		if (_depthStencilBufferHandle == DepthStencilBufferHandle::INVALID)
		{
			DepthStencilBuffer depthStencilBuffer;
			depthStencilBuffer._pDepthStencilView = pDepthStencilView_ScopeGuard.release();
			depthStencilBuffer._pShaderResourceView = pShaderResourceView_ScopeGuard.release();
			_depthStencilBufferHandle = Push(depthStencilBuffer, _depthStencilBuffers);
		}
		else
		{
			DepthStencilBuffer& depthStencilBuffer = _depthStencilBuffers[_depthStencilBufferHandle.Value()];
			depthStencilBuffer._pDepthStencilView = pDepthStencilView_ScopeGuard.release();
			depthStencilBuffer._pShaderResourceView = pShaderResourceView_ScopeGuard.release();
		}
	}
	
	// ビューポートの設定。
	SetViewport(sz);
	
	return true;
}

//! フレームバッファの内容を画面に出力する。
HRESULT 
Core::Present()
{
	HRESULT hr;
	if (_flags.is(SCREEN_OCCLUDED))
	{
		// 画面が隠蔽されてるので、画面への描画は行わない。
		hr = _pSwapChain->Present(0, DXGI_PRESENT_TEST);
	}
	else
	{
		// 描画する。
		hr = _pSwapChain->Present(0, 0);
	}
	
	if (hr == DXGI_STATUS_OCCLUDED)
	{
		_flags.set(SCREEN_OCCLUDED);
	}
	else
	{
		_flags.reset(SCREEN_OCCLUDED);
		switch (hr)
		{
			case S_OK:
				_flags.reset(DEVICE_REMOVED | DEVICE_ERROR | UNKNOWN_ERROR);
				break;
			case DXGI_ERROR_DEVICE_HUNG:
			case DXGI_ERROR_DEVICE_RESET:
				_flags.set(DEVICE_ERROR);
				break;
			case DXGI_ERROR_DEVICE_REMOVED:
				_flags.set(DEVICE_REMOVED);
				break;
			case DXGI_ERROR_DRIVER_INTERNAL_ERROR:
			default:
				_flags.set(UNKNOWN_ERROR);
				break;
		}
	}
	return hr;
}
	
bool
Core::IsDeviceRemoved() const
{
	return _flags.is(DEVICE_REMOVED);
}

bool
Core::IsScreenOccluded() const
{
	return _flags.is(SCREEN_OCCLUDED);
}

ID3D10Device* 
Core::GetNativeDevice()
{
	_pDevice->AddRef();
	return _pDevice;
}


RenderTargetHandle
Core::GetBackBuffer() const
{
	return _backBufferHandle;
}

DepthStencilBufferHandle
Core::GetDepthStencilBuffer() const
{
	return _depthStencilBufferHandle;
}

bool
Core::ResizeBackBuffer(SIZE sz)
{
	if (is_not_null(_pDevice))
	{
		_pDevice->OMSetRenderTargets(0, NULL, NULL);
		
		ReleaseBackBufferAndDepthStencil();

		_pSwapChain->ResizeBuffers(1, 0, 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		if (failed(CreateBackBufferAndDepthStencil(sz)))
		{
			return false;
		}
	}
	return true;
}

void
Core::SetViewport(SIZE sz)
{
	D3D10_VIEWPORT viewport[1];
	viewport[0].TopLeftX = 0;
	viewport[0].TopLeftY = 0;
	viewport[0].Width = sz.cx;
	viewport[0].Height = sz.cy;
	viewport[0].MinDepth = 0.f;
	viewport[0].MaxDepth = 1.f;
	_pDevice->RSSetViewports(1, viewport);
}


// ------------------------------------------------------------------------
// 描画ターゲット関連。
RenderTargetHandle
Core::CreateRenderTarget(unsigned int width, unsigned int height, DXGI_FORMAT format, bool useShaderResource)
{
	HRESULT hr;
	if (width == 0 || height == 0)
	{
		// バックバッファのサイズで作成する。
		ID3D10Texture2D* pBackBuffer;
		hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr))
		{
			return RenderTargetHandle::INVALID;
		}
		D3D10_TEXTURE2D_DESC desc;
		pBackBuffer->GetDesc(&desc);
		if (width == 0)
			width = desc.Width;
		if (height == 0)
			height = desc.Height;
		safe_release(pBackBuffer);
	}
	
	D3D10_TEXTURE2D_DESC descTex;
	descTex.Width = width;
	descTex.Height = height;
	descTex.MipLevels = 1;	// TODO: mipLevels 指定版。
	descTex.ArraySize = 1;
	descTex.Format = format;
	descTex.SampleDesc.Count = 1;
	descTex.SampleDesc.Quality = 0;
	descTex.Usage = D3D10_USAGE_DEFAULT;
	descTex.BindFlags = D3D10_BIND_RENDER_TARGET;
	if (useShaderResource)
		descTex.BindFlags |= D3D10_BIND_SHADER_RESOURCE;
	descTex.CPUAccessFlags = 0;
	descTex.MiscFlags = 0;
	
	auto_interface<ID3D10Texture2D> pTexture;
	{
		ID3D10Texture2D* pTextureTmp;
		hr = _pDevice->CreateTexture2D(&descTex, NULL, &pTextureTmp);
		if (FAILED(hr))
		{
			return RenderTargetHandle::INVALID;
		}
		pTexture.reset(pTextureTmp);
	}
	
	// 描画ターゲットビューの作成。
	D3D10_RENDER_TARGET_VIEW_DESC descRTV;
	descRTV.Format = descTex.Format;
	descRTV.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
	descRTV.Texture2D.MipSlice = 0;
	
	RenderTarget renderTarget;
	hr = _pDevice->CreateRenderTargetView(pTexture.get(), &descRTV, &renderTarget._pRenderTargetView);
	if (FAILED(hr))
	{
		return RenderTargetHandle::INVALID;
	}
	auto_interface<ID3D10RenderTargetView> pRenderTargetView_ScopeGuard(renderTarget._pRenderTargetView);
	
	if (useShaderResource)
	{
		// シェーダリソースビューを作成する。
		D3D10_SHADER_RESOURCE_VIEW_DESC descSRV;
		ZeroMemory(&descSRV, sizeof(descSRV));
		descSRV.Format = descTex.Format;
		descSRV.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
		descSRV.Texture2D.MostDetailedMip = 0;
		descSRV.Texture2D.MipLevels = descTex.MipLevels;
		hr = _pDevice->CreateShaderResourceView(pTexture.get(), &descSRV, &renderTarget._pShaderResourceView);
		if (FAILED(hr))
		{
			return TextureHandle::INVALID;
		}
	}
	
	pRenderTargetView_ScopeGuard.release();
	
	return Push(renderTarget, _renderTargets);
}

DepthStencilBufferHandle
Core::CreateDepthStencilBuffer(unsigned int width, unsigned int height, bool useShaderResource)
{
	HRESULT hr;
	if (width == 0 || height == 0)
	{
		// バックバッファのサイズで作成する。
		ID3D10Texture2D* pBackBuffer;
		hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr))
		{
			return RenderTargetHandle::INVALID;
		}
		D3D10_TEXTURE2D_DESC desc;
		pBackBuffer->GetDesc(&desc);
		if (width == 0)
			width = desc.Width;
		if (height == 0)
			height = desc.Height;
		safe_release(pBackBuffer);
	}
	
	D3D10_TEXTURE2D_DESC descDepth;
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	//descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.Format = DXGI_FORMAT_R32_TYPELESS;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D10_USAGE_DEFAULT;
	descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	if (useShaderResource)
		descDepth.BindFlags |= D3D10_BIND_SHADER_RESOURCE;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	auto_interface<ID3D10Texture2D> pDepthStencilBuffer;
	ID3D10Texture2D* pDepthStencilBufferTmp;
	hr = _pDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencilBufferTmp);
	if (FAILED(hr))
	{
		return false;
	}
	pDepthStencilBuffer.reset(pDepthStencilBufferTmp);
	
	// 深度ステンシルビューの作成。
	D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	DepthStencilBuffer depthStencilBuffer;
	hr = _pDevice->CreateDepthStencilView(pDepthStencilBuffer.get(), &descDSV, &depthStencilBuffer._pDepthStencilView);
	if (FAILED(hr))
	{
		return false;
	}
	auto_interface<ID3D10DepthStencilView> pDepthStencilView_ScopeGuard(depthStencilBuffer._pDepthStencilView);
	
	// シェーダリソースビューの作成。
	if (useShaderResource)
	{
		D3D10_SHADER_RESOURCE_VIEW_DESC descSRV;
		ZeroMemory(&descSRV, sizeof(descSRV));
		descSRV.Format = DXGI_FORMAT_R32_FLOAT;
		descSRV.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
		descSRV.Texture2D.MostDetailedMip = 0;
		descSRV.Texture2D.MipLevels = -1;
		hr = _pDevice->CreateShaderResourceView(pDepthStencilBuffer.get(), &descSRV, &depthStencilBuffer._pShaderResourceView);
		if (FAILED(hr))
		{
			return false;
		}
	}
	
	pDepthStencilView_ScopeGuard.release();
	
	return Push(depthStencilBuffer, _depthStencilBuffers);
}

bool
Core::SetRenderTargets(RenderTargetHandle renderTargetHandle, DepthStencilBufferHandle depthStencilBufferHandle)
{
	#if defined(_DEBUG)
	ValidateHandle(renderTargetHandle, _renderTargets);
	ValidateHandle(depthStencilBufferHandle, _depthStencilBuffers);
	#endif
	ID3D10RenderTargetView* pRenderTargetViewCurrent = _renderTargets[renderTargetHandle.Value()]._pRenderTargetView;
	ID3D10DepthStencilView* pDepthStencilViewCurrent = _depthStencilBuffers[depthStencilBufferHandle.Value()]._pDepthStencilView;
	_pDevice->OMSetRenderTargets(
		1,
		&pRenderTargetViewCurrent,
		pDepthStencilViewCurrent
		);
	return true;
}

//! 描画ターゲットをクリアする。
void
Core::ClearRenderTarget(RenderTargetHandle handle, const Color& color)
{
	#if defined(_DEBUG)
	ValidateHandle(handle, _renderTargets);
	#endif
	_pDevice->ClearRenderTargetView(
		_renderTargets[handle.Value()]._pRenderTargetView,
		color.ToFloat4()
		);
}

void
Core::ClearDepthStencil(DepthStencilBufferHandle handle, DWORD clearDepthStencilFlag, float depth, uint8 stencil)
{
	#if defined(_DEBUG)
	ValidateHandle(handle, _depthStencilBuffers);
	#endif
	_pDevice->ClearDepthStencilView(
		_depthStencilBuffers[handle.Value()]._pDepthStencilView,
		clearDepthStencilFlag,
		depth,
		stencil
		);
}


// ------------------------------------------------------------------------
// 入力レイアウト関連。
InputLayoutHandle
Core::CreateInputLayout(D3D10_INPUT_ELEMENT_DESC* inputElementDescs, UINT numInputElements, ShaderHandle shaderHandle)
{
	if (!ValidateHandle(shaderHandle, _shaders))
		return InputLayoutHandle::INVALID;
		
	Shader& shader = _shaders[shaderHandle.Value()];
	ID3D10Blob* pVertexShaderBytecode = shader.GetVertexShaderBytecode();
	
	ID3D10InputLayout* pInputLayout;
	HRESULT hr = _pDevice->CreateInputLayout(
		inputElementDescs,
		numInputElements,
		pVertexShaderBytecode->GetBufferPointer(),
		pVertexShaderBytecode->GetBufferSize(),
		&pInputLayout
		);
	if (FAILED(hr))
	{
		return InputLayoutHandle::INVALID;
	}
	
	return Push(pInputLayout, _inputLayouts);
}

void
Core::ReleaseInputLayout(InputLayoutHandle handle)
{
	if (!ValidateHandle(handle, _inputLayouts))
		return;
	safe_release(_inputLayouts[handle.Value()]);
}

void
Core::SetInputLayout(InputLayoutHandle handle)
{
	#if defined(_DEBUG)
	ValidateHandle(handle, _inputLayouts);
	#endif
	_pDevice->IASetInputLayout(_inputLayouts[handle.Value()]);
}

void
Core::ClearInputLayout()
{
	_pDevice->IASetInputLayout(NULL);
}

// ------------------------------------------------------------------------
// ブレンドステート関連。
BlendStateHandle
Core::CreateBlendState(const D3D10_BLEND_DESC& desc)
{
	ID3D10BlendState* pBlendState;
	HRESULT hr = _pDevice->CreateBlendState(
		&desc,
		&pBlendState
		);
	if (FAILED(hr))
	{
		return BlendStateHandle::INVALID;
	}
	
	return Push(pBlendState, _blendStates);
}

void
Core::ReleaseBlendState(BlendStateHandle handle)
{
	if (!ValidateHandle(handle, _blendStates))
		return;
	safe_release(_blendStates[handle.Value()]);
}

void
Core::SetBlendState(BlendStateHandle handle)
{
	#if defined(_DEBUG)
	ValidateHandle(handle, _blendStates);
	#endif
	float factor[4] = {0.f, 0.f, 0.f, 0.f};
	_pDevice->OMSetBlendState(_blendStates[handle.Value()], factor, 0xffffffff);
}

const D3D10_BLEND_DESC* 
Core::GetBlendStateDesc(BlendStateHandle handle) const
{
	if (!ValidateHandle(handle, _blendStates))
		return NULL;
	static D3D10_BLEND_DESC desc;
	_blendStates[handle.Value()]->GetDesc(&desc);
	return &desc;
}

// ------------------------------------------------------------------------
// ラスタライザステート関連。
RasterizerStateHandle
Core::CreateRasterizerState(const D3D10_RASTERIZER_DESC& desc)
{
	ID3D10RasterizerState* pRasterizerState;
	HRESULT hr = _pDevice->CreateRasterizerState(
		&desc,
		&pRasterizerState
		);
	if (FAILED(hr))
	{
		return RasterizerStateHandle::INVALID;
	}
	
	return Push(pRasterizerState, _rasterizerStates);
}

void
Core::ReleaseRasterizerState(RasterizerStateHandle handle)
{
	if (!ValidateHandle(handle, _rasterizerStates))
		return;
	safe_release(_rasterizerStates[handle.Value()]);
}

void
Core::SetRasterizerState(RasterizerStateHandle handle)
{
	#if defined(_DEBUG)
	ValidateHandle(handle, _rasterizerStates);
	#endif
	_pDevice->RSSetState(_rasterizerStates[handle.Value()]);
}

const D3D10_RASTERIZER_DESC* 
Core::GetRasterizerStateDesc(RasterizerStateHandle handle) const
{
	if (!ValidateHandle(handle, _rasterizerStates))
		return NULL;
	static D3D10_RASTERIZER_DESC desc;
	_rasterizerStates[handle.Value()]->GetDesc(&desc);
	return &desc;
}

// ------------------------------------------------------------------------
// 深度ステンシルステート関連。
DepthStencilStateHandle
Core::CreateDepthStencilState(const D3D10_DEPTH_STENCIL_DESC& desc)
{
	ID3D10DepthStencilState* pDepthStencilState;
	HRESULT hr = _pDevice->CreateDepthStencilState(
		&desc,
		&pDepthStencilState
		);
	if (FAILED(hr))
	{
		return DepthStencilStateHandle::INVALID;
	}
	
	return Push(pDepthStencilState, _depthStencilStates);
}

void
Core::ReleaseDepthStencilState(DepthStencilStateHandle handle)
{
	if (!ValidateHandle(handle, _depthStencilStates))
		return ;
	safe_release(_depthStencilStates[handle.Value()]);
}

void
Core::SetDepthStencilState(DepthStencilStateHandle handle)
{
	#if defined(_DEBUG)
	ValidateHandle(handle, _depthStencilStates);
	#endif
	_pDevice->OMSetDepthStencilState(_depthStencilStates[handle.Value()], 0);
}

const D3D10_DEPTH_STENCIL_DESC* 
Core::GetDepthStencilStateDesc(DepthStencilStateHandle handle) const
{
	if (!ValidateHandle(handle, _depthStencilStates))
		return NULL;
	static D3D10_DEPTH_STENCIL_DESC desc;
	_depthStencilStates[handle.Value()]->GetDesc(&desc);
	return &desc;
}

// ------------------------------------------------------------------------
// 頂点バッファ関連。
VertexBufferHandle
Core::CreateVertexBuffer(D3D10_USAGE usage, UINT CPUAccessFlags, size_t sizeVertex, size_t numVertices, void* pInitialData)
{
	D3D10_BUFFER_DESC desc;
	desc.Usage = usage;
	desc.ByteWidth = static_cast<UINT>(sizeVertex * numVertices);
	desc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = CPUAccessFlags;
	desc.MiscFlags = 0;
	
	D3D10_SUBRESOURCE_DATA subData;
	subData.pSysMem = pInitialData;
	subData.SysMemPitch = 0;
	subData.SysMemSlicePitch = 0;

	VertexBuffer vertexBuffer;
	vertexBuffer._sizeVertex = sizeVertex;

	HRESULT hr = _pDevice->CreateBuffer(&desc, is_null(pInitialData) ? NULL : &subData, &vertexBuffer._pInterface);
	if (FAILED(hr))
		return VertexBufferHandle::INVALID;

	return Push(vertexBuffer, _vertexBuffers);
}

void
Core::ReleaseVertexBuffer(VertexBufferHandle handle)
{
	if (!ValidateHandle(handle, _vertexBuffers))
		return;
	_vertexBuffers[handle.Value()].Release();
}

bool
Core::GetVertexBufferPtr(VertexBufferHandle handle, BufferPtr& bufferPtr, D3D10_MAP mapType, UINT mapFlags)
{
	if (!ValidateHandle(handle, _vertexBuffers))
		return false;
	
	VertexBuffer& vertexBuffer = _vertexBuffers[handle.Value()];
	HRESULT hr = vertexBuffer._pInterface->Map(mapType, mapFlags, (void**)&bufferPtr._ptr);
	if (FAILED(hr))
		return false;
	bufferPtr._pInterface = vertexBuffer._pInterface;
	return true;
}

void
Core::SetVertexBuffer(unsigned int slot, VertexBufferHandle handle)
{
	#if defined(_DEBUG)
	ValidateHandle(handle, _vertexBuffers);
	#endif
	VertexBuffer& vertexBuffer = _vertexBuffers[handle.Value()];
	UINT strides[1] = { static_cast<UINT>(vertexBuffer._sizeVertex) };
	UINT offsets[1] = { 0 };
	_pDevice->IASetVertexBuffers(slot, 1, &vertexBuffer._pInterface, strides, offsets);
}

void
Core::UpdateVertexBuffer(VertexBufferHandle handle, void* pNewData)
{
	#if defined(_DEBUG)
	ValidateHandle(handle, _vertexBuffers);
	#endif
	VertexBuffer& vertexBuffer = _vertexBuffers[handle.Value()];
	_pDevice->UpdateSubresource(vertexBuffer._pInterface, 0, NULL, pNewData, 0, 0);
}

// ------------------------------------------------------------------------
// インデックスバッファ関連。
IndexBufferHandle
Core::CreateIndexBuffer(D3D10_USAGE usage, UINT CPUAccessFlags, size_t sizeIndex, size_t numIndices, void* pInitialData)
{
	DXGI_FORMAT format;
	switch (sizeIndex)
	{
		case 2:
			format = DXGI_FORMAT_R16_UINT;
			break;
		case 4:
			format = DXGI_FORMAT_R32_UINT;
			break;
		default:
			return IndexBufferHandle::INVALID;
	}
	
	IndexBufferHandle ret;
	
	D3D10_BUFFER_DESC desc;
	desc.Usage = usage;
	desc.ByteWidth = static_cast<UINT>(sizeIndex * numIndices);
	desc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = CPUAccessFlags;
	desc.MiscFlags = 0;
	
	D3D10_SUBRESOURCE_DATA subData;
	subData.pSysMem = pInitialData;
	subData.SysMemPitch = 0;
	subData.SysMemSlicePitch = 0;

	IndexBuffer indexBuffer;
	indexBuffer._format = format;

	HRESULT hr = _pDevice->CreateBuffer(&desc, is_null(pInitialData) ? NULL : &subData, &indexBuffer._pInterface);
	if (FAILED(hr))
		return IndexBufferHandle::INVALID;

	return Push(indexBuffer, _indexBuffers);
}

void
Core::ReleaseIndexBuffer(IndexBufferHandle handle)
{
	if (!ValidateHandle(handle, _indexBuffers))
		return;
	_indexBuffers[handle.Value()].Release();
}

bool
Core::GetIndexBufferPtr(IndexBufferHandle handle, BufferPtr& bufferPtr, D3D10_MAP mapType, UINT mapFlags)
{
	if (!ValidateHandle(handle, _indexBuffers))
		return false;
	
	IndexBuffer& indexBuffer = _indexBuffers[handle.Value()];
	HRESULT hr = indexBuffer._pInterface->Map(mapType, mapFlags, (void**)&bufferPtr._ptr);
	if (FAILED(hr))
		return false;
	bufferPtr._pInterface = indexBuffer._pInterface;
	return true;
}

void
Core::SetIndexBuffer(IndexBufferHandle handle)
{
	#if defined(_DEBUG)
	ValidateHandle(handle, _indexBuffers);
	#endif
	IndexBuffer& indexBuffer = _indexBuffers[handle.Value()];
	_pDevice->IASetIndexBuffer(indexBuffer._pInterface, indexBuffer._format, 0);
}

// ------------------------------------------------------------------------
// シェーダ関連。
ShaderHandle
Core::CreateShader(
	const TCHAR* path,
	const char* vertexShaderFunction,
	const char* vertexShaderProfile,
	const char* geometryShaderFunction,
	const char* geometryShaderProfile,
	const char* pixelShaderFunction,
	const char* pixelShaderProfile
	)
{
	Shader shader;
	if (failed(shader.Create(
		_pDevice,
		path,
		vertexShaderFunction,
		vertexShaderProfile,
		geometryShaderFunction,
		geometryShaderProfile,
		pixelShaderFunction,
		pixelShaderProfile
		)))
	{
		return ShaderHandle::INVALID;
	}

	return Push(shader, _shaders);
}

void
Core::ReleaseShader(ShaderHandle handle)
{
	if (!ValidateHandle(handle, _shaders))
		return;
	_shaders[handle.Value()].Release();
}

void
Core::ClearShader()
{
	_pDevice->VSSetShader(NULL);
	_pDevice->GSSetShader(NULL);
	_pDevice->PSSetShader(NULL);
}

void
Core::ApplyShader(ShaderHandle handle)
{
	#if defined(_DEBUG)
	ValidateHandle(handle, _shaders);
	#endif
	Shader& shader = _shaders[handle.Value()];		
	shader.Apply(_pDevice);
}

// ------------------------------------------------------------------------
// シェーダ定数バッファ関連。
unsigned int
Core::GetShaderConstantBufferIndex(ShaderHandle handle, const char* cbufName) const
{	
	#if defined(_DEBUG)
	ValidateHandle(handle, _shaders);
	#endif
	const Shader& shader = _shaders[handle.Value()];
	return shader.GetConstantBufferIndex(cbufName);
}

unsigned int
Core::GetShaderConstantBufferVariableIndex(ShaderHandle handle, unsigned int cbufIndex, const char* varName) const
{	
	#if defined(_DEBUG)
	ValidateHandle(handle, _shaders);
	#endif
	const Shader& shader = _shaders[handle.Value()];
	return shader.GetConstantBufferVariableIndex(cbufIndex, varName);
}

unsigned int
Core::GetShaderGlobalVariableIndex(ShaderHandle handle, const char* varName) const
{	
	#if defined(_DEBUG)
	ValidateHandle(handle, _shaders);
	#endif
	const Shader& shader = _shaders[handle.Value()];
	return shader.GetGlobalVariableIndex(varName);
}

void* 
Core::GetShaderConstantBuffer(ShaderHandle handle, unsigned int cbufIndex)
{	
	#if defined(_DEBUG)
	ValidateHandle(handle, _shaders);
	#endif
	Shader& shader = _shaders[handle.Value()];
	return shader.GetConstantBuffer(cbufIndex);
}

void* 
Core::GetShaderConstantBufferVariable(ShaderHandle handle, unsigned int cbufIndex, unsigned int varIndex)
{	
	#if defined(_DEBUG)
	ValidateHandle(handle, _shaders);
	#endif
	Shader& shader = _shaders[handle.Value()];
	return shader.GetConstantBufferVariable(cbufIndex, varIndex);
}

void* 
Core::GetShaderGlobalVariable(ShaderHandle handle, unsigned int varIndex)
{	
	#if defined(_DEBUG)
	ValidateHandle(handle, _shaders);
	#endif
	Shader& shader = _shaders[handle.Value()];
	return shader.GetGlobalVariable(varIndex);
}

// ------------------------------------------------------------------------
// テクスチャ関連。
TextureHandle
Core::CreateTextureFromFile(const TCHAR* filePath)
{
	ID3D10ShaderResourceView* pTextureSRV;
	HRESULT hr;
	D3DX10CreateShaderResourceViewFromFile(_pDevice, filePath, NULL, NULL, &pTextureSRV, &hr);
	if (FAILED(hr))
	{
		return TextureHandle::INVALID;
	}
	
	return Push(pTextureSRV, _textures);
}

TextureHandle
Core::CreateTextureArrayFromFiles(const TCHAR** filePaths, unsigned int numFilePaths)
{
	if (numFilePaths == 0)
	{
		return TextureHandle::INVALID;
	}
	
	HRESULT hr;
	auto_interface<ID3D10Texture2D> pTextureArray;
	UINT w = 0;
	UINT h = 0;
	DXGI_FORMAT format;
	UINT mipLevels;
	
	// 1枚ずつテクスチャを読み込んで、テクスチャ配列にコピーしていく。
	for (unsigned int i = 0; i < numFilePaths; ++i)
	{
		auto_interface<ID3D10Texture2D> pTexture;
		{
			D3DX10_IMAGE_LOAD_INFO imgInfo;
			ZeroMemory(&imgInfo, sizeof(imgInfo));
			imgInfo.Usage = D3D10_USAGE_STAGING;
			imgInfo.CpuAccessFlags = D3D10_CPU_ACCESS_READ;
			//imgInfo.Width = D3DX10_DEFAULT;
			//imgInfo.Height = D3DX10_DEFAULT;
			//imgInfo.Depth = D3DX10_DEFAULT;
			//imgInfo.MipLevels = D3DX10_DEFAULT;
			imgInfo.Format = DXGI_FORMAT_FROM_FILE;
			
			ID3D10Resource* pResource;
			D3DX10CreateTextureFromFile(_pDevice, filePaths[i], &imgInfo, NULL, &pResource, &hr);
			if (FAILED(hr))
			{
				return TextureHandle::INVALID;
			}
			
			ID3D10Texture2D* pTextureTmp;
			hr = pResource->QueryInterface(__uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&pTextureTmp));
			if (FAILED(hr))
			{
				return TextureHandle::INVALID;
			}
			
			pTexture.reset(pTextureTmp);
		}

		D3D10_TEXTURE2D_DESC descTex;
		ZeroMemory(&descTex, sizeof(descTex));
		pTexture->GetDesc(&descTex);

		// テクスチャ配列を作成する。
		if (is_null(pTextureArray))
		{
			w = descTex.Width;
			h = descTex.Height;
			format = descTex.Format;
			mipLevels = descTex.MipLevels;
			
			descTex.Usage = D3D10_USAGE_DEFAULT;
			descTex.BindFlags = D3D10_BIND_SHADER_RESOURCE;
			descTex.CPUAccessFlags = 0;
			descTex.ArraySize = numFilePaths;
			ID3D10Texture2D* pTextureArrayTmp;
			hr = _pDevice->CreateTexture2D(&descTex, NULL, &pTextureArrayTmp);
			if (FAILED(hr))
			{
				return TextureHandle::INVALID;
			}
			pTextureArray.reset(pTextureArrayTmp);
		}
		// 読み込んだテクスチャのチェック。
		else
		{
			if (w != descTex.Width || h != descTex.Height)
			{
				return TextureHandle::INVALID;
			}
		}
		
		// テクスチャ配列にコピーする。
		for (UINT k = 0; k < descTex.MipLevels; ++k)
		{
			UINT subresourceIndexSrc = D3D10CalcSubresource(k, 0, descTex.MipLevels);
			UINT subresourceIndexDst = D3D10CalcSubresource(k, i, descTex.MipLevels);
			
			D3D10_MAPPED_TEXTURE2D mappedTexture;
			hr = pTexture->Map(subresourceIndexSrc, D3D10_MAP_READ, 0, &mappedTexture);
			if (FAILED(hr))
			{
				return TextureHandle::INVALID;
			}

			_pDevice->UpdateSubresource(pTextureArray.get(), subresourceIndexDst, NULL, mappedTexture.pData, mappedTexture.RowPitch, 0);

			pTexture->Unmap(subresourceIndexSrc);
		}
	}
	
	// シェーダリソースビューを作成する。
	ID3D10ShaderResourceView* pTextureArraySRV;
	D3D10_SHADER_RESOURCE_VIEW_DESC descSRV;
	ZeroMemory(&descSRV, sizeof(descSRV));
	descSRV.Format = format;
	descSRV.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2DARRAY;
	descSRV.Texture2DArray.MipLevels = mipLevels;
	descSRV.Texture2DArray.ArraySize = numFilePaths;
	hr = _pDevice->CreateShaderResourceView(pTextureArray.get(), &descSRV, &pTextureArraySRV);
	if (FAILED(hr))
	{
		return TextureHandle::INVALID;
	}
	
	return Push(pTextureArraySRV, _textures);
}

void
Core::SetShaderTexture(ShaderHandle shaderHandle, unsigned int texIndex, TextureHandle textureHandle)
{
	#if defined(_DEBUG)
	ValidateHandle(shaderHandle, _shaders);
	ValidateHandle(textureHandle, _textures);
	#endif
	Shader& shader = _shaders[shaderHandle.Value()];
	shader.SetTexture(texIndex, _textures[textureHandle.Value()]);
}

void
Core::SetShaderTexture(ShaderHandle shaderHandle, const char* texName, TextureHandle textureHandle)
{
	#if defined(_DEBUG)
	ValidateHandle(shaderHandle, _shaders);
	ValidateHandle(textureHandle, _textures);
	#endif
	Shader& shader = _shaders[shaderHandle.Value()];
	shader.SetTexture(texName, _textures[textureHandle.Value()]);
}

void
Core::SetShaderTexture(ShaderHandle shaderHandle, const char* texName, unsigned int indexInArray, TextureHandle textureHandle)
{
	#if defined(_DEBUG)
	ValidateHandle(shaderHandle, _shaders);
	ValidateHandle(textureHandle, _textures);
	#endif
	Shader& shader = _shaders[shaderHandle.Value()];
	shader.SetTexture(texName, indexInArray, _textures[textureHandle.Value()]);
}

void
Core::SetShaderTextureArray(ShaderHandle shaderHandle, unsigned int texArrayIndex, TextureHandle textureArrayHandle)
{
	#if defined(_DEBUG)
	ValidateHandle(shaderHandle, _shaders);
	ValidateHandle(textureArrayHandle, _textures);
	#endif
	Shader& shader = _shaders[shaderHandle.Value()];
	shader.SetTextureArray(texArrayIndex, _textures[textureArrayHandle.Value()]);
}

void
Core::SetShaderTextureArray(ShaderHandle shaderHandle, const char* texArrayName, TextureHandle textureArrayHandle)
{
	#if defined(_DEBUG)
	ValidateHandle(shaderHandle, _shaders);
	ValidateHandle(textureArrayHandle, _textures);
	#endif
	Shader& shader = _shaders[shaderHandle.Value()];
	shader.SetTextureArray(texArrayName, _textures[textureArrayHandle.Value()]);
}

void
Core::SetRenderTargetToShaderTexture(ShaderHandle shaderHandle, unsigned int texIndex, RenderTargetHandle renderTargetHandle)
{
	#if defined(_DEBUG)
	ValidateHandle(shaderHandle, _shaders);
	ValidateHandle(renderTargetHandle, _renderTargets);
	#endif
	Shader& shader = _shaders[shaderHandle.Value()];
	shader.SetTexture(texIndex, _renderTargets[renderTargetHandle.Value()]._pShaderResourceView);
}

void
Core::SetRenderTargetToShaderTexture(ShaderHandle shaderHandle, const char* texName, RenderTargetHandle renderTargetHandle)
{
	#if defined(_DEBUG)
	ValidateHandle(shaderHandle, _shaders);
	ValidateHandle(renderTargetHandle, _renderTargets);
	#endif
	Shader& shader = _shaders[shaderHandle.Value()];
	shader.SetTexture(texName, _renderTargets[renderTargetHandle.Value()]._pShaderResourceView);
}

void
Core::SetDepthStencilBufferToShaderTexture(ShaderHandle shaderHandle, unsigned int texIndex, DepthStencilBufferHandle depthStencilBufferHandle)
{
	#if defined(_DEBUG)
	ValidateHandle(shaderHandle, _shaders);
	ValidateHandle(depthStencilBufferHandle, _depthStencilBuffers);
	#endif
	Shader& shader = _shaders[shaderHandle.Value()];
	shader.SetTexture(texIndex, _depthStencilBuffers[depthStencilBufferHandle.Value()]._pShaderResourceView);
}

void
Core::SetDepthStencilBufferToShaderTexture(ShaderHandle shaderHandle, const char* texName, DepthStencilBufferHandle depthStencilBufferHandle)
{
	#if defined(_DEBUG)
	ValidateHandle(shaderHandle, _shaders);
	ValidateHandle(depthStencilBufferHandle, _depthStencilBuffers);
	#endif
	Shader& shader = _shaders[shaderHandle.Value()];
	shader.SetTexture(texName, _depthStencilBuffers[depthStencilBufferHandle.Value()]._pShaderResourceView);
}

// ------------------------------------------------------------------------
// サンプラーステート関連。
SamplerStateHandle
Core::CreateSamplerState(const D3D10_SAMPLER_DESC& desc)
{
	ID3D10SamplerState* pSamplerState;
	HRESULT hr = _pDevice->CreateSamplerState(
		&desc,
		&pSamplerState
		);
	if (FAILED(hr))
	{
		return SamplerStateHandle::INVALID;
	}

	return Push(pSamplerState, _samplerStates);
}

void
Core::ReleaseSamplerState(SamplerStateHandle handle)
{
	if (!ValidateHandle(handle, _samplerStates))
		return ;
	safe_release(_samplerStates[handle.Value()]);
}

const D3D10_SAMPLER_DESC* 
Core::GetSamplerStateDesc(SamplerStateHandle handle) const
{
	if (!ValidateHandle(handle, _samplerStates))
		return NULL;
	static D3D10_SAMPLER_DESC desc;
	_samplerStates[handle.Value()]->GetDesc(&desc);
	return &desc;
}

void
Core::SetShaderSamplerState(ShaderHandle shaderHandle, unsigned int sampIndex, SamplerStateHandle samplerHandle)
{
	#if defined(_DEBUG)
	ValidateHandle(shaderHandle, _shaders);
	ValidateHandle(samplerHandle, _samplerStates);
	#endif
	Shader& shader = _shaders[shaderHandle.Value()];
	shader.SetSamplerState(sampIndex, _samplerStates[samplerHandle.Value()]);
}

void
Core::SetShaderSamplerState(ShaderHandle shaderHandle, const char* sampName, SamplerStateHandle samplerHandle)
{
	#if defined(_DEBUG)
	ValidateHandle(shaderHandle, _shaders);
	ValidateHandle(samplerHandle, _samplerStates);
	#endif
	Shader& shader = _shaders[shaderHandle.Value()];
	shader.SetSamplerState(sampName, _samplerStates[samplerHandle.Value()]);
}

void
Core::SetShaderSamplerState(ShaderHandle shaderHandle, const char* sampName, unsigned int indexInArray, SamplerStateHandle samplerHandle)
{
	#if defined(_DEBUG)
	ValidateHandle(shaderHandle, _shaders);
	ValidateHandle(samplerHandle, _samplerStates);
	#endif
	Shader& shader = _shaders[shaderHandle.Value()];
	shader.SetSamplerState(sampName, indexInArray, _samplerStates[samplerHandle.Value()]);
}


// ------------------------------------------------------------------------
// フォント関連。
FontHandle
Core::CreateFont(const FontDesc& desc)
{
	Font font;
	
	HRESULT hr;
	hr = D3DX10CreateFont(
		_pDevice,
		desc.Height,
		desc.Width,
		desc.Weight,
		desc.MipLevels,
		desc.Italic,
		desc.CharSet,
		desc.OutputPrecision,
		desc.Quality,
		desc.PitchAndFamily,
		desc.pFaceName,
		&font._pInterface
		);
	if (FAILED(hr))
		return FontHandle::INVALID;
	
	font._desc = desc;
	
	return Push(font, _fonts);
}

void
Core::ReleaseFont(FontHandle handle)
{
	if (!ValidateHandle(handle, _fonts))
		return;
	_fonts[handle.Value()].Release();
}

const FontDesc* 
Core::GetFontDesc(FontHandle handle) const
{
	if (!ValidateHandle(handle, _fonts))
		return NULL;
	return &_fonts[handle.Value()]._desc;
}

INT 
Core::GetFontHeight(FontHandle handle) const
{
	const FontDesc* pDesc = GetFontDesc(handle);
	if (is_null(pDesc))
		return 0;
	return pDesc->Height;
}

void
Core::DrawText(FontHandle handle, int x, int y, const Color& color, const TCHAR* format, va_list va)
{
	#if defined(_DEBUG)
	ValidateHandle(handle, _fonts);
	#endif

	static TCHAR buf[256];
	_vstprintf_s(buf, format, va);
	
	RECT rc;
	rc.left		= x;
	rc.top		= y;
	rc.right	= x;
	rc.bottom	= y;
	
	Font& font = _fonts[handle.Value()];
	
	int result;
	result = font._pInterface->DrawText(
		NULL,
		buf,
		-1,
		&rc,
		DT_LEFT | DT_NOCLIP,
		ToD3DXCOLOR(color)
		);
}

void
Core::DrawText(FontHandle handle, int x, int y, const Color& color, const TCHAR* format, ...)
{
	va_list va;
	va_start(va, format);
	DrawText(handle, x, y, color, format, va);
	va_end(va);
}

