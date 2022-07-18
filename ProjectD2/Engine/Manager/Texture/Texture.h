#pragma once
class Texture
{
public:
	Texture(const LPDIRECT3DTEXTURE9& _texture, const D3DXVECTOR2& _start, const D3DXVECTOR2& _end, const D3DXVECTOR2& _pivot = { 0.5f, 0.5f });
	Texture(const LPDIRECT3DTEXTURE9& _texture, const UINT& _w, const UINT& _h, const D3DXVECTOR2& _start, const D3DXVECTOR2& _end, const D3DXVECTOR2& _pivot = { 0.5f, 0.5f });
	Texture(const LPDIRECT3DTEXTURE9& _texture, const int& _maxFrameX, const int& _maxFrameY, const int& _frameX, const int& _frameY, const D3DXVECTOR2& _pivot = { 0.5f, 0.5f });
	~Texture();

	void Render();
	void SetData();

private:
	VertexTexture* m_vertices;
	WORD*          m_indices;

	UINT m_vertexCount;
	UINT m_indexCount;
	
	LPDIRECT3DVERTEXBUFFER9 m_vertexBuffer;
	LPDIRECT3DINDEXBUFFER9  m_indexBuffer;

	LPDIRECT3DTEXTURE9 m_texture;
	D3DXVECTOR2 m_srcSize;
	D3DXVECTOR2 m_size;
	D3DXVECTOR2 m_halfSize;
	D3DXVECTOR2 m_uvStart;
	D3DXVECTOR2 m_uvEnd;
	D3DXVECTOR2 m_pivot;
};

