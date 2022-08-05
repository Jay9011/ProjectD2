#include "Framework.h"
#include "DialogUI.h"

#include "Engine/Component/Component.h"
#include "Engine/Component/Collision/Collision.h"
#include "Engine/Resource/Shader.h"

DialogUI::DialogUI(Scene* _scene, int _updateOrder, UIObject* _parent) :
    UIObject(_scene, _updateOrder, _parent)
    , m_scene(_scene)
    , m_shader(SHADER(L"AlphaShader"))
    , m_face(Face::Normal)
    , m_facePos({ -330.0f, 1.0f })
    , m_textBox({ -230, -50, 400, 50 })
    , m_textSpeed(0.02f)
{
    InitTexture();

    // Text 설정
    D3DXFONT_DESC desc = {};
    memset(&desc, 0, sizeof(D3DXFONT_DESC));
    desc.CharSet = HANGUL_CHARSET;
    wcscpy_s(desc.FaceName, L"맑은 고딕");
    desc.Height = 25;
    desc.Width = 9;
    desc.Weight = FW_BOLD;
    desc.Quality = DEFAULT_QUALITY;
    desc.MipLevels = 1;
    desc.Italic = false;
    desc.OutputPrecision = OUT_DEFAULT_PRECIS;
    desc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    
    D3DXCreateFontIndirect(DEVICE, &desc, &m_font);
    
    m_text = L"안녕하세요. 저는 당신의 여행을 도와줄 D2라고 합니다. /UP/ \n엔터 테스트";
}

DialogUI::~DialogUI()
{
    m_font->Release();
}

void DialogUI::UpdateObject()
{
    m_updateEvent();
}

void DialogUI::RenderObject()
{
    D3DXVECTOR2 pos = GetWorldPos();
    RECT rect = {pos.x + m_textBox.left, pos.y + m_textBox.top, pos.x + m_textBox.right, pos.y + m_textBox.bottom};

    m_shader->Begin();
    
    /*
    * Face 위치
    */
    AddPos(m_facePos.x, m_facePos.y);
    UpdateWorld();
    SetWorld();
    m_faces[m_face]->Render();
    SetPos(pos);
    UpdateWorld();
    SetWorld();
    
    m_dialogBox->Render();

    // 종료 대기 Key 출력
    if (m_isWait)
    {
        PrintKeyImg({ rect.right - 20.0f, rect.bottom - 20.0f }, Keys::F);
    }

    m_shader->End();

    if (m_text.size() > 0)
    {
        if (m_isFirst)
        {
            m_textIndex = 0;
            m_isFirst = false;
        }

        if (m_text.size() >= m_textIndex)   // 텍스트를 완성시키지 않았다면
        {
            m_textTimer += fDT; // 시간 기록
            
            if (m_textTimer >= m_textSpeed) // textSpeed에 해당하는 시간을 넘겼다면
            {
                m_textTimer = 0.0f;
                ++m_textIndex;          // text index 증가
            }
        }
        else    // 텍스트를 완성시킨 경우
        {
            m_isEnd = true;
            m_isWait = true;
        }
        
        m_font->DrawText(nullptr, m_text.c_str(), m_textIndex, &rect, DT_LEFT | DT_TOP, D3DCOLOR_ARGB(255, 255, 255, 255));
        
    }

}

void DialogUI::InitTexture()
{
    /*
    * Dialog
    */
    m_dialogBox = TEXTURE->Add(L"UI\\Dialog.png", 847, 148);

    /*
    *  얼굴 Texture
    */
    for (size_t i = 0; i < Face::Face_FIN; i++)
    {
        switch (i)
        {
        case DialogUI::Normal:
            m_faces.push_back(TEXTURE->Add(L"UI\\face\\3.png", 172, 130));
            break;
        case DialogUI::Serious:
            m_faces.push_back(TEXTURE->Add(L"UI\\face\\1.png", 172, 130));
            break;
        case DialogUI::Surprised:
            m_faces.push_back(TEXTURE->Add(L"UI\\face\\0.png", 172, 130));
            break;
        default:
            m_faces.push_back(TEXTURE->Add(L"UI\\face\\3.png", 172, 130));
            break;
        }
    }

    /*
    *  키보드 Texture
    */
    for (size_t i = 0; i < Keys::Keys_FIN; i++)
    {
        switch (i)
        {
        case DialogUI::LEFT:
            m_keys.push_back(TEXTURE->Add(L"UI\\LEFTKey.png", 19, 18));
            break;
        case DialogUI::RIGHT:
            m_keys.push_back(TEXTURE->Add(L"UI\\RIGHTKey.png", 19, 18));
            break;
        case DialogUI::UP:
            m_keys.push_back(TEXTURE->Add(L"UI\\UPKey.png", 19, 18));
            break;
        case DialogUI::D:
            m_keys.push_back(TEXTURE->Add(L"UI\\DKey.png", 19, 18));
            break;
        case DialogUI::F:
            m_keys.push_back(TEXTURE->Add(L"UI\\Fkey.png", 19, 18));
            break;
        case DialogUI::TAB:
            m_keys.push_back(TEXTURE->Add(L"UI\\TABKey.png", 36, 18));
            break;
        default:
            break;
        }
    }
}

void DialogUI::PrintKeyImg(D3DXVECTOR2 _pos, Keys _key)
{
    D3DXVECTOR2 pos = GetWorldPos();
    SetPos(_pos);
    UpdateWorld();
    SetWorld();

    m_keys[_key]->Render();
    
    SetPos(pos);
    UpdateWorld();
    SetWorld();
}

DialogUI::Keys DialogUI::GetKeyFromCommand(const wstring& _command)
{
    if (_command == L"UP")
    {
        return Keys::UP;
    }
    else if (_command == L"LEFT")
    {
        return Keys::LEFT;
    }
    else if (_command == L"RIGHT")
    {
        return Keys::RIGHT;
    }
    else if (_command == L"D")
    {
        return Keys::D;
    }
    else if (_command == L"F")
    {
        return Keys::F;
    }
    else if (_command == L"TAB")
    {
        return Keys::TAB;
    }

    return Keys::Keys_FIN;
}
