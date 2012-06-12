/*!
 	@file Direct3D/Mesh.cpp
 
 	@author Dila
 */

#include "Direct3D/Mesh.h"

using namespace Direct3D;

Mesh::Mesh() :
	_pMesh(NULL),
	_Material()
{
	ZeroMemory(&_Material, sizeof(D3DMATERIAL));
	SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);
}

Mesh::~Mesh()
{
	Release();
}

void Mesh::Release()
{
	if (_pMesh) { _pMesh->Release(); _pMesh = NULL; }
}

void Mesh::SetDiffuseColor(float r, float g, float b, float a)
{
	_Material.Diffuse.r = r;
	_Material.Diffuse.g = g;
	_Material.Diffuse.b = b;
	_Material.Diffuse.a = a;
}

void Mesh::SetAmbientColor(float r, float g, float b, float a)
{
	_Material.Ambient.r = r;
	_Material.Ambient.g = g;
	_Material.Ambient.b = b;
	_Material.Ambient.a = a;
}

HRESULT Mesh::Render(Device& device)
{
	if (_pMesh) {
		HRESULT hr;

		hr = device.GetInterface()->SetMaterial(&_Material);
		if (FAILED(hr)) {
			return hr;
		}

		hr = _pMesh->DrawSubset(0);
		if (FAILED(hr)) {
			return hr;
		}
	}
	return D3D_OK;
}
