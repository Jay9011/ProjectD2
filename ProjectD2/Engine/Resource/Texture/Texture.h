#pragma once
class Texture
{
public:
	Texture(LPDIRECT3DTEXTURE9 _texture, D3DXVECTOR2 _start, D3DXVECTOR2 _end, D3DXVECTOR2 _pivot = { 0.5f, 0.5f });
	Texture(LPDIRECT3DTEXTURE9 _texture, D3DXVECTOR2 _start, D3DXVECTOR2 _end, UINT _w, UINT _h, D3DXVECTOR2 _pivot = { 0.5f, 0.5f });
	Texture(LPDIRECT3DTEXTURE9 _texture, int _maxFrameX, int _maxFrameY, int _frameX, int _frameY, D3DXVECTOR2 _pivot = { 0.5f, 0.5f });
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
	D3DXVECTOR2 m_size;
	D3DXVECTOR2 m_halfSize;
	D3DXVECTOR2 m_uvStart;
	D3DXVECTOR2 m_uvEnd;
	D3DXVECTOR2 m_pivot;
};

