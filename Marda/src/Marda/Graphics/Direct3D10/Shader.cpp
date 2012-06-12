/*!
	@file	Graphics/Direct3D10/Shader.cpp

	@author	E.Nakayama
*/

#include <boost/tokenizer.hpp>

#include "Marda/Graphics/Direct3D10.h"
#include "Marda/Graphics/Direct3D10/Shader.h"

using namespace Marda;
using namespace Direct3D10;

namespace
{
	static const char* GLOBALS_CBUFFER_NAME = "$Globals";	// シェーダーグローバル変数が格納される定数バッファの名前。
		
}	// end namespace
		
Shader::Shader()
	: _pVertexShader(NULL)
	, _pGeometryShader(NULL)
	, _pPixelShader(NULL)
	, _pVertexShaderBytecode(NULL)
	, _pGeometryShaderBytecode(NULL)
	, _pPixelShaderBytecode(NULL)
{
}

Shader::~Shader()
{
	// std::vector<Shader> でコピーが発生した時に解放されてしまう為、ここに Release の呼び出しは書かない。
}

void
Shader::Release()
{
	safe_release(_pVertexShader);
	safe_release(_pGeometryShader);
	safe_release(_pPixelShader);
	safe_release(_pVertexShaderBytecode);
	safe_release(_pGeometryShaderBytecode);
	safe_release(_pPixelShaderBytecode);
	_constantBufferRegisterMap.clear();
	std::for_each(_constantBuffers.begin(), _constantBuffers.end(), &safe_release<ID3D10Buffer>);
	_constantBuffers.clear();
	_constantBufferVariableTables.clear();
	_textureRegisterMap.clear();
	_samplerRegisterMap.clear();
}

bool 
Shader::Create(
	ID3D10Device* pDevice,
	const TCHAR* path,
	const char* vertexShaderFunction,
	const char* vertexShaderProfile,
	const char* geometryShaderFunction,
	const char* geometryShaderProfile,
	const char* pixelShaderFunction,
	const char* pixelShaderProfile
	)
{
	HRESULT hr;
	ID3D10Blob* pErrorsBlob = NULL;

	// 頂点シェーダ。
	D3DX10CompileFromFile(
		path, NULL, NULL,
		vertexShaderFunction, vertexShaderProfile,
		D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR,
		0, NULL,
		&_pVertexShaderBytecode,
		&pErrorsBlob,
		&hr
		);
	if (FAILED(hr))
	{
		Error(pErrorsBlob);
		return false;
	}
		
	hr = pDevice->CreateVertexShader(
		_pVertexShaderBytecode->GetBufferPointer(),
		_pVertexShaderBytecode->GetBufferSize(),
		&_pVertexShader
		);
	if (FAILED(hr))
	{
		return false;
	}
		

	// ジオメトリシェーダ。
	if (is_not_null(geometryShaderFunction))
	{
		D3DX10CompileFromFile(
			path, NULL, NULL,
			geometryShaderFunction, geometryShaderProfile,
			D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR,
			0, NULL,
			&_pGeometryShaderBytecode,
			&pErrorsBlob,
			&hr
			);
		if (FAILED(hr))
		{
			Error(pErrorsBlob);
			return false;
		}
		
		hr = pDevice->CreateGeometryShader(
			_pGeometryShaderBytecode->GetBufferPointer(),
			_pGeometryShaderBytecode->GetBufferSize(),
			&_pGeometryShader
			);
		if (FAILED(hr))
		{
			return false;
		}
	}

	// ピクセルシェーダ。
	if (is_not_null(pixelShaderFunction))
	{
		D3DX10CompileFromFile(
			path, NULL, NULL,
			pixelShaderFunction, pixelShaderProfile,
			D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR,
			0, NULL,
			&_pPixelShaderBytecode,
			&pErrorsBlob,
			&hr
			);
		if (FAILED(hr))
		{
			Error(pErrorsBlob);
			return false;
		}
			
		hr = pDevice->CreatePixelShader(
			_pPixelShaderBytecode->GetBufferPointer(),
			_pPixelShaderBytecode->GetBufferSize(),
			&_pPixelShader
			);
		if (FAILED(hr))
		{
			return false;
		}
	}
	
	// シェーダコードを解析する。
	DEBUG_TRACEA("## %s", vertexShaderFunction);
	if (failed(Parse(pDevice, _pVertexShaderBytecode)))
	{
		return false;
	}

	if (is_not_null(_pGeometryShaderBytecode))
	{
		DEBUG_TRACEA("## %s", geometryShaderFunction);
		if (failed(Parse(pDevice, _pGeometryShaderBytecode)))
		{
			return false;
		}
	}

	if (is_not_null(_pPixelShaderBytecode))
	{
		DEBUG_TRACEA("## %s", pixelShaderFunction);
		if (failed(Parse(pDevice, _pPixelShaderBytecode)))
		{
			return false;
		}
	}
	
	_pfnApplyConstantBuffers = _constantBuffers.empty() ? &Shader::_ApplyEmptyConstantBuffers : &Shader::_ApplyConstantBuffers;
	_pfnApplyTextures = _textures.empty() ? &Shader::_ApplyEmptyTextures : &Shader::_ApplyTextures;
	_pfnApplySamplers = _samplers.empty() ? &Shader::_ApplyEmptySamplerStates : &Shader::_ApplySamplerStates;
	
	return true;
}

bool 
Shader::Parse(ID3D10Device* pDevice, ID3D10Blob* pShaderBytecode)
{
	HRESULT hr;
	ID3D10ShaderReflection* pReflector = NULL;
	auto_interface<ID3D10ShaderReflection> pReflectorScopeGuard(pReflector);
	D3D10ReflectShader(pShaderBytecode->GetBufferPointer(), pShaderBytecode->GetBufferSize(), &pReflector);
	D3D10_SHADER_DESC shaderDesc;
	pReflector->GetDesc(&shaderDesc);

	#if defined(_DEBUG)
	for (UINT i = 0; i < shaderDesc.InputParameters; ++i)
	{
		D3D10_SIGNATURE_PARAMETER_DESC spDesc;
		pReflector->GetInputParameterDesc(i, &spDesc);
		DEBUG_TRACEA("InputParameter[%u]: Semantic=%s%u, Register=%u", i, spDesc.SemanticName, spDesc.SemanticIndex, spDesc.Register);
	}
	#endif
	
	// シェーダリソースの解析。
	size_t numConstantBuffers = 0;
	size_t numTextures = 0;
	size_t numSamplers = 0;
	
	for (UINT i = 0; i < shaderDesc.BoundResources; ++i)
	{
		D3D10_SHADER_INPUT_BIND_DESC ibDesc;
		pReflector->GetResourceBindingDesc(i, &ibDesc);

		RegisterInfo registerInfo;
		registerInfo._number = ibDesc.BindPoint;
		registerInfo._count = ibDesc.BindCount;
			/*	BindPoint は各種レジスタの番号。定数バッファ=b#、テクスチャ/テクスチャバッファ=t#、サンプラ=s#。
				BindCount は、以下の様に要素数で宣言した配列に対して割り当てられたレジスタ数。
					Texture2D texs[3] : register(t3); // texs[0] = t4, texs[1] = t5, texs[2] = t6 となる。
				ただし、以下の様に宣言された配列では、BindCount は 1 となる。
					Texture2DArray texs : register(t3);
				本クラスでは、このレジスタ番号を 定数バッファ、テクスチャ、サンプラインタフェース用の各種 vector の添字になる様に実装してある。

				要素数指定の構文の場合、最適化によって使用されていない要素は切り詰められる可能性がある。
				例えば、
					Texture2D texs[3];
					...
					SV_Target PS(PS_INPUT input)
					{
						return texs[0].Sample(...);
					}
				とした場合、最適化によって texs の BindCount は 1 に切り詰められる。もし、texs[0] が texs[2] であれば、
				切り詰めは発生しない。

				ちなみに、要素数指定の場合、要素アクセス時の添字にはリテラルしか指定出来ない。
				以下の様なリテラルループの場合は、変数も指定可能。
					for (int i = 0; i < 3; ++i)
					{
						float color = texs[i].Sample(...);
					}
			*/

		if (ibDesc.Type == D3D10_SIT_CBUFFER)
		{
			DEBUG_TRACEA("Resource[%u]: ConstantBuffer, Name=%s, Register=b%u, Count=%u", i, ibDesc.Name, registerInfo._number, registerInfo._count);
			_constantBufferRegisterMap.insert(std::make_pair(ibDesc.Name, registerInfo));
			if (numConstantBuffers < registerInfo._number + registerInfo._count)
				numConstantBuffers = registerInfo._number + registerInfo._count;
		}
		else
		if (ibDesc.Type == D3D10_SIT_TEXTURE)
		{
			switch (ibDesc.Dimension)
			{
				case D3D10_SRV_DIMENSION_TEXTURE1DARRAY:
				case D3D10_SRV_DIMENSION_TEXTURE2DARRAY:
				case D3D10_SRV_DIMENSION_TEXTURE2DMSARRAY:
					DEBUG_TRACEA("Resource[%u]: TextureArray, Name=%s, Register=t%u, Count=%u", i, ibDesc.Name, registerInfo._number, registerInfo._count);
					break;
				default:
					DEBUG_TRACEA("Resource[%u]: Texture, Name=%s, Register=t%u, Count=%u", i, ibDesc.Name, registerInfo._number, registerInfo._count);
					break;
			}
			_textureRegisterMap.insert(std::make_pair(ibDesc.Name, registerInfo));
			if (numTextures < registerInfo._number + registerInfo._count)
				numTextures = registerInfo._number + registerInfo._count;
		}
		else
		if (ibDesc.Type == D3D10_SIT_SAMPLER)
		{
			DEBUG_TRACEA("Resource[%u]: Sampler, Name=%s, Register=s%u, Count=%u", i, ibDesc.Name, registerInfo._number, registerInfo._count);
			_samplerRegisterMap.insert(std::make_pair(ibDesc.Name, registerInfo));
			if (numSamplers < registerInfo._number + registerInfo._count)
				numSamplers = registerInfo._number + registerInfo._count;
		}
		else
		{
			DEBUG_TRACEA("Resource[%u]: Unknown, Name=%s, Register=%u, Count=%u", i, ibDesc.Name, registerInfo._number, registerInfo._count);
		}
	}
	
	if (_constantBuffers.size() < numConstantBuffers)
	{
		_constantBuffers.resize(numConstantBuffers, NULL);
		_constantBufferVariableTables.resize(numConstantBuffers);
		_constantBufferCaches.resize(numConstantBuffers);
	}
	
	if (_textures.size() < numTextures)
	{
		_textures.resize(numTextures);
	}

	if (_samplers.size() < numSamplers)
	{
		_samplers.resize(numSamplers);
	}
	
	// 定数バッファの作成と、バッファ内の変数を解析する。
	for (UINT i = 0; i < shaderDesc.ConstantBuffers; ++i)
	{
		ID3D10ShaderReflectionConstantBuffer* pCBuf = pReflector->GetConstantBufferByIndex(i);	// 解放不要。
		D3D10_SHADER_BUFFER_DESC cbufDesc;
		pCBuf->GetDesc(&cbufDesc);
		
		ResourceRegisterMap::iterator itrReg = _constantBufferRegisterMap.find(cbufDesc.Name);
		if (itrReg != _constantBufferRegisterMap.end())
		{
			const RegisterInfo& registerInfo = itrReg->second;
			ID3D10Buffer*& pBuf = _constantBuffers[registerInfo._number];
			ConstantBufferCache& cache = _constantBufferCaches[registerInfo._number];
			ConstantBufferVariableTable& variableTable = _constantBufferVariableTables[registerInfo._number];
			if (is_null(pBuf))
			{
				// 定数バッファ作成。
				D3D10_BUFFER_DESC bufDesc;
				#if 0
				// Map 関数を利用する場合は下記設定になるが、そもそも定数バッファに対する Map は D3D10_MAP_WRITE_DISCARD のみしか許されない模様。
				// D3D10_USAGE_STAGING を使用しても作成に失敗する模様。つまり、定数バッファの一部を書き換える事（＝変数毎の書き換え）は出来ない。
				// その為、本クラスに定数バッファの内容をキャッシュしておく事で実現する様にした。
				// また、本クラスで保持するメモリイメージは実際の定数バッファと同一になるので、UpdateSubresource を利用する形を採った。
				bufDesc.Usage = D3D10_USAGE_DYNAMIC;	
				bufDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
				bufDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
				#else
				bufDesc.Usage = D3D10_USAGE_DEFAULT;	
				bufDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
				bufDesc.CPUAccessFlags = 0;
				#endif
				bufDesc.MiscFlags = 0;
				bufDesc.ByteWidth = cbufDesc.Size;
				hr = pDevice->CreateBuffer(&bufDesc, NULL, &pBuf);
				if (FAILED(hr))
					return false;
				DEBUG_TRACEA("ConstantBuffer[%u]: Created, Name=%s, Size=" FMT_SIZE_T, registerInfo._number, cbufDesc.Name, cbufDesc.Size);
				
				cache.resize(cbufDesc.Size);
				
				// 変数情報の取得。
				D3D10_SHADER_VARIABLE_DESC varDesc;
				variableTable.resize(cbufDesc.Variables);
				for (UINT k = 0; k < cbufDesc.Variables; ++k)
				{
					ID3D10ShaderReflectionVariable* pVar = pCBuf->GetVariableByIndex(k);
					pVar->GetDesc(&varDesc);
					variableTable[k]._name = varDesc.Name;
					variableTable[k]._offset = varDesc.StartOffset;
					variableTable[k]._size = varDesc.Size;
					DEBUG_TRACEA("ConstantBufferVariable[%u]: Name=%s, Offset=" FMT_SIZE_T ", Size=" FMT_SIZE_T, k, varDesc.Name, varDesc.StartOffset, varDesc.Size);
				}
				
			}	// end if (is_null(...))
		}	// end if (itrReg ...)
	}	// end for (UINT i ...)
	
	return true;
}

void
Shader::Error(ID3D10Blob* pErrorsBlob)
{
	DEBUG_TRACEA("ShaderCompileError!");

	void* pError = pErrorsBlob->GetBufferPointer();
#if 0
	Tokens<char> errorMessage;
	errorMessage.Set(reinterpret_cast<char*>(pError), "\n", "");
	std::string msg;
	while (errorMessage.Next(msg))
	{
		DEBUG_TRACEA(msg.c_str());
	}
#else
	typedef boost::char_separator<char> ErrorMessageSeparator;
	typedef boost::tokenizer<ErrorMessageSeparator> ErrorMessages;

	ErrorMessageSeparator sep("\n");
	std::string allMessages(reinterpret_cast<char*>(pError)); 
	ErrorMessages messages(allMessages, sep);
	ErrorMessages::iterator itr = messages.begin();
	ErrorMessages::iterator itrEnd = messages.end();
	for (; itr != itrEnd; ++itr)
	{
		DEBUG_TRACEA(itr->c_str());
	}
#endif
	safe_release(pErrorsBlob);	// こっちで解放。
}

void
Shader::Apply(ID3D10Device* pDevice)
{
	pDevice->VSSetShader(_pVertexShader);
	pDevice->GSSetShader(_pGeometryShader);
	pDevice->PSSetShader(_pPixelShader);

	(this->*_pfnApplyConstantBuffers)(pDevice);
	(this->*_pfnApplyTextures)(pDevice);
	(this->*_pfnApplySamplers)(pDevice);
}

void
Shader::_ApplyConstantBuffers(ID3D10Device* pDevice)
{
	for (size_t i = 0, sz = _constantBuffers.size(); i < sz; ++i)
	{
		pDevice->UpdateSubresource(_constantBuffers[i], 0, NULL, &_constantBufferCaches[i][0], 0, 0);
	}
	
	pDevice->VSSetConstantBuffers(0, static_cast<UINT>(_constantBuffers.size()), &_constantBuffers[0]);
	pDevice->GSSetConstantBuffers(0, static_cast<UINT>(_constantBuffers.size()), &_constantBuffers[0]);
	pDevice->PSSetConstantBuffers(0, static_cast<UINT>(_constantBuffers.size()), &_constantBuffers[0]);
}

void
Shader::_ApplyEmptyConstantBuffers(ID3D10Device* /*pDevice*/)
{
}

void
Shader::_ApplyTextures(ID3D10Device* pDevice)
{
	pDevice->PSSetShaderResources(0, static_cast<UINT>(_textures.size()), &_textures[0]);
}

void
Shader::_ApplyEmptyTextures(ID3D10Device* /*pDevice*/)
{
}

void
Shader::_ApplySamplerStates(ID3D10Device* pDevice)
{
	pDevice->PSSetSamplers(0, static_cast<UINT>(_samplers.size()), &_samplers[0]);
}

void
Shader::_ApplyEmptySamplerStates(ID3D10Device* /*pDevice*/)
{
}

unsigned int
Shader::GetConstantBufferIndex(const char* cbufName) const
{
	ResourceRegisterMap::const_iterator found = _constantBufferRegisterMap.find(cbufName);
	#if defined(_DEBUG)
	if (found == _constantBufferRegisterMap.end())
		throw ElementNotFoundException("GetConstantBufferIndex: cbufName is not found!");
	#endif
	return found->second._number;
}

unsigned int
Shader::GetConstantBufferIndexGlobals() const
{
	return GetConstantBufferIndex(GLOBALS_CBUFFER_NAME);
}

unsigned int
Shader::GetConstantBufferVariableIndex(unsigned int cbufIndex, const char* varName) const
{
	const ConstantBufferVariableTable& variableTable = _constantBufferVariableTables[cbufIndex];
	for (size_t i = 0, sz = variableTable.size(); i < sz; ++i)
	{
		if (variableTable[i]._name.compare(varName) == 0)
			return static_cast<unsigned int>(i);
	}
	#if defined(_DEBUG)
	throw ElementNotFoundException("GetConstantBufferVariableIndex: varName is not found!");
	#else
	return 0;
	#endif
}

unsigned int
Shader::GetGlobalVariableIndex(const char* varName) const
{
	return GetConstantBufferVariableIndex(GetConstantBufferIndexGlobals(), varName);
}

void*
Shader::GetConstantBuffer(unsigned int cbufIndex)
{
	#if defined(_DEBUG)
	if (_constantBuffers.size() <= cbufIndex)
		throw std::out_of_range("GetConstantBuffer: cbufIndex is out of range!");
	#endif
	return &_constantBufferCaches[cbufIndex][0];
}

void*
Shader::GetConstantBufferVariable(unsigned int cbufIndex, unsigned int varIndex)
{
	#if defined(_DEBUG)
	if (_constantBuffers.size() <= cbufIndex)
		throw std::out_of_range("GetConstantBufferVariable: cbufIndex is out of range!");
	#endif
		
	ConstantBufferVariableTable& variableTable = _constantBufferVariableTables[cbufIndex];
	#if defined(_DEBUG)
	if (variableTable.size() <= varIndex)
		throw std::out_of_range("GetConstantBufferVariable: varIndex is out of range!");
	#endif

	ConstantBufferVariable& cbufVar = variableTable[varIndex];
	
	return &_constantBufferCaches[cbufIndex][0] + cbufVar._offset;
}

void*
Shader::GetGlobalVariable(unsigned int varIndex)
{
	return GetConstantBufferVariable(GetConstantBufferIndexGlobals(), varIndex);
}	

unsigned int
Shader::GetTextureIndex(const char* texName) const
{
	ResourceRegisterMap::const_iterator found = _textureRegisterMap.find(texName);
	#if defined(_DEBUG)
	if (found == _textureRegisterMap.end())
		throw ElementNotFoundException("GetTextureIndex: texName is not found!");
	#endif
	return found->second._number;
}

unsigned int
Shader::GetTextureIndex(const char* texName, unsigned int indexInArray) const
{
	ResourceRegisterMap::const_iterator found = _textureRegisterMap.find(texName);
	#if defined(_DEBUG)
	if (found == _textureRegisterMap.end())
		throw ElementNotFoundException("GetTextureIndex: texName is not found!");
	if (found->second._count >= indexInArray)
		throw ElementNotFoundException("GetTextureIndex: indexInArray is too large!");
	#endif
	return found->second._number + indexInArray;
}

unsigned int
Shader::GetTextureArrayIndex(const char* texArrayName) const
{
	return GetTextureIndex(texArrayName);
}

void
Shader::SetTexture(unsigned int texIndex, ID3D10ShaderResourceView* pTextureView)
{
	#if defined(_DEBUG)
	if (_textures.size() <= texIndex)
		throw std::out_of_range("SetTexture: texIndex is out of range!");
	#endif
	_textures[texIndex] = pTextureView;
}

void
Shader::SetTexture(const char* texName, ID3D10ShaderResourceView* pTextureView)
{
	SetTexture(GetTextureIndex(texName), pTextureView);
}

void
Shader::SetTexture(const char* texName, unsigned int indexInArray, ID3D10ShaderResourceView* pTextureView)
{
	SetTexture(GetTextureIndex(texName, indexInArray), pTextureView);
}

void
Shader::SetTextureArray(unsigned int texArrayIndex, ID3D10ShaderResourceView* pTextureArrayView)
{
	SetTexture(texArrayIndex, pTextureArrayView);
}

void
Shader::SetTextureArray(const char* texArrayName, ID3D10ShaderResourceView* pTextureArrayView)
{
	SetTexture(texArrayName, pTextureArrayView);
}

unsigned int
Shader::GetSamplerStateIndex(const char* sampName) const
{
	ResourceRegisterMap::const_iterator found = _samplerRegisterMap.find(sampName);
	#if defined(_DEBUG)
	if (found == _samplerRegisterMap.end())
		throw ElementNotFoundException("GetSamplerStateIndex: sampName is not found!");
	#endif
	return found->second._number;
}

unsigned int
Shader::GetSamplerStateIndex(const char* sampName, unsigned int indexInArray) const
{
	ResourceRegisterMap::const_iterator found = _samplerRegisterMap.find(sampName);
	#if defined(_DEBUG)
	if (found == _samplerRegisterMap.end())
		throw ElementNotFoundException("GetSamplerStateIndex: sampName is not found!");
	if (found->second._count >= indexInArray)
		throw ElementNotFoundException("GetSamplerStateIndex: indexInArray is too large!");
	#endif
	return found->second._number + indexInArray;
}

void
Shader::SetSamplerState(unsigned int sampIndex, ID3D10SamplerState* pSamplerState)
{
	#if defined(_DEBUG)
	if (_samplers.size() <= sampIndex)
		throw std::out_of_range("SetSamplerState: sampIndex is out of range!");
	#endif
	_samplers[sampIndex] = pSamplerState;
}

void
Shader::SetSamplerState(const char* sampName, ID3D10SamplerState* pSamplerState)
{
	SetSamplerState(GetSamplerStateIndex(sampName), pSamplerState);
}

void
Shader::SetSamplerState(const char* sampName, unsigned int indexInArray, ID3D10SamplerState* pSamplerState)
{
	SetSamplerState(GetSamplerStateIndex(sampName, indexInArray), pSamplerState);
}

ID3D10Blob*
Shader::GetVertexShaderBytecode()
{
	return _pVertexShaderBytecode;
}
