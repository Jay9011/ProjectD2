#include "Framework.h"
#include "Camera.h"

#include "Engine/Object/GameObject.h"

Camera::Camera() :
    m_speed(500)
    , m_target(nullptr)
    , m_offset(WIN_CENTER_X, WIN_CENTER_Y)
    , m_postOffset(m_offset)
    , m_restrictRange(FLT_MIN, FLT_MIN, FLT_MAX, FLT_MAX)
{
}

Camera::~Camera() = default;

void Camera::Update()
{
    if (m_target)
    {
        // 만약 DEAD가 가능한 Object Type인지 확인
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

void Camera::Render()
{

    CameraEffectProgress();
}

void Camera::FreeMode()
{
	static D3DXVECTOR2 prevPos = { WIN_CENTER_X, WIN_CENTER_Y };
    
	// 마우스 가운데 버튼을 눌렀을 때 해당 좌표를 저장한다.
	if (KEYDOWN(VK_RBUTTON))
	{
		prevPos = MOUSEPOS;
	}

	if (KEYPRESS(VK_RBUTTON))
	{
        // 마우스 가운데 버튼을 누르고 있을 때 카메라의 위치를 이동시킨다.
        D3DXVECTOR2 delta = MOUSEPOS - prevPos;
        D3DXVec2Normalize(&delta, &delta);

        AddPos(-delta * m_speed * fDT);
	}
}

void Camera::TargetMode()
{
    D3DXVECTOR2 destPos = m_target->GetPos();
    destPos = -destPos + m_postOffset;
    
    D3DXVECTOR2 cameraPos = GetPos();
    
    D3DXVec2Lerp(&cameraPos, &cameraPos, &destPos, fDT * 4.5f);
    SetPos(cameraPos);
}

void Camera::CameraEffectProgress()
{
    if (m_effectQueue.empty())
        return;

    auto& effect = m_effectQueue.front();
    effect.progTime += fDT;
    
    float percent = effect.progTime / effect.duration;
    if(percent < 0.0f)
    {
        percent = 0.0f;
    }
    else if(percent > 1.0f)
    {
        percent = 1.0f;
    }

    /*
    *  Effect 처리
    */
    switch (effect.effect)
    {
    case CAM_EFFECT::SHAKE:
    {
        sign *= -1;
        D3DXVECTOR2 offset = m_offset - D3DXVECTOR2(sign * effect.power, sign * effect.power);
        m_postOffset = offset;
    }
        break;
    default:
        break;
    }
    
    /*
    * Effect 종료 처리
    */
    if (effect.progTime >= effect.duration)
    {
        m_effectQueue.pop();
        m_postOffset = m_offset;
    }
}

inline void Camera::SetLookAt(const D3DXVECTOR2& _look)
{
    D3DXVECTOR2 destPos = _look;
    destPos = -destPos + m_postOffset;
    
    SetPos(destPos);
}
