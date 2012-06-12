/*!
 	@file Direct3D/Light.cpp
 
 	ライトクラスの実装
 
 	@author Dila
 */
#include "Direct3D/Light.h"

using namespace Direct3D;


DirectionalLight::DirectionalLight()
{
	ZeroMemory(&_light, sizeof(D3DLIGHT9));
	_light.Type = D3DLIGHT_DIRECTIONAL;

	SetDirection(0.0f, 0.0f, 1.0f);
	SetPosition(0.0f, 0.0f, 0.0f);
	SetDiffuseColor(1.0f, 1.0f, 1.0f);
	SetAmbientColor(1.0f, 1.0f, 1.0f);
	SetRange(1000.0f);
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::SetDirection(float x, float y, float z)
{
	D3DXVec3Normalize((D3DXVECTOR3*)&_light.Direction, &D3DXVECTOR3(x, y, z));
}

void DirectionalLight::SetPosition(float x, float y, float z)
{
	_light.Position = D3DXVECTOR3(x, y, z);
}

void DirectionalLight::SetDiffuseColor(float r, float g, float b, float a)
{
	_light.Diffuse.r = r;
	_light.Diffuse.g = g;
	_light.Diffuse.b = b;
	_light.Diffuse.a = a;
}

void DirectionalLight::SetAmbientColor(float r, float g, float b, float a)
{
	_light.Ambient.r = r;
	_light.Ambient.g = g;
	_light.Ambient.b = b;
	_light.Ambient.a = a;
}

void DirectionalLight::SetRange(float range)
{
	_light.Range = range;
}
