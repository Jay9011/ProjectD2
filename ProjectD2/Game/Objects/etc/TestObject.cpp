#include "Framework.h"
#include "TestObject.h"

#include "Engine/Singleton/Shader/Shader.h"
#include "Engine/Component/Animator/Animator.h"

TestObject::TestObject(Scene* _scene, GameObject* _parent) :
	TestObject(_scene, OBJECT_TYPE::DEFAULT, _parent)
{}

TestObject::TestObject(Scene* _scene, OBJECT_TYPE _type, GameObject* _parent) :
	GameObject(_scene, _type, _parent)
	, m_isRight(true)
	, m_speed(100.f)
	, m_state(PLAYER_STATE::IDLE)
{
#if _DEBUG
	TwAddVarRW(_scene->twbar, "Speed", TW_TYPE_FLOAT, &m_speed, "min=0.0 max=1000.0 step=1.0");
#endif // _DEBUG

	m_shader = SHADER(L"AlphaShader");
	m_animator = new Animator(this);

	SetAnimation();
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
	m_animator->m_shader = SHADER(L"AlphaShader");
	m_animator->Render();
	
	GameObject::FinalRender();
}

void TestObject::SetAnimation()
{
	vector<Texture*> frames;
	// IDLE
	//frames.push_back(TEXTURE->Add(L"Character\\Player\\Idle.png", 75, 75, {   4,  4 }, {  39,  50 }, { 0.5f, 0.5f } ));
	//frames.push_back(TEXTURE->Add(L"Character\\Player\\Idle.png", 75, 75, {  47,  4 }, {  82,  48 }, { 0.5f, 0.5f } ));
	//frames.push_back(TEXTURE->Add(L"Character\\Player\\Idle.png", 75, 75, {  90,  4 }, { 125,  49 }, { 0.5f, 0.5f } ));
	//frames.push_back(TEXTURE->Add(L"Character\\Player\\Idle.png", 75, 75, { 133,  4 }, { 168,  48 }, { 0.5f, 0.5f } ));
	//frames.push_back(TEXTURE->Add(L"Character\\Player\\Idle.png", 75, 75, { 176,  4 }, { 211,  48 }, { 0.5f, 0.5f } ));
	//m_animator->PushBack(frames, ANIM_PLAY_TYPE::Loop, 0.5f);

	m_animator->LoadXML("Character\\Player\\", "Appear", ANIM_PLAY_TYPE::Loop, 0.5f);

	// RUN
	
	// JUMP
	
}
