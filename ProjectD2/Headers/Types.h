#pragma once

// 모니터 해상도
using RESOLUTION = struct
{
	unsigned int WIN_WIDTH;
	unsigned int WIN_HEIGHT;
};
using PRESOLUTION = RESOLUTION*;

// int형 Size x, y
using ISIZE = struct tagISIZE
{
	unsigned int x;
	unsigned int y;
	
	tagISIZE() : x(0), y(0) {}
	tagISIZE(unsigned int x, unsigned int y) : x(x), y(y) {}
};
using PISIZE = ISIZE*;

// float형 Size x, y
using FSIZE = struct tagFSIZE
{
	float x;
	float y;
	
	tagFSIZE() : x(0.0f), y(0.0f) {}
	tagFSIZE(float x, float y) : x(x), y(y) {}
};
using PFSIZE = FSIZE*;

// VertexColor
using VERTEXCOLOR = struct tagVertexColor
{
	D3DXVECTOR3 position;
	D3DCOLOR    color;
	
	tagVertexColor() : position(0.0f, 0.0f, 0.0f), color(D3DCOLOR_XRGB(255, 255, 255)) {}
	tagVertexColor(float _x, float _y, D3DCOLOR _color, float _z = 0.f)
	{
		position = D3DXVECTOR3(_x, _y, _z);
		color    = _color;
	}
	
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
};
using PVERTEXCOLOR = VERTEXCOLOR*;

// VertexTexture
using VERTEXTEXTURE = struct tagVertexTexture
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 uv;
	
	tagVertexTexture() : position(0.0f, 0.0f, 0.0f), uv(0.0f, 0.0f) {}
	tagVertexTexture(float _x, float _y, float _u, float _v, float _z = 0.f)
	{
		position = D3DXVECTOR3(_x, _y, _z);
		uv       = D3DXVECTOR2(_u, _v);
	}
	
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
};