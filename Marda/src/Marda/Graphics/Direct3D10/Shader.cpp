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
	static const char* GLOBALS_CBUFFER_NAME = "$Globals";	// �V�F�[�_�[�O���[�o���ϐ����i�[�����萔�o�b�t�@�̖��O�B
		
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
	// std::vector<Shader> �ŃR�s�[�������������ɉ������Ă��܂��ׁA������ Release �̌Ăяo���͏����Ȃ��B
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

	// ���_�V�F�[�_�B
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
		

	// �W�I���g���V�F�[�_�B
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

	// �s�N�Z���V�F�[�_�B
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
	
	// �V�F�[�_�R�[�h����͂���B
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
	
	// �V�F�[�_���\�[�X�̉�́B
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
			/*	BindPoint �͊e�탌�W�X�^�̔ԍ��B�萔�o�b�t�@=b#�A�e�N�X�`��/�e�N�X�`���o�b�t�@=t#�A�T���v��=s#�B
				BindCount �́A�ȉ��̗l�ɗv�f���Ő錾�����z��ɑ΂��Ċ��蓖�Ă�ꂽ���W�X�^���B
					Texture2D texs[3] : register(t3); // texs[0] = t4, texs[1] = t5, texs[2] = t6 �ƂȂ�B
				�������A�ȉ��̗l�ɐ錾���ꂽ�z��ł́ABindCount �� 1 �ƂȂ�B
					Texture2DArray texs : register(t3);
				�{�N���X�ł́A���̃��W�X�^�ԍ��� �萔�o�b�t�@�A�e�N�X�`���A�T���v���C���^�t�F�[�X�p�̊e�� vector �̓Y���ɂȂ�l�Ɏ������Ă���B

				�v�f���w��̍\���̏ꍇ�A�œK���ɂ���Ďg�p����Ă��Ȃ��v�f�͐؂�l�߂���\��������B
				�Ⴆ�΁A
					Texture2D texs[3];
					...
					SV_Target PS(PS_INPUT input)
					{
						return texs[0].Sample(...);
					}
				�Ƃ����ꍇ�A�œK���ɂ���� texs �� BindCount �� 1 �ɐ؂�l�߂���B�����Atexs[0] �� texs[2] �ł���΁A
				�؂�l�߂͔������Ȃ��B

				���Ȃ݂ɁA�v�f���w��̏ꍇ�A�v�f�A�N�Z�X���̓Y���ɂ̓��e���������w��o���Ȃ��B
				�ȉ��̗l�ȃ��e�������[�v�̏ꍇ�́A�ϐ����w��\�B
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
	
	// �萔�o�b�t�@�̍쐬�ƁA�o�b�t�@���̕ϐ�����͂���B
	for (UINT i = 0; i < shaderDesc.ConstantBuffers; ++i)
	{
		ID3D10ShaderReflectionConstantBuffer* pCBuf = pReflector->GetConstantBufferByIndex(i);	// ����s�v�B
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
				// �萔�o�b�t�@�쐬�B
				D3D10_BUFFER_DESC bufDesc;
				#if 0
				// Map �֐��𗘗p����ꍇ�͉��L�ݒ�ɂȂ邪�A���������萔�o�b�t�@�ɑ΂��� Map �� D3D10_MAP_WRITE_DISCARD �݂̂���������Ȃ��͗l�B
				// D3D10_USAGE_STAGING ���g�p���Ă��쐬�Ɏ��s����͗l�B�܂�A�萔�o�b�t�@�̈ꕔ�����������鎖�i���ϐ����̏��������j�͏o���Ȃ��B
				// ���ׁ̈A�{�N���X�ɒ萔�o�b�t�@�̓��e���L���b�V�����Ă������Ŏ�������l�ɂ����B
				// �܂��A�{�N���X�ŕێ����郁�����C���[�W�͎��ۂ̒萔�o�b�t�@�Ɠ���ɂȂ�̂ŁAUpdateSubresource �𗘗p����`���̂����B
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
				
				// �ϐ����̎擾�B
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
	safe_release(pErrorsBlob);	// �������ŉ���B
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
