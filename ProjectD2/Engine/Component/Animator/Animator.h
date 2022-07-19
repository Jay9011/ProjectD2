#pragma once
#include "Engine/Component/Component.h"
#include "Animation.h"

class Animator : public Component
{
public:
	Animator(GameObject* _owner, int _updateOrder = 100);
	virtual ~Animator();

public:
	void PushBack(const vector<Texture*>& _actions, const ANIM_PLAY_TYPE& _type = ANIM_PLAY_TYPE::Loop, const float& _speed = 0.1f);

	// Component을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual void FinalUpdate() override;
	
	void LoadXML(const string& _path, const string& _name, const ANIM_PLAY_TYPE& _type = ANIM_PLAY_TYPE::Loop, const float& _speed = 0.1f);

private:
	void ChangeAnimation();

public:
	class Shader* m_shader = nullptr;
	int m_passIndex = 0;
	
private:
	vector<Animation*> m_animations;
	Animation* m_previousAnimation = nullptr;
	Animation* m_currentAnimation = nullptr;
	Animation* m_nextAnimation = nullptr;

	
/* === === === === ===
*  Getter / Setter
* === === === === === */
public:
	size_t GetAnimationsSize() const { return m_animations.size(); }
	
	Texture* GetCurrentTexture() const  { return m_currentAnimation->GetCurrentTexture(); }
	Texture* GetPreviousTexture() const { return m_currentAnimation->GetPreviousTexture(); }

	void SetNextAnimation(Animation* _nextAnim)       { m_nextAnimation = _nextAnim; }
	void SetNextAnimation(const size_t& _index)       { m_nextAnimation = m_animations[_index]; }
	void SetCurrentAnimation(Animation* _currentAnim) { m_previousAnimation = m_currentAnimation; m_currentAnimation = _currentAnim; }
	
	void SetEvent(const size_t& _animation, const UINT& _index, const std::function<void()>& _callback) { m_animations[_animation]->SetFrameEvent(_index, _callback); }
	void SetEndEvent(const size_t& _animation, const std::function<void()>& _callback)                  { m_animations[_animation]->SetFinishFrameEvent(_callback); }
	
	Animation* Find(const UINT& _index) const { return m_animations[_index]; }
};

