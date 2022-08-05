#include "Framework.h"
#include "FallPlatform.h"

#include "Engine/Component/Component.h"
#include "Engine/Component/Animator/Animator.h"
#include "Engine/Component/Collision/Collision.h"
#include "Engine/Resource/Shader.h"

#include "Game/Objects/Charactor/Player.h"

FallPlatform::FallPlatform(Scene* _scene, OBJECT_TYPE _type, int _updateOrder, GameObject* _parent) :
    GameObject(_scene, _type, _updateOrder, _parent)
{
	m_animator = ADDCOMP::NewAnimator(this, SHADER(L"AlphaShader"));
	InitAnim();

	m_collider = ADDCOMP::NewAARect({ -16, -16 }, { 16, 16 }, OBJECT_TYPE::PLATFORM, this);
	m_collider->SetCallbackOnCollisionEnter([this](Collider* _other) {
		if (_other->GetOwner()->GetType() == OBJECT_TYPE::PLAYER)
		{
			m_animator->Find(Anim::SINK)->Play();
		}
	});
    
	m_animator->Find(Anim::HOLD)->Play();
}

FallPlatform::~FallPlatform()
{
}

void FallPlatform::UpdateObject()
{
	if (m_isSinked)	// 바닥이 내려간 상태라면
	{
		m_timer += fDT;
		if (m_timer >= 1.0f)
		{
            m_isSinked = false;
            m_timer = 0.0f;
            m_animator->Find(Anim::HOLD)->Play();
		}
	}
}

void FallPlatform::RenderObject()
{
}

void FallPlatform::InitAnim()
{
	vector<Texture*> frames;
	float speed = 0.1f;
	for (size_t i = 0; i < Anim::FIN; i++)
	{
		frames.clear();

		switch (i)
		{
		case FallPlatform::HOLD:
			for (int x = 4; x < 8; ++x)
			{
				frames.push_back(TEXTURE->Add(L"Tile\\Platform.png", 8, 1, x, 0, { 0.501f, 0.5f }));
			}
			speed = 0.1f;
			break;
		case FallPlatform::SINK:
			for (int x = 0; x < 4; ++x)
			{
				frames.push_back(TEXTURE->Add(L"Tile\\Platform.png", 8, 1, x, 0, { 0.501f, 0.5f }));
			}
			speed = 0.1f;
			break;
		default:
			break;
		}

		m_animator->PushBack(frames, ANIM_PLAY_TYPE::ONCE, speed);
	}

	m_animator->Find(Anim::HOLD)->SetFinishFrameEvent([this]() {
		m_collider->IsActive(true);
		m_isSinked = false;
	});
	m_animator->Find(Anim::SINK)->SetFrameEvent(1, [this]() {
		m_collider->IsActive(false);
		m_isSinked = true;
	});
}
