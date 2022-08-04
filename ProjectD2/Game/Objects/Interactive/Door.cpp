#include "Framework.h"
#include "Door.h"

#include "Engine/Component/Component.h"
#include "Engine/Component/Animator/Animator.h"
#include "Engine/Component/Collision/Collision.h"
#include "Engine/Resource/Shader.h"

Door::Door(Scene* _scene, OBJECT_TYPE _type, int _updateOrder, GameObject* _parent) :
    GameObject(_scene, _type, _updateOrder, _parent)
{
    m_animator = ADDCOMP::NewAnimator(this, SHADER(L"AlphaShader"));
    InitAnimation();
    m_animator->SetCurrentAnimation(m_animator->Find(Anim::CLOSE));

    m_collider = ADDCOMP::NewAARect({-16, -32}, {16, 32}, OBJECT_TYPE::PLATFORM, this);
	m_collider->IsActive(true);
}

Door::~Door()
{
}

void Door::Interact()
{
    if (!m_isUsed)
    {
        m_animator->Find(Anim::OPEN)->Play();
        m_isUsed = true;
    }
}

void Door::UpdateObject()
{
}

void Door::RenderObject()
{
}

void Door::InitAnimation()
{
	vector<Texture*> frames;

	for (size_t i = 0; i < Anim::FIN; i++)
	{
		frames.clear();

		switch (i)
		{
		case Door::CLOSE:
			for (int x = 0; x < 1; ++x)
			{
				frames.push_back(TEXTURE->Add(L"Tile\\Entry.png", 8, 1, x, 0, { 0.501f, 0.5f }));
			}
			break;
		case Door::OPEN:
			for (int x = 0; x < 5; ++x)
			{
				frames.push_back(TEXTURE->Add(L"Tile\\Entry.png", 8, 1, x, 0, { 0.501f, 0.5f }));
			}
			break;
		case Door::FIN:
			break;
		default:
			break;
		}

		m_animator->PushBack(frames, ANIM_PLAY_TYPE::ONCE);
	}

    m_animator->Find(Anim::OPEN)->SetFrameEvent(3, [this]() {
		m_collider->IsActive(false);
		if (m_openFunc != nullptr)
		{
            m_openFunc();
		}
    });
}
