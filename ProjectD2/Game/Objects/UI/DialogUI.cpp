#include "Framework.h"
#include "DialogUI.h"

#include "Engine/Component/Component.h"
#include "Engine/Component/Collision/Collision.h"
#include "Engine/Resource/Shader.h"

DialogUI::DialogUI(Scene* _scene, int _updateOrder, UIObject* _parent) :
    UIObject(_scene, _updateOrder, _parent)
    , m_scene(_scene)
    , m_shader(SHADER(L"AlphaShader"))
    , m_face(DialogFace::Normal)
    , m_facePos({ -330.0f, 1.0f })
    , m_textBox({ -230, -33, 400, 70 })
    , m_textSpeed(0.02f)
{
    InitTexture();

    // Text ����
    D3DXFONT_DESC desc = {};
    memset(&desc, 0, sizeof(D3DXFONT_DESC));
    desc.CharSet = HANGUL_CHARSET;
    wcscpy_s(desc.FaceName, L"���� ���");
    desc.Height = 25;
    desc.Width = 9;
    desc.Weight = FW_BOLD;
    desc.Quality = DEFAULT_QUALITY;
    desc.MipLevels = 1;
    desc.Italic = false;
    desc.OutputPrecision = OUT_DEFAULT_PRECIS;
    desc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    
    D3DXCreateFontIndirect(DEVICE, &desc, &m_font);

    SetState(OBJECT_STATE::HIDDEN);
}

DialogUI::~DialogUI()
{
    m_font->Release();
}

void DialogUI::UpdateObject()
{
    if(m_updateEvent != nullptr)
        m_updateEvent();
}

void DialogUI::RenderObject()
{
    D3DXVECTOR2 pos = GetPos();
    RECT rect = {pos.x + m_textBox.left, pos.y + m_textBox.top, pos.x + m_textBox.right, pos.y + m_textBox.bottom};

    m_shader->Begin();
    
    /*
    * Face ��ġ
    */
    AddPos(m_facePos.x, m_facePos.y);
    UpdateWorld();
    SetWorld();
    m_faces[(int)m_face]->Render();
    SetPos(pos);
    UpdateWorld();
    SetWorld();
    
    m_dialogBox->Render();

    if (m_waitTime > 0 && m_waitTimer >= m_waitTime)
    {
        m_isWaitOver = true;
    }

    // ���� ��� Key ���
    if (m_isWait)
    {
        PrintKeyImg({ rect.right - 20.0f, rect.bottom - 35.0f }, Keys::F);
    }

    m_shader->End();

    if (m_text.size() > 0)
    {
        if (m_isFirst)
        {
            m_textIndex = 0;
            m_isFirst = false;
        }

        if (m_text.size() >= m_textIndex)   // �ؽ�Ʈ�� �ϼ���Ű�� �ʾҴٸ�
        {
            m_textTimer += fDT; // �ð� ���
            
            if (m_textTimer >= m_textSpeed) // textSpeed�� �ش��ϴ� �ð��� �Ѱ�ٸ�
            {
                m_textTimer = 0.0f;
                ++m_textIndex;          // text index ����
            }
        }
        else    // �ؽ�Ʈ�� �ϼ���Ų ���
        {
            m_waitTimer += fDT;
            m_isEnd = true;
            if(m_isWaiting)
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
    *  �� Texture
    */
    for (size_t i = 0; i < (int)DialogFace::Face_FIN; i++)
    {
        switch ((DialogFace)i)
        {
        case DialogFace::Normal:
            m_faces.push_back(TEXTURE->Add(L"UI\\face\\3.png", 172, 130));
            break;
        case DialogFace::Serious:
            m_faces.push_back(TEXTURE->Add(L"UI\\face\\1.png", 172, 130));
            break;
        case DialogFace::Surprised:
            m_faces.push_back(TEXTURE->Add(L"UI\\face\\0.png", 172, 130));
            break;
        default:
            m_faces.push_back(TEXTURE->Add(L"UI\\face\\3.png", 172, 130));
            break;
        }
    }

    /*
    *  Ű���� Texture
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
