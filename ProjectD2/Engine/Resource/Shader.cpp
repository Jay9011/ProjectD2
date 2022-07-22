#include "Framework.h"
#include "Shader.h"

unordered_map<wstring, Shader*> Shader::m_shaderMap = {};

Shader::Shader(const wstring& _path)
{
	if (FAILED(D3DXCreateEffectFromFile(DEVICE, _path.c_str(), nullptr, nullptr, D3DXSHADER_DEBUG, nullptr, &m_shader, nullptr)))
	{
		wstring error = L"Failed to load shader : " + _path;
		MessageBox(nullptr, error.c_str(), L"Error", MB_OK);
	}
	SetColor({ 1, 1, 1, 1 });
}

Shader::~Shader()
{
	m_shader->Release();
}

Shader* Shader::Add(const wstring& _path)
{
	wstring path = L"Engine\\Resource\\Shader\\" + _path + L".hlsl";

	if(m_shaderMap.find(path) != m_shaderMap.end())
		return m_shaderMap[path];
	
	m_shaderMap.emplace(path, new Shader(path));
	
	return m_shaderMap[path];
}

void Shader::Delete()
{
	for(auto& iter : m_shaderMap)
		delete iter.second;
	m_shaderMap.clear();
}

void Shader::Begin(int passIndex)
{
	m_shader->Begin(nullptr, 0);
	m_shader->BeginPass(passIndex);
}

void Shader::End()
{
	m_shader->EndPass();
	m_shader->End();
}

void Shader::Set(const string& _param, const int& _value)
{
	m_shader->SetInt(_param.c_str(), _value);
}

void Shader::Set(const string& _param, const float& _value)
{
	m_shader->SetFloat(_param.c_str(), _value);
}

void Shader::Set(const string& _param, void* _data, const int& _count)
{
	m_shader->SetFloatArray(_param.c_str(), (float*)_data, _count);
}

void Shader::Set(const string& _param, const LPDIRECT3DTEXTURE9& _texture)
{
	m_shader->SetTexture(_param.c_str(), _texture);
}
