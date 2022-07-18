#include "Framework.h"
#include "TestObject.h"

#include "Engine/Singleton/Shader/Shader.h"

TestObject::TestObject(Scene* _scene, GameObject* _parent) :
	TestObject(_scene, OBJECT_TYPE::DEFAULT, _parent)
{}

TestObject::TestObject(Scene* _scene, OBJECT_TYPE _type, GameObject* _parent) :
	GameObject(_scene, _type, _parent)
	, m_isRight(true)
	, m_speed(100.f)
{
#if _DEBUG
	TwAddVarRW(_scene->twbar, "Speed", TW_TYPE_FLOAT, &m_speed, "min=0.0 max=1000.0 step=1.0");
#endif // _DEBUG

	m_texture = TEXTURE->Add(L"Character\\Player\\Idle.png", 75, 75, { 1, 1 }, { 36,47 }, { 0.5, 0.477273 });
	//m_shader = SHADER(L"AlphaShader");
	m_shader = SHADER(L"OutlineShader");
}

TestObject::~TestObject()
{
}

void TestObject::FinalUpdate()
{
	if(KEYPRESS(VK_LEFT))
	{
		AddPos(V_LEFT * m_speed * fDT);
		if (m_isRight)
		{
			m_isRight = !m_isRight;
			ScaleXInverse();
		}
	}
	if (KEYPRESS(VK_RIGHT))
	{
		AddPos(V_RIGHT * m_speed * fDT);
		if (!m_isRight)
		{
			m_isRight = !m_isRight;
			ScaleXInverse();
		}
	}
	if (KEYPRESS(VK_UP))
	{
		AddPos(V_UP * m_speed * fDT);
	}
	if (KEYPRESS(VK_DOWN))
	{
		AddPos(V_DOWN * m_speed * fDT);
	}
}

void TestObject::FinalRender()
{
	D3DXVECTOR2 imageSize = m_texture->GetSize();
	m_shader->Set("ImageSize", &imageSize, 2);
	m_shader->Begin();
	m_texture->Render();
	m_shader->End();
	
	GameObject::FinalRender();
}
