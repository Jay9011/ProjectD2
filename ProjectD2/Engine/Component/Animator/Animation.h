#pragma once
class Animator;

struct Frame
{
	Texture* texture;
	float    duration;
	function<void()> callbackEvent;
};

class Animation
{
public:
	Animation(const vector<Texture*>& _frames, ANIM_PLAY_TYPE _playType = ANIM_PLAY_TYPE::LOOP, float _speed = 0.1f);
	Animation(Animator* _owner, const vector<Texture*>& _frames, ANIM_PLAY_TYPE _playType = ANIM_PLAY_TYPE::LOOP, float _speed = 0.1f);
	~Animation();

	void Update();
	void FinalUpdate();
	void Render();
	
	void Play(Animation* _nextAnimation = nullptr);
	void Play(const size_t& _nextAnimIdx);
	void Play(const ANIM_PLAY_FLAG& _playFlag, const Animation* _beforeAnimation, Animation* _nextAnimation = nullptr);
	void Play(const ANIM_PLAY_FLAG& _playFlag, const int& _startFrame, const int& _endFrame, Animation* _nextAnimation = nullptr, const function<void()>& _reserveEvent = nullptr);
    void Play(const ANIM_PLAY_FLAG& _playFlag, const int& _startFrame, const int& _endFrame, const size_t& _nextAnimIdx, const function<void()>& _reserveEvent = nullptr);
	void Play(const ANIM_PLAY_FLAG& _playFlag, const int& _startFrame, const int& _endFrame, const bool& _isReversing, Animation* _nextAnimation = nullptr, const function<void()>& _reserveEvent = nullptr);
	void Play(const ANIM_PLAY_FLAG& _playFlag, const int& _startFrame, const int& _endFrame, const bool& _isReversing, const size_t& _nextAnimIdx, const function<void()>& _reserveEvent = nullptr);
	void Pause();
	void Stop();
	void Reset();

private:
	Animator* m_owner;
	
	ANIM_PLAY_TYPE m_playType;
	vector<Frame> m_frames;
	
	bool m_isPlay;
	bool m_isPause;
	bool m_isReverse;
	bool m_isFinish;
	
	float m_time;
	
	UINT m_prevFrame;
	UINT m_currentFrame;
	int  m_reserveEndFrame;

	function<void()> m_reserveEvent;

private:
	void PlayStateChange(const int& _startFrame = -1);
	
/* === === === === ===
*  Getter / Setter
* === === === === === */
public:
	Texture* GetCurrentTexture() const  { return m_frames[m_currentFrame].texture; }
	Texture* GetPreviousTexture() const { return m_frames[m_prevFrame].texture; }
	
	ANIM_PLAY_TYPE GetPlayType() const                         { return m_playType; }
	void           SetPlayType(const ANIM_PLAY_TYPE& playType) { m_playType = playType; }
	
	size_t GetFrameCount() const { return m_frames.size(); }
	Frame* GetFrame(const size_t& index) { return &m_frames[index]; }
    UINT   GetCurrentFrame() const { return m_currentFrame; }

	void  SetFrameDuration(const size_t& _index, const float& _duration) { m_frames[_index].duration = _duration; }
	float GetFrameDuration(const size_t& _index) const                   { return m_frames[_index].duration; }

	void SetFrameEvent(const size_t& _index, const function<void()>& _callback) { m_frames[_index].callbackEvent = _callback; }
	void SetFinishFrameEvent(const function<void()>& _callback){ m_frames.back().callbackEvent = _callback; }

	bool IsPlay() const    { return m_isPlay; }
    bool IsPause() const   { return m_isPause; }
	bool IsReverse() const { return m_isReverse; }
	bool IsDone() const    { return m_isFinish; }

	void ReverseChange()         { m_isReverse = !m_isReverse; }
	bool Finish(bool _finished)	 { m_isFinish = _finished; return m_isFinish; }

	friend class Animator;
};

