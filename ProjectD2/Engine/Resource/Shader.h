#pragma once
#define SHADER(p) Shader::Add(p)
class Shader
{
private:
	Shader(const wstring& _path);
	~Shader();

public:
	static Shader* Add(const wstring& _path);
	static void    Delete();

	void Begin(int passIndex = 0);
	void End();
	
	void Set(const string& _param, const int& _value);
	void Set(const string& _param, const float& _value);
	void Set(const string& _param, void* _data, const int& _count);
	void Set(const string& _param, const LPDIRECT3DTEXTURE9& _texture);
	
private:
	D3DXCOLOR    m_color;
	LPD3DXEFFECT m_shader;
	static unordered_map<wstring, Shader*> m_shaderMap;

public:
	void SetColor(const D3DXCOLOR& _color) { m_color = _color; Set("Color", (float*)&m_color, 4); }
	
};

