#include "Framework.h"
#include "ScreenButton.h"

#include "Engine/Component/Component.h"
#include "Engine/Component/Animator/Animator.h"
#include "Engine/Resource/Shader.h"
#include "Engine/Component/Collision/Collision.h"

#include "Game/Objects/Interactive/Door.h"

ScreenButton::ScreenButton(Scene* _scene, OBJECT_TYPE _type, int _updateOrder, GameObject* _parent) :
    GameObject(_scene, _type, _updateOrder, _parent)
    , m_interactObject(nullptr)
{
	m_animator = ADDCOMP::NewAnimator(this, SHADER(L"AlphaShader"));
	InitAnimation();
	m_animator->SetCurrentAnimation(m_animator->Find(Anim::OFF));

	m_keyImg = TEXTURE->Add(L"UI\\Fkey.png");
    m_shader = SHADER(L"AlphaShader");

    /*
	* InteractBox 사정거리 내에 들어오면 inRange를 true로, 빠져나오면 inRange를 false로 만든다.
	*/
	m_InteractBox = ADDCOMP::NewAARect({ -20, -10 }, { 20, 20 }, this);
	m_InteractBox->IsActive(true);
    m_InteractBox->SetCallbackOnCollisionEnter([this](Collider* _other)
		{
			if (_other->GetOwner()->GetType() == OBJECT_TYPE::PLAYER)
			{
				m_inRange = true;
			}
		});
	m_InteractBox->SetCallbackOnCollisionExit([this](Collider* _other)
		{
			if (_other->GetOwner()->GetType() == OBJECT_TYPE::PLAYER)
			{
				m_inRange = false;
			}
		});
}

ScreenButton::~ScreenButton()
{
}

void ScreenButton::UpdateObject()
{
	if (!m_inRange)
		return;

	if (KEYDOWN('F'))
	{
		if (!m_isUsed)
		{
			m_animator->Find(Anim::ON)->Play();
            
            if(m_interactObject != nullptr)
                m_interactObject->Interact();
		}
		m_isUsed = !m_isUsed;
	}
}

void ScreenButton::PostUpdateObject()
{
}

void ScreenButton::FinalUpdateObject()
{
}

void ScreenButton::RenderObject()
{
	if (!m_inRange || m_isUsed)
        return;
    
	D3DXVECTOR2 pos = GetPos();
	AddPos(0.0f, -30.0f);
	UpdateWorld();
	SetWorld();
    
	m_shader->Begin();
	m_keyImg->Render();
	m_shader->End();

	SetPos(pos);
	UpdateWorld();
	SetWorld();
}

void ScreenButton::InitAnimation()
{
	vector<Texture*> frames;

	for (size_t i = 0; i < Anim::FIN; i++)
	{
		frames.clear();

		switch (i)
		{
		case ScreenButton::OFF:
			frames.push_back(TEXTURE->Add(L"Tile\\Screen2Blk.png", 1, 1, 0, 0, { 0.501f, 0.5f }));
			break;
		case ScreenButton::ON:
			for (int x = 0; x < 4; ++x)
			{
				frames.push_back(TEXTURE->Add(L"Tile\\Screen2.png", 4, 1, x, 0, { 0.501f, 0.5f }));
			}
			break;
		}

		m_animator->PushBack(frames);
	}
}
