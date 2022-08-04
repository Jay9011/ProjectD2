#pragma once
#include "Engine/Object/UIObject.h"
#include "Engine/Observer/Observer.h"
class Player;

enum class HEART_STATE
{
    Empty,
    Full,
    FIN
};
//
//class Heart : public UIObject
//{
//public:
//    Heart(Scene* _scene, int _updateOrder = 100, UIObject* _parent = nullptr);
//    ~Heart();
//    
//    // UIObject을(를) 통해 상속됨
//    virtual void UpdateObject() override;
//    virtual void RenderObject() override;
//
//private:
//    class Animator* animator;
//
//    friend class HeartUI;
//};

class HeartUI : public UIObject, public Observer<Player>
{
    
public:
    HeartUI(Scene* _scene, int _updateOrder = 100, UIObject* _parent = nullptr);
    ~HeartUI();

    // UIObject을(를) 통해 상속됨
    virtual void UpdateObject() override;
    virtual void RenderObject() override;

    // Observer을(를) 통해 상속됨
    virtual void FieldChanged(Player& field, const string& fieldName) override;

private:
    void InitHeart();

private:
    Scene* m_scene;

    float m_maxHP = 0;
    float m_HP = 0;

    Animator* m_animator;

    /*vector<Heart*> m_hearts;*/
};

