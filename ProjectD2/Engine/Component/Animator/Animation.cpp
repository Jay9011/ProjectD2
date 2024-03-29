#include "Framework.h"
#include "Animation.h"

#include "Animator.h"

Animation::Animation(const vector<Texture*>& _frames, ANIM_PLAY_TYPE _playType, float _speed) :
	m_playType(_playType)
	, m_isPlay(false)
	, m_isPause(false)
	, m_isReverse(false)
	, m_isFinish(false)
	, m_time(0.0f)
	, m_prevFrame(0)
	, m_currentFrame(0)
	, m_reserveEndFrame(-1)
{
	Frame frame = {};

	for (Texture* texture : _frames)
	{
		frame.texture = texture;
		frame.duration = _speed;

		m_frames.push_back(frame);
	}
}

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
	, m_reserveEndFrame(-1)
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

	if (m_currentFrame == m_reserveEndFrame)
	{
		m_reserveEndFrame = -1;
		m_reserveEvent();
		Reset();
	}
}

void Animation::Render()
{
	m_frames[m_currentFrame].texture->Render();
}

void Animation::Play(Animation* _nextAnimation)
{
    if(m_owner != nullptr)
	{
		m_owner->SetCurrentAnimation(this);
	
		if(_nextAnimation != nullptr)
			m_owner->SetNextAnimation(_nextAnimation);
	}
	
	PlayStateChange();
}

void Animation::Play(const size_t& _nextAnimIdx)
{
	if (m_owner != nullptr)
	{
		m_owner->SetCurrentAnimation(this);
	
		if(_nextAnimIdx < m_owner->GetAnimationsSize())
			m_owner->SetNextAnimation(_nextAnimIdx);
	}

	PlayStateChange();
}

void Animation::Play(const ANIM_PLAY_FLAG& _playFlag, const Animation* _beforeAnimation, Animation* _nextAnimation)
{
	switch (_playFlag)
	{
	case ANIM_PLAY_FLAG::ChangeSameMotion:
		Play(_nextAnimation);
		m_isReverse = _beforeAnimation->m_isReverse;
		m_time = _beforeAnimation->m_time;
		m_prevFrame = _beforeAnimation->m_prevFrame <= m_frames.size() ? _beforeAnimation->m_prevFrame : (UINT)m_frames.size() - 1;
		m_currentFrame = _beforeAnimation->m_currentFrame <= m_frames.size() ? _beforeAnimation->m_currentFrame : (UINT)m_frames.size() - 1;
		m_reserveEndFrame = _beforeAnimation->m_reserveEndFrame <= m_frames.size() ? _beforeAnimation->m_reserveEndFrame : (UINT)m_frames.size() - 1;
		break;
	default:
		Play(_nextAnimation);
		break;
	}
}

void Animation::Play(const ANIM_PLAY_FLAG& _playFlag, const int& _startFrame, const int& _endFrame, Animation* _nextAnimation, const function<void()>& _reserveEvent)
{
	switch (_playFlag)
	{
	case ANIM_PLAY_FLAG::SetFrameToEnd:
		Play(_nextAnimation);
		m_currentFrame = _startFrame;
		break;
	case ANIM_PLAY_FLAG::SetFrameToSetEndFrame:
		Play(_nextAnimation);
		m_currentFrame = _startFrame;
		m_reserveEvent = _reserveEvent;
		if (_endFrame >= 0)
		{
			m_reserveEndFrame = _endFrame >= m_frames.size() ? (UINT)m_frames.size() - 1 : (UINT)_endFrame;
		}
		break;
	default:
		Play(_nextAnimation);
		break;
	}
}

void Animation::Play(const ANIM_PLAY_FLAG& _playFlag, const int& _startFrame, const int& _endFrame, const size_t& _nextAnimIdx, const function<void()>& _reserveEvent)
{
	switch (_playFlag)
	{
	case ANIM_PLAY_FLAG::SetFrameToEnd:
		Play(_nextAnimIdx);
		m_currentFrame = _startFrame;
		break;
	case ANIM_PLAY_FLAG::SetFrameToSetEndFrame:
		Play(_nextAnimIdx);
		m_currentFrame = _startFrame;
		m_reserveEvent = _reserveEvent;
		if (_endFrame >= 0)
		{
			m_reserveEndFrame = _endFrame >= m_frames.size() ? (UINT)m_frames.size() - 1 : (UINT)_endFrame;
		}
		break;
	default:
		Play(_nextAnimIdx);
		break;
	}
}

void Animation::Play(const ANIM_PLAY_FLAG& _playFlag, const int& _startFrame, const int& _endFrame, const bool& _isReversing, Animation* _nextAnimation, const function<void()>& _reserveEvent)
{
	Play(_playFlag, _startFrame, _endFrame, _nextAnimation, _reserveEvent);
	m_isReverse = _isReversing;
}

void Animation::Play(const ANIM_PLAY_FLAG& _playFlag, const int& _startFrame, const int& _endFrame, const bool& _isReversing, const size_t& _nextAnimIdx, const function<void()>& _reserveEvent)
{
	Play(_playFlag, _startFrame, _endFrame, _nextAnimIdx, _reserveEvent);
	m_isReverse = _isReversing;
}

void Animation::PlayStateChange(const int& _startFrame)
{
	m_isPause = false;

	if (m_isPause)
		return;

	m_isPlay = true;
	m_isReverse = false;
	m_isFinish = false;
	m_time = 0.0f;

	m_prevFrame = 0;
	m_currentFrame = _startFrame < 0 ? 0 : _startFrame;
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
