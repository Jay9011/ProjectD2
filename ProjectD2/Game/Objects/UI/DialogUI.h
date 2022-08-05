#pragma once
#include "Engine/Object/UIObject.h"

class DialogUI : public UIObject
{
    enum Face
    {
        Normal,
        Serious,
        Surprised,
        Face_FIN
    };
    enum Keys
    {
        LEFT,
        RIGHT,
        UP,
        D,
        F,
        TAB,
        Keys_FIN
    };
public:
    DialogUI(Scene* _scene, int _updateOrder = 100, UIObject* _parent = nullptr);
    ~DialogUI();

    // UIObject��(��) ���� ��ӵ�
    virtual void UpdateObject() override;
    virtual void RenderObject() override;

private:
    void InitTexture();
    void PrintKeyImg(D3DXVECTOR2 _pos, Keys _key);
    Keys GetKeyFromCommand(const wstring& _command);

private:
    Scene* m_scene;

    Shader* m_shader;

    Texture* m_dialogBox;   // ��ü dialogBox

    vector<Texture*> m_faces;
    Face m_face;
    D3DXVECTOR2 m_facePos;  // ���� ��ġ
    
    RECT m_textBox;     // �ؽ�Ʈ�� ���� ����
    LPD3DXFONT m_font;  // ��Ʈ ����

    
    wstring m_text;             // �ؽ�Ʈ
    bool    m_isFirst = true;   // �ؽ�Ʈ�� ó�� �׸����� Ȯ�ο�
    size_t  m_textIndex = 0;    // �ؽ�Ʈ �ε��� (Ÿ���� ȿ����)
    float   m_textSpeed;        // �ؽ�Ʈ Ÿ���� �ӵ�
    float   m_textTimer = 0.0f; // Ÿ���� Ÿ�̸�

    bool    m_isEnd = false;    // ��ȭ ���� ����
    bool    m_isWait = false;   // ��ȭ ���� �� ��� ����
    float   m_waitTime = 0.0f;  // ��� �ð�
    float   m_waitTimer = 0.0f; // ��� Ÿ�̸�
    
    vector<Texture*> m_keys;

    std::function<void()> m_updateEvent;
    
/* === === === === === 
*  Getter / Setter
* === === === === === */
public:
    void SetText(const wstring& _text) { m_text = _text; };
    void SetTextSpeed(float _speed) { m_textSpeed = _speed; };

    bool IsFirst() const        { return m_isFirst; };
    bool IsFirst(bool _isFirst) 
    { 
        m_isFirst = _isFirst;
        if (_isFirst)
        {
            m_textIndex = 0;
            m_textTimer = 0.0f;
            m_isEnd = false;
            m_isWait = false;
            m_waitTimer = 0.0f;
        }
        return m_isFirst;
    };
    
    bool IsEnd() const      { return m_isEnd; };
    bool IsEnd(bool _isEnd) { m_isEnd = _isEnd; return m_isEnd; };
    
    bool IsWait() const       { return m_isWait; };
    bool IsWait(bool _isWait) { m_isWait = _isWait; return m_isWait; };

    float GetWaitTime() const      { return m_waitTime; };
    void  SetWaitTime(float _time) { m_waitTime = _time; };
    
    void SetUpdateEvent(std::function<void()> _event) { m_updateEvent = _event; };
    void Clear()
    {
        m_text.clear();
        m_isFirst = true;
        m_textIndex = 0;
        m_textTimer = 0.0f;
        m_isEnd = false;
        m_isWait = false;
        m_waitTimer = 0.0f;
        m_updateEvent = nullptr;
    }
};

