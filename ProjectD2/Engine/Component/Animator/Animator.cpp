#include "Framework.h"
#include "Animator.h"

#include "Engine/Singleton/Shader/Shader.h"
#include "Lib/XML.h"

Animator::Animator(GameObject* _owner, int _updateOrder) :
	Component(_owner, _updateOrder)
{
}

Animator::~Animator()
{
	SAFE_DELETE_VEC(m_animations);
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
		if (m_currentAnimation->GetPlayType() == ANIM_PLAY_TYPE::Loop)
		{
			m_currentAnimation->Reset();
			return;
		}
		else if (m_currentAnimation->GetPlayType() == ANIM_PLAY_TYPE::Once)
		{
			m_currentAnimation->Stop();
			return;
		}
		else if (m_currentAnimation->GetPlayType() == ANIM_PLAY_TYPE::PingPong)
		{
			m_currentAnimation->ReverseChange();
			m_currentAnimation->Finish(false);
			return;
		}
	}
}
