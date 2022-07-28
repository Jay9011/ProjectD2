#include "Framework.h"
#include "Animator.h"

#include "Engine/Object/GameObject.h"
#include "Engine/Resource/Shader.h"
#include "Lib/XML.h"

Animator::Animator(GameObject* _owner, int _updateOrder) :
	Component(_owner, _updateOrder)
	, m_owner(_owner)
{
}

Animator::~Animator()
{
	while (!m_animations.empty())
	{
		delete m_animations.back();
		m_animations.pop_back();
	}
}

void Animator::PushBack(const vector<Texture*>& _actions, const ANIM_PLAY_TYPE& _type, const float& _speed)
{
	m_animations.push_back(new Animation(this, _actions, _type, _speed));
}

void Animator::Update()
{
	if (m_animations.empty())
	{
		return;
	}
	else if (m_currentAnimation == nullptr)
	{
		if (m_nextAnimation == nullptr)
		{
			m_animations.front()->Play();
			return;
		}
		else
		{
			m_nextAnimation->Play();
			m_nextAnimation = nullptr;
			return;
		}
	}
	
	if (m_currentAnimation->IsDone())
	{
		ChangeAnimation();
	}
	
	m_currentAnimation->Update();
}

void Animator::Render()
{
	if (m_animations.empty())
		return;
	
	if (m_shader != nullptr)
	{
		m_shader->Begin(m_passIndex);
	}
	
	m_currentAnimation->Render();

	if (m_shader != nullptr)
	{
		m_shader->End();
	}
}

void Animator::FinalUpdate()
{
	m_currentAnimation->FinalUpdate();
}

void Animator::LoadXML(const string& _path, const string& _name, const ANIM_PLAY_TYPE& _type, const float& _speed)
{
	vector<Texture*> clips;
	
	XmlDocument* document = new XmlDocument;
	
	wstring wPath;
	wPath.assign(_path.begin(), _path.end());
	wstring wName;
	wName.assign(_name.begin(), _name.end());

	wstring contentPath = PATH->GetContentPath();
	
	string xmlFullPath;
	xmlFullPath.assign(contentPath.begin(), contentPath.end());
	xmlFullPath += "Textures\\" + _path + _name + ".xml";
	
	document->LoadFile(xmlFullPath.c_str());
	
	XmlElement* altas = document->FirstChildElement();
	string imageName = altas->Attribute("imagePath");
	imageName = _path + imageName;

	wstring imagePath;
	imagePath.assign(imageName.begin(), imageName.end());

	XmlElement* sprite = altas->FirstChildElement();
	
	while (sprite != nullptr)
	{
		D3DXVECTOR2 start = D3DXVECTOR2(sprite->FloatAttribute("x"), sprite->FloatAttribute("y"));
		ISIZE       size  = ISIZE(sprite->FloatAttribute("w"), sprite->FloatAttribute("h"));
		D3DXVECTOR2 pivot = D3DXVECTOR2(sprite->FloatAttribute("pX"), sprite->FloatAttribute("pY"));

		clips.push_back(TEXTURE->Add(imagePath, start, size, pivot));
		
		sprite = sprite->NextSiblingElement();
	}
	
	PushBack(clips, _type, _speed);

	delete document;
}

void Animator::ChangeAnimation()
{
	if (m_nextAnimation != nullptr)
	{
		m_currentAnimation = m_nextAnimation;
		m_nextAnimation = nullptr;
		m_currentAnimation->Play();
		return;
	}
	else // 다음 Animation이 없는 경우
	{
		if (m_currentAnimation->GetPlayType() == ANIM_PLAY_TYPE::LOOP)
		{
			m_currentAnimation->Reset();
			return;
		}
		else if (m_currentAnimation->GetPlayType() == ANIM_PLAY_TYPE::ONCE)
		{
			m_currentAnimation->Stop();
			return;
		}
		else if (m_currentAnimation->GetPlayType() == ANIM_PLAY_TYPE::PINGPONG)
		{
			m_currentAnimation->ReverseChange();
			m_currentAnimation->Finish(false);
			return;
		}
	}
}
// 이전 Animation의 상태 그대로 바꾸려는 Animation에게 전달한다.
void Animator::ChangeCurrentAnimation(Animation* _replacedAnim)
{
	m_previousAnimation = m_currentAnimation;
    m_currentAnimation = _replacedAnim;

	m_currentAnimation->m_isPlay          = m_previousAnimation->m_isPlay;
	m_currentAnimation->m_isPause         = m_previousAnimation->m_isPause;
	m_currentAnimation->m_isReverse       = m_previousAnimation->m_isReverse;
	m_currentAnimation->m_isFinish        = m_previousAnimation->m_isFinish;
	m_currentAnimation->m_time            = m_previousAnimation->m_time;
	m_currentAnimation->m_prevFrame       = m_previousAnimation->m_prevFrame;
	m_currentAnimation->m_currentFrame    = m_previousAnimation->m_currentFrame;
	m_currentAnimation->m_reserveEndFrame = m_previousAnimation->m_reserveEndFrame;
}

void Animator::OnUpdateWorldTransform()
{
}
