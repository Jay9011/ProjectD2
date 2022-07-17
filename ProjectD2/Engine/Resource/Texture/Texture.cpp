#include "Framework.h"
#include "Texture.h"

Texture::Texture(LPDIRECT3DTEXTURE9 _texture, D3DXVECTOR2 _start, D3DXVECTOR2 _end, D3DXVECTOR2 _pivot) :
	Texture(_texture, _start, _end, 0, 0, _pivot)
{

}

Texture::Texture(LPDIRECT3DTEXTURE9 _texture, D3DXVECTOR2 _start, D3DXVECTOR2 _end, UINT _w, UINT _h, D3DXVECTOR2 _pivot) :
	m_texture(_texture)
	, m_pivot(_pivot)
{
	D3DSURFACE_DESC surface;
	m_texture->GetLevelDesc(0, &surface);

	m_srcSize = D3DXVECTOR2(surface.Width, surface.Height);

	m_uvStart.x = _start.x / m_srcSize.x;
	m_uvStart.y = _start.y / m_srcSize.y;

	m_uvEnd.x = (_end.x == 0.0f) ? 1.0f : _end.x / m_srcSize.x;
	m_uvEnd.y = (_end.y == 0.0f) ? 1.0f : _end.y / m_srcSize.y;

	if (_w != 0 || _h != 0)
		m_size = { (float)_w, (float)_h };
	else
		m_size = m_srcSize;

	m_halfSize = m_size * 0.5f;

	SetData();
}

Texture::Texture(LPDIRECT3DTEXTURE9 _texture, int _maxFrameX, int _maxFrameY, int _frameX, int _frameY, D3DXVECTOR2 _pivot) :
	m_texture(_texture)
	, m_pivot(_pivot)
{
	D3DSURFACE_DESC surface;
	m_texture->GetLevelDesc(0, &surface);

	m_srcSize = D3DXVECTOR2(surface.Width, surface.Height);
	m_size = D3DXVECTOR2(surface.Width / _maxFrameX, surface.Height / _maxFrameY);
	
	m_uvStart.x = (float)_frameX / (float)_maxFrameX;
	m_uvStart.y = (float)_frameY / (float)_maxFrameY;

	m_uvEnd.x = (float)(_frameX + 1) / (float)_maxFrameX;
	m_uvEnd.y = (float)(_frameY + 1) / (float)_maxFrameY;

	m_halfSize = m_size * 0.5f;

	SetData();
}

Texture::~Texture()
{
	m_vertexBuffer->Release();
	 m_indexBuffer->Release();
}

void Texture::Render()
{
	DEVICE->SetFVF(VERTEXTEXTURE::FVF);
	DEVICE->SetTexture(0, m_texture);	// 텍스처 설정 (매핑을 위한 텍스처)
	
	DEVICE->SetStreamSource(0, m_vertexBuffer, 0, sizeof(VERTEXTEXTURE));	// 출력할 버텍스 버퍼
	DEVICE->SetIndices(m_indexBuffer);
	DEVICE->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_vertexCount, 0, m_indexCount / 3);
	
	DEVICE->SetTexture(0, nullptr);		// 텍스처 설정 해제
}

void Texture::SetData()
{
	m_vertexCount = 4;
	VertexTexture vertices[4];

	vertices[0] = VERTEXTEXTURE(-m_size.x * m_pivot.x      , -m_size.y * m_pivot.y      , m_uvStart.x, m_uvStart.y);
	vertices[1] = VERTEXTEXTURE(+m_size.x * (1 - m_pivot.x), -m_size.y * m_pivot.y      , m_uvEnd.x  , m_uvStart.y);
	vertices[2] = VERTEXTEXTURE(-m_size.x * m_pivot.x      , +m_size.y * (1 - m_pivot.y), m_uvStart.x, m_uvEnd.y);
	vertices[3] = VERTEXTEXTURE(+m_size.x * (1 - m_pivot.x), +m_size.y * (1 - m_pivot.y), m_uvEnd.x  , m_uvEnd.y);

	m_indexCount = 6;
	WORD indices[6] = { 0, 1, 2, 2, 1, 3 };

	UINT vertexSize = m_vertexCount * sizeof(VERTEXTEXTURE);

	DEVICE->CreateVertexBuffer(vertexSize, 0, VERTEXTEXTURE::FVF, D3DPOOL_DEFAULT, &m_vertexBuffer, nullptr);

	void* vertexData = nullptr;
	m_vertexBuffer->Lock(0, vertexSize, &vertexData, 0);
	memcpy(vertexData, vertices, vertexSize);
	m_vertexBuffer->Unlock();

	UINT indexSize = m_indexCount * sizeof(WORD);

	DEVICE->CreateIndexBuffer(indexSize, 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_indexBuffer, nullptr);

	void* indexData = nullptr;
	m_indexBuffer->Lock(0, indexSize, &indexData, 0);
	memcpy(indexData, indices, indexSize);
	m_indexBuffer->Unlock();
}
