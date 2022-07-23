#include "Framework.h"
#include "PlatformRect.h"

#include "Engine/Component/Component.h"
#include "Engine/Resource/Shader.h"
#include "Engine/Component/Animator/Animator.h"
#include "Engine/Component/Collision/Collider.h"
#include "Engine/Component/Collision/Colliders/AARect.h"

PlatformRect::PlatformRect(const D3DXVECTOR2& _size, const bool& _useCollider, Scene* _scene, OBJECT_TYPE _type, int _updateOrder, GameObject* _parent) :
	GameObject(_scene, _type, _updateOrder, _parent)
	, m_size(_size)
	, m_halfSize(_size * 0.5f)
{
	if (_useCollider)
	{
		m_collider = ADDCOMP::NewAARect(-m_halfSize, m_halfSize, this);
		m_collider->IsActive(true);
	}
}

PlatformRect::PlatformRect(const D3DXVECTOR2& _size, const wstring& _texturePath, const bool& _useCollider, Scene* _scene, OBJECT_TYPE _type, int _updateOrder, GameObject* _parent) :
	PlatformRect(_size, _useCollider, _scene, _type, _updateOrder, _parent)
{
	m_texture = TEXTURE->Add(_texturePath, (UINT)_size.x, (UINT)_size.y, {0, 0}, {_size.x, _size.y});
	m_type = PlatformType::Texture;
}

PlatformRect::PlatformRect(const D3DXVECTOR2& _size, const wstring& _texturePath, const size_t& _maxFrameX, const size_t& _maxFrameY, const bool& _useCollider, Scene* _scene, OBJECT_TYPE _type, int _updateOrder, GameObject* _parent) :
	PlatformRect(_size, _texturePath, _useCollider, _scene, _type, _updateOrder, _parent)
{
	m_animator = ADDCOMP::NewAnimator(this, SHADER(L"AlphaShader"));
	
	vector<Texture*> frames;
	
	for (int y = 0; y < _maxFrameY; ++y)
	{
		for (int x = 0; x < _maxFrameX; ++x)
		{
			frames.push_back(TEXTURE->Add(_texturePath, (int)_maxFrameX, (int)_maxFrameY, x, y));
		}
	}

	m_animator->PushBack(frames);

	m_type = PlatformType::Animation;
}

PlatformRect::~PlatformRect() = default;

void PlatformRect::UpdateObject()
{
}

void PlatformRect::RenderObject()
{
	if (m_type == PlatformType::Texture)
	{
		m_texture->Render();
	}
}
