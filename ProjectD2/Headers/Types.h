#pragma once

/* === === === === ===
*   수정 요소 Types
*  === === === === === */
using ColliderOptions = struct _colOp
{
	bool slidable = false;
	D3DXVECTOR2 resistance = { 30.0f, 0.0f };
};


/* === === === === === 
*   수정 불가 Types
*  === === === === === */

// 모니터 해상도
using RESOLUTION = struct
{
	unsigned int width;
	unsigned int height;
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

// float형 RECT left, top, right, bottom
using FRECT = struct tagFRECT
{
	float left;
	float top;
	float right;
	float bottom;
	
	tagFRECT() : left(0.0f), top(0.0f), right(0.0f), bottom(0.0f) {}
	tagFRECT(float left, float top, float right, float bottom) : left(left), top(top), right(right), bottom(bottom) {}
	D3DXVECTOR2 Size() { return D3DXVECTOR2(right - left, bottom - top); }
	D3DXVECTOR2 Pos() { return D3DXVECTOR2(left + (right - left) * 0.5f, top + (bottom - top) * 0.5f); }
};
using PFRECT = FRECT*;

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