#include "Framework.h"
#include "Effect.h"

#include "Engine/Component/Component.h"
#include "Engine/Component/Animator/Animator.h"
#include "Engine/Resource/Shader.h"

Effect::Effect(const wstring& _path, int _maxFrameX, int _maxFrameY, ANIM_PLAY_TYPE _type, bool _isAdditive, float speed) :
    isActive(false)
    , isAdditive(_isAdditive)
    , m_color(1.0f, 1.0f, 1.0f, 1.0f)
{
	vector<Texture*> actions;

	for (int y = 0; y < _maxFrameY; y++)
	{
		for (int x = 0; x < _maxFrameX; x++)
		{
			actions.push_back(TEXTURE->Add(_path, _maxFrameX, _maxFrameY, x, y));
		}
	}
	m_shader = Shader::Add(L"AlphaShader");
	m_animation = new Animation(actions, _type, speed);
    
	if (_type == ANIM_PLAY_TYPE::ONCE)
	{
		m_animation->SetFinishFrameEvent([this]() {isActive = false; });
	}
}

Effect::~Effect()
{
	delete m_animation;
}

void Effect::Update()
{
	if (!isActive)
		return;

	m_animation->Update();
	UpdateWorld();
}

void Effect::Render()
{
	if (!isActive)
		return;

	SetWorld();

	m_shader->Set("Color", &m_color, 4);
	m_shader->Begin(isAdditive);

	m_animation->Render();

	m_shader->End();
}

void Effect::Play(D3DXVECTOR2 _pos, D3DXCOLOR _color)
{
	isActive = true;
	SetPos(_pos);
    m_color = _color;
    
	m_animation->Play();
}