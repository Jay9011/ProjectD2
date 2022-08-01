#pragma once
class BulletManager
{
public:
    BulletManager(UINT _poolCount , Scene* _scene, int _updateOrder = 100);
    BulletManager(UINT _poolCount , float _limitDist, float _damage, float _speed, Scene* _scene, int _updateOrder = 100);
    ~BulletManager();

    void SetScale(D3DXVECTOR2 _scale);
    void Fire(D3DXVECTOR2 _pos, D3DXVECTOR2 _dir, float _damage = 0.0f, float _speed = 0.0f);
    
private:
    vector<Bullet*> m_bullets;
    
};

