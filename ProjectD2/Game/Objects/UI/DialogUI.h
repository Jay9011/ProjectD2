#pragma once
#include "Engine/Object/UIObject.h"

enum class DialogFace
{
    Normal,
    Serious,
    Surprised,
    Face_FIN
};

class DialogUI : public UIObject
{
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
    ~DialogUI() override;

    // UIObject을(를) 통해 상속됨
    virtual void UpdateObject() override;
    virtual void RenderObject() override;

private:
    void InitTexture();
    void PrintKeyImg(D3DXVECTOR2 _pos, Keys _key);
    Keys GetKeyFromCommand(const wstring& _command);

private:
    Scene* m_scene;

    Shader* m_shader;

    Texture* m_dialogBox;   // 전체 dialogBox

    vector<Texture*> m_faces;
    DialogFace  m_face;
    D3DXVECTOR2 m_facePos;  // 얼굴의 위치
    
    RECT m_textBox;     // 텍스트를 담을 영역
    LPD3DXFONT m_font;  // 폰트 정보

    
    wstring m_text;             // 텍스트
    bool    m_isFirst = true;   // 텍스트를 처음 그리는지 확인용
    size_t  m_textIndex = 0;    // 텍스트 인덱스 (타이핑 효과용)
    float   m_textSpeed;        // 텍스트 타이핑 속도
    float   m_textTimer = 0.0f; // 타이핑 타이머

    bool    m_isWaiting = false; // 키 입력을 기다리는지 확인용
    bool    m_isWaitOver = false;// 키 입력 대기 시간을 초과했는지 확인용
    bool    m_isEnd = false;     // 대화 종료 여부
    bool    m_isWait = false;    // 대화 종료 후 대기 여부
    float   m_waitTime = 0.0f;   // 대기 시간
    float   m_waitTimer = 0.0f;  // 대기 타이머
    
    vector<Texture*> m_keys;

    std::function<void()> m_updateEvent;
    
/* === === === === === 
*  Getter / Setter
* === === === === === */
public:
    void SetText(const wstring& _text) { m_text = _text; };
    void SetTextSpeed(float _speed) { m_textSpeed = _speed; };
    void SetFace(DialogFace _face) { m_face = _face; };

    bool IsFirst() const        { return m_isFirst; };
    bool IsFirst(bool _isFirst) 
    { 
        m_isFirst = _isFirst;
        if (_isFirst)
        {
            Init();
        }
        return m_isFirst;
    };
    
    bool IsEnd() const      { return m_isEnd; };
    bool IsEnd(bool _isEnd) { m_isEnd = _isEnd; return m_isEnd; };
    
    bool IsWait() const       { return m_isWait; };
    bool IsWait(bool _isWait) { m_isWait = _isWait; return m_isWait; };

    bool IsWaiting() const          { return m_isWaiting; }
    bool IsWaiting(bool _isWaiting) { m_isWaiting = _isWaiting; return m_isWaiting; }

    float GetWaitTime() const      { return m_waitTime; };
    void  SetWaitTime(float _time) { m_waitTime = _time; };

    bool IsWaitOver() const       { return m_isWaitOver; };
    
    void SetUpdateEvent(std::function<void()> _event) { m_updateEvent = _event; };
    void Clear()
    {
        Init();
        m_text.clear();
        m_updateEvent = nullptr;
    }
    void Init()
    {
        m_face = DialogFace::Normal;
        m_isFirst = true;
        m_textIndex = 0;
        m_textTimer = 0.0f;
        m_isEnd = false;
        m_isWait = false;
        m_isWaitOver = false;
        m_waitTime = 0.0f;
        m_waitTimer = 0.0f;
    }
};

