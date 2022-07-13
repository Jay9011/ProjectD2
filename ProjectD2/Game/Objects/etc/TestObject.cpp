#include "Framework.h"
#include "TestObject.h"

#include "Engine/Resource/Texture/Texture.h"

TestObject::TestObject(Game* _game, Scene* _scene, GameObject* _parent) :
	TestObject(_game, _scene, OBJECT_TYPE::DEFAULT, _parent)
{}

TestObject::TestObject(Game* _game, Scene* _scene, OBJECT_TYPE _type, GameObject* _parent) :
	GameObject(_game, _scene, _type, _parent)
	, m_isRight(true)
	, m_speed(100.f)
{
#if _DEBUG
	TwAddVarRW(_scene->twbar, "Speed", TW_TYPE_FLOAT, &m_speed, "min=0.0 max=1000.0 step=1.0");
#endif // _DEBUG

	LPDIRECT3DTEXTURE9 texture;

	wstring path = PATH->GetContentPath();
	path += L"Textures/Character/Player/Idle.png";
	
	D3DXCreateTextureFromFileEx
	(
		DEVICE,
		path.c_str(),
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		D3DX_FROM_FILE,
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
	
	m_texture = new Texture(texture, { 1, 1 }, { 36, 47 }, 75, 75, { 0.5 , 0.477273 });
}

TestObject::~TestObject()
{
	delete m_texture;
}

void TestObject::UpdateObject()
{
	if(KEYPRESS(VK_LEFT))
	{
		AddPos(V_LEFT * m_speed * DT);
		if (m_isRight)
		{
			m_isRight = !m_isRight;
			ScaleXInverse();
		}
	}
	if (KEYPRESS(VK_RIGHT))
	{
		AddPos(V_RIGHT * m_speed * DT);
		if (!m_isRight)
		{
			m_isRight = !m_isRight;
			ScaleXInverse();
		}
	}
	if (KEYPRESS(VK_UP))
	{
		AddPos(V_UP * m_speed * DT);
	}
	if (KEYPRESS(VK_DOWN))
	{
		AddPos(V_DOWN * m_speed * DT);
	}
}

void TestObject::Render()
{
	m_texture->Render();
	GameObject::Render();
}
