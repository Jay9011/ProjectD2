#pragma once
#include "Engine/Object/GameObject.h"
class PlatformRect : public GameObject
{
	enum class PlatformType
	{
		Texture,
		Animation
	};

public:
	// �ؽ�ó ��� ����
	PlatformRect(const D3DXVECTOR2& _size, const bool& _useCollider, Scene* _scene, OBJECT_TYPE _type, int _updateOrder = 100, GameObject* _parent = nullptr);
	// �ؽ�ó ���
	PlatformRect(const D3DXVECTOR2& _size, const wstring& _texturePath, const bool& _useCollider, Scene* _scene, OBJECT_TYPE _type, int _updateOrder = 100, GameObject* _parent = nullptr);
	// �ִϸ��̼� ���
	PlatformRect(const D3DXVECTOR2& _size, const wstring& _texturePath, const size_t& _maxFrameX, const size_t& _maxFrameY, const bool& _useCollider, Scene* _scene, OBJECT_TYPE _type, int _updateOrder = 100, GameObject* _parent = nullptr);
	~PlatformRect();

	// GameObject��(��) ���� ��ӵ�
	virtual void UpdateObject() override;
	virtual void RenderObject() override;

private:
	PlatformType m_type;
	Animator* m_animator;
	Texture*  m_texture;
	AARect*   m_collider;

	D3DXVECTOR2 m_size;
	D3DXVECTOR2 m_halfSize;

public:
	Animator* GetAnimator() { return m_animator; }
	Texture*  GetTexture()  { return m_texture; }
	AARect*   GetCollider() { return m_collider; }
	
	bool AnimPlay() { if (m_type == PlatformType::Animation) m_animator->Find(0)->Play(); }
	bool AnimStop() { if (m_type == PlatformType::Animation) m_animator->Find(0)->Stop(); }
};
