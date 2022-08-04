#pragma once
#include "Engine/Object/UIObject.h"
#include "Engine/Observer/Observer.h"
class Player;
class HeartUI : public UIObject, public Observer<Player>
{
    enum Heart
    {
        Full,
        Empty,
        FIN
    };
public:
    HeartUI(Scene* _scene, int _updateOrder = 100, UIObject* _parent = nullptr);
    ~HeartUI();

    // UIObject��(��) ���� ��ӵ�
    virtual void UpdateObject() override;
    virtual void RenderObject() override;

    // Observer��(��) ���� ��ӵ�
    virtual void FieldChanged(Player& field, const string& fieldName) override;

private:
    void SetAnimation();

private:
    Scene* m_scene;

    Animator* m_animator;

    float m_maxHP;
    float m_HP;

};

