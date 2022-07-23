#include "Framework.h"
#include "Animation.h"

#include "Animator.h"

Animation::Animation(Animator* _owner, const vector<Texture*>& _frames, ANIM_PLAY_TYPE _playType, float _speed) :
	m_owner(_owner)
	, m_playType(_playType)
	, m_isPlay(false)
	, m_isPause(false)
	, m_isReverse(false)
	, m_isFinish(false)
	, m_time(0.0f)
	, m_prevFrame(0)
	, m_currentFrame(0)
{
	Frame frame = {};
	
	for (Texture* texture : _frames)
	{
		frame.texture = texture;
		frame.duration = _speed;
		
		m_frames.push_back(frame);
	}
}

Animation::~Animation()
{
}

void Animation::Update()
{
	if (!m_isPlay)
		return;

	m_time += fDT;
	
	if (m_time >= m_frames[m_currentFrame].duration)
	{
		m_time -= m_frames[m_currentFrame].duration;
		m_prevFrame = m_currentFrame;
		
		if (!m_isReverse)
		{
			if (++m_currentFrame >= m_frames.size())
			{
				m_isFinish = true;
				--m_currentFrame;
			}
		}
		else
		{
			if (--m_currentFrame <= 0)
			{
				m_isFinish = true;
			}
		}

	}
}

void Animation::FinalUpdate()
{
	if (m_frames[m_prevFrame].callbackEvent != nullptr)
		m_frames[m_prevFrame].callbackEvent();
}

void Animation::Render()
{
	m_frames[m_currentFrame].texture->Render();
}

void Animation::Play()
{
	Play(nullptr);
}

void Animation::Play(Animation* _nextAnimation)
{
	m_owner->SetCurrentAnimation(this);
	
	if(_nextAnimation != nullptr)
		m_owner->SetNextAnimation(_nextAnimation);
	
	PlayStateChange();
}

void Animation::Play(const size_t& _nextAnimIdx)
{
	m_owner->SetCurrentAnimation(this);
	
	if(_nextAnimIdx < m_owner->GetAnimationsSize())
		m_owner->SetNextAnimation(_nextAnimIdx);

	PlayStateChange();
}

void Animation::PlayStateChange()
{
	if (m_isPause)
	{
		m_isPause = false;
	}
	else
	{
		m_isPause = true;
		m_currentFrame = 0;
		m_time = 0.0f;
		m_isReverse = false;
		m_isFinish = false;
	}

	m_isPlay = true;
}

void Animation::Pause()
{
	m_isPlay = false;
	m_isPause = true;
}

void Animation::Stop()
{
	m_isPlay = false;
	m_isPause = false;
	m_isReverse = false;
}

void Animation::Reset()
{
	m_isPlay = true;
	m_isPause = false;
	m_currentFrame = 0;
	m_time = 0.0f;
	m_isReverse = false;
	m_isFinish = false;
}
