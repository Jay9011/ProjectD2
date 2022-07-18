#pragma once
#include "Engine/Manager/Texture/Texture.h"
class TextureMgr
{
public:
	TextureMgr();
	~TextureMgr();

public:
	Texture* Add(const wstring& _path, const D3DXVECTOR2& _start = { 0, 0 }, const D3DXVECTOR2& _end = { 0, 0 }, const D3DXVECTOR2& _pivot = { 0, 0 });
	Texture* Add(const wstring& _path, const UINT& _w, const UINT& _h, const D3DXVECTOR2& _start = { 0, 0 }, const D3DXVECTOR2& _end = { 0, 0 }, const D3DXVECTOR2& _pivot = { 0, 0 });
	Texture* Add(const wstring& _path, const int& maxFrameX, const int& maxFrameY, const int& frameX, const int& frameY, const D3DXVECTOR2& _pivot = { 0, 0 });
	
	LPDIRECT3DTEXTURE9 LoadTexture(const wstring& _path);

private:
	unordered_map<wstring, LPDIRECT3DTEXTURE9> m_textureMap;
	vector<Texture*> m_textures;

};

