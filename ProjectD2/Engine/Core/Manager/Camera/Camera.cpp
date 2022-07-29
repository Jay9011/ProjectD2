#include "Framework.h"
#include "Camera.h"

#include "Engine/Object/GameObject.h"

Camera::Camera() :
    m_speed(500)
    , m_target(nullptr)
    , m_offset(WIN_CENTER_X, WIN_CENTER_Y)
    , m_restrictRange(FLT_MIN, FLT_MIN, FLT_MAX, FLT_MAX)
{
}

Camera::~Camera() = default;

void Camera::Update()
{
    if (m_target)
    {
        // ���� DEAD�� ������ Object Type���� Ȯ��
        const GameObject* obj = dynamic_cast<GameObject*>(m_target);

        if(obj != nullptr && obj->IsDead())
        {
            m_target = nullptr;
        }
    }

    if (m_target == nullptr)
    {
        FreeMode();
    }
    else
    {
        TargetMode();
    }

    UpdateWorld();

    DEVICE->SetTransform(D3DTS_VIEW, &m_world);
}

void Camera::FreeMode()
{
	static D3DXVECTOR2 prevPos = { WIN_CENTER_X, WIN_CENTER_Y };
    
	// ���콺 ��� ��ư�� ������ �� �ش� ��ǥ�� �����Ѵ�.
	if (KEYDOWN(VK_RBUTTON))
	{
		prevPos = MOUSEPOS;
	}

	if (KEYPRESS(VK_RBUTTON))
	{
        // ���콺 ��� ��ư�� ������ ���� �� ī�޶��� ��ġ�� �̵���Ų��.
        D3DXVECTOR2 delta = MOUSEPOS - prevPos;
        D3DXVec2Normalize(&delta, &delta);

        AddPos(-delta * m_speed * fDT);
	}
}

void Camera::TargetMode()
{
    D3DXVECTOR2 destPos = m_target->GetPos();
    destPos = -destPos + m_offset;
    
    D3DXVECTOR2 cameraPos = GetPos();
    
    D3DXVec2Lerp(&cameraPos, &cameraPos, &destPos, fDT);
    SetPos(cameraPos);
}
