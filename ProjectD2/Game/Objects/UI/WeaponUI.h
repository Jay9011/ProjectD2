#pragma once
#include "Engine/Object/UIObject.h"
#include "Engine/Observer/Observer.h"

class WeaponUI :public UIObject, Observer<Player>
{
public:
    WeaponUI(Scene* _scene, int _updateOrder = 100, UIObject* _parent = nullptr);
    ~WeaponUI() override;

    // UIObject��(��) ���� ��ӵ�
    virtual void UpdateObject() override;
    virtual void RenderObject() override;

    // Observer��(��) ���� ��ӵ�
    virtual void FieldChanged(Player& field, const string& fieldName) override;

private:
    void SetAnimation();
    void PlayAnimation(PLAYER_EQUIP_TYPE _type);

private:
    Scene* m_scene;
    
    Animator* m_animator;
    
};

