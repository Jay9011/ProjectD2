#pragma once

// 모니터 해상도
using RESOLUTION = struct
{
	unsigned int WIN_WIDTH;
	unsigned int WIN_HEIGHT;
};
using PRESOLUTION = RESOLUTION*;

// VertexColor
using VERTEXCOLOR = struct VertexColor
{
	D3DXVECTOR3 position;
	D3DCOLOR    color;
	
	VertexColor() : position(0.0f, 0.0f, 0.0f), color(D3DCOLOR_XRGB(255, 255, 255)) {}
	VertexColor(float _x, float _y, D3DCOLOR _color, float _z = 0.f)
	{
		position = D3DXVECTOR3(_x, _y, _z);
		color    = _color;
	}
	
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
};
using PVERTEXCOLOR = VERTEXCOLOR*;

// VertexTexture
using VERTEXTEXTURE = struct VertexTexture
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 uv;
	
	VertexTexture() : position(0.0f, 0.0f, 0.0f), uv(0.0f, 0.0f) {}
	VertexTexture(float _x, float _y, float _u, float _v, float _z = 0.f)
	{
		position = D3DXVECTOR3(_x, _y, _z);
		uv       = D3DXVECTOR2(_u, _v);
	}
	
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
};