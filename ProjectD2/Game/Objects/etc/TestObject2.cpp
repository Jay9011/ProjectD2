#include "Framework.h"
#include "TestObject2.h"

#include "Engine/Component/Component.h"
#include "Engine/Component/Animator/Animator.h"
#include "Engine/Resource/Shader.h"
#include "Engine/Component/Collision/Collision.h"



TestObject2::TestObject2(Scene* _scene, int _updateOrder, GameObject* _parent) :
	TestObject2(_scene, OBJECT_TYPE::DEFAULT, _updateOrder, _parent)
{
}

TestObject2::TestObject2(Scene* _scene, OBJECT_TYPE _type, int _updateOrder, GameObject* _parent) :
	GameObject(_scene, _type, _updateOrder, _parent)
	, m_isRight(true)
	, m_speed(100.f)
{
	m_width = 200;
	m_height = 200;
	m_halfWidth = m_width * 0.5f;
	m_halfHeight = m_height * 0.5f;

	m_bodyCollider = ADDCOMP::NewAARect({ -m_halfWidth, -m_halfHeight }, { m_halfWidth, m_halfHeight }, this);
	m_bodyCollider->IsActive(true);
	
	m_animator = ADDCOMP::NewAnimator(this, SHADER(L"AlphaShader"));

	SetAnimation();
	
	m_animator->Find(0)->Play();
}

TestObject2::~TestObject2()
{
}

void TestObject2::UpdateObject()
{
}

void TestObject2::RenderObject()
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR2 textureSize = m_animator->GetCurrentTexture()->GetSize();
	D3DXVECTOR2 textureHalfSize = textureSize * 0.5f;

	size_t textureXCount = m_width / textureSize.x;
	size_t textureYCount = m_height / textureSize.y;

	D3DXVECTOR3 coord = {pos.x - m_halfWidth + textureHalfSize.x, pos.y - m_halfHeight + textureHalfSize.y, 0.0f};

	for (size_t y = 0; y < textureYCount; y++)
	{
		for (size_t x = 0; x < textureXCount; x++)
		{
			SetPos(coord.x + textureSize.x * x, coord.y + textureSize.y * y);
			UpdateWorld();
			SetWorld();
			m_animator->Render();
		}
	}

	SetPos(pos);
	UpdateWorld();
	SetWorld();

	GameObject::RenderObject();
}

void TestObject2::SetAnimation()
{
	vector<Texture*> frames;
	for (size_t i = 0; i < 8; i++)
	{
		frames.push_back(TEXTURE->Add(L"Tile\\Entry.png", 8, 1, i, 0));
	}

	m_animator->PushBack(frames);
}