#include "Framework.h"
#include "EffectMgr.h"

#include "Engine/Component/Animator/Animation.h"
#include "Engine/Object/Effect.h"

EffectMgr::EffectMgr()
{
}

EffectMgr::~EffectMgr()
{
	for (auto& effects : m_effectMap)
	{
		for (auto& effect : effects.second)
		{
			delete effect;
		}
	}
}

void EffectMgr::Update()
{
	for (auto& effects : m_effectMap)
	{
		for (auto& effect : effects.second)
		{
			effect->Update();
		}
	}
}

void EffectMgr::FinalUpdate()
{
    for (auto& effects : m_effectMap)
    {
        for (auto& effect : effects.second)
        {
            effect->FinalUpdate();
        }
    }
}

void EffectMgr::Render()
{
    for (auto& effects : m_effectMap)
    {
        for (auto& effect : effects.second)
        {
            effect->Render();
        }
    }
}

void EffectMgr::Add(const string& _key, const wstring& _path, UINT _poolCount, int _maxFrameX, int _maxFrameY, bool _isAdditive, float _speed)
{
	if (m_effectMap.find(_key) != m_effectMap.end())
		return;

	vector<Effect*> effects;

    for (UINT i = 0; i < _poolCount; i++)
    {
        auto effect = new Effect(_path, _maxFrameX, _maxFrameY, ANIM_PLAY_TYPE::ONCE, _isAdditive, _speed);
        effects.push_back(effect);
    }

	m_effectMap.insert(make_pair(_key, effects));
}

void EffectMgr::Play(const string& _key, D3DXVECTOR2 _pos, D3DXCOLOR _color)
{
    if (m_effectMap.find(_key) == m_effectMap.end())
        return;
    
    for (auto& effect : m_effectMap[_key])
    {
        if (!effect->IsActive())
        {
            effect->Play(_pos, _color);
            return;
        }
    }
}
