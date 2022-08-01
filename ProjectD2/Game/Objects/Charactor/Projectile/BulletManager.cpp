#include "Framework.h"
#include "BulletManager.h"

#include "Bullet.h"

BulletManager::BulletManager(UINT _poolCount, Scene* _scene, int _updateOrder) :
    BulletManager(_poolCount, 200.0f, 0.0f, 0.0f, _scene, _updateOrder)
{
}

BulletManager::BulletManager(UINT _poolCount, float _limitDist, float _damage, float _speed, Scene* _scene, int _updateOrder)
{
	for (UINT i = 0; i < _poolCount; i++)
	{
		m_bullets.emplace_back(new Bullet(_limitDist, _damage, _speed, _scene, _updateOrder));
	}
}

BulletManager::~BulletManager()
{
}

void BulletManager::SetScale(D3DXVECTOR2 _scale)
{
	for (auto& bullet : m_bullets)
	{
		bullet->SetScale(_scale);
	}
}

void BulletManager::Fire(D3DXVECTOR2 _pos, D3DXVECTOR2 _dir, float _damage, float _speed)
{
    for (auto& bullet : m_bullets)
    {
        if (bullet->GetState() == OBJECT_STATE::INACTIVE)
        {
            bullet->SetPos(_pos);
            bullet->Fire(_dir, _damage, _speed);
            break;
        }
    }
}
