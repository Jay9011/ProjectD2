#include "Framework.h"
#include "TextureMgr.h"

TextureMgr::TextureMgr() = default;

TextureMgr::~TextureMgr()
{
	for (auto& texture : m_textureMap)
	{
		texture.second->Release();
	}
	m_textureMap.clear();

	SAFE_DELETE_VEC(m_textures);
}

Texture* TextureMgr::Add(const wstring& _path, const D3DXVECTOR2& _start, const D3DXVECTOR2& _end, const D3DXVECTOR2& _pivot)
{
	Texture* texture = new Texture(LoadTexture(_path), _start, _end, _pivot);

	m_textures.push_back(texture);

	return texture;
}

Texture* TextureMgr::Add(const wstring& _path, const UINT& _w, const UINT& _h, const D3DXVECTOR2& _start, const D3DXVECTOR2& _end, const D3DXVECTOR2& _pivot)
{
	Texture* texture = new Texture(LoadTexture(_path), _w, _h, _start, _end, _pivot);

	m_textures.push_back(texture);

	return texture;
}

Texture* TextureMgr::Add(const wstring& _path, const int& _maxFrameX, const int& _maxFrameY, const int& _frameX, const int& _frameY, const D3DXVECTOR2& _pivot)
{
	Texture* texture = new Texture(LoadTexture(_path), _maxFrameX, _maxFrameY, _frameX, _frameY, _pivot);

	m_textures.push_back(texture);

	return texture;
}

LPDIRECT3DTEXTURE9 TextureMgr::LoadTexture(const wstring& _path)
{
	if(m_textureMap.find(_path) != m_textureMap.end())
		return m_textureMap[_path];
	
	LPDIRECT3DTEXTURE9 texture = nullptr;
	wstring path = PATH->GetContentPath();
	path += L"Textures/" + _path;
	
	D3DXCreateTextureFromFileEx
	(
		DEVICE,
		path.c_str(),
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		1,
		0,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,
		D3DX_FILTER_NONE,
		0xFFFF00FF,
		nullptr,
		nullptr,
		&texture
	);

	m_textureMap.emplace(path, texture);
	
	return texture;
}
