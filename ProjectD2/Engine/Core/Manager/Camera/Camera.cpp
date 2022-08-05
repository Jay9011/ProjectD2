#include "Framework.h"
#include "Camera.h"

#include "Engine/Object/GameObject.h"

Camera::Camera() :
    m_speed(500)
    , m_target(nullptr)
    , m_offset(WIN_CENTER_X, WIN_CENTER_Y)
    , m_postOffset(m_offset)
    , m_restrictRange(-FLT_MAX, -FLT_MAX, FLT_MAX, FLT_MAX)
{}

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

    CameraRestrictCorrection(); // 카메라 위치 보정

    UpdateWorld();

    SetView();
}

void Camera::Render()
{

    CameraEffectProgress();
}

void Camera::SetView()
{
    DEVICE->SetTransform(D3DTS_VIEW, &m_world);
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
    D3DXVECTOR2 destPos = m_target->GetWorldPos();
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

void Camera::CameraRestrictCorrection()
{
    if (Left() < m_restrictRange.left)
    {
        AddPos(Left() - m_restrictRange.left, 0.0f);
    }
    else if (Right() > m_restrictRange.right)
    {
        AddPos(Right() - m_restrictRange.right, 0.0f);
    }

    if (Top() < m_restrictRange.top)
    {
        AddPos(0.0f, Top() - m_restrictRange.top);
    }
    else if (Bottom() > m_restrictRange.bottom)
    {
        AddPos(0.0f, Bottom() - m_restrictRange.bottom);
    }
}

inline void Camera::SetLookAt(const D3DXVECTOR2& _look)
{
    D3DXVECTOR2 destPos = _look;
    destPos = -destPos + m_postOffset;
    
    SetPos(destPos);
}

void Camera::RenderCameraRect()
{
    vector<VERTEXCOLOR> vertexList;
    vector<WORD>        indexList;

    D3DCOLOR color = 0xFFF637EC;

    vertexList.push_back(VERTEXCOLOR(Left()  + 1.0f , Top()    + 1.0f, color, 0.0f)); // LT
    vertexList.push_back(VERTEXCOLOR(Right() - 1.0f , Top()    + 1.0f, color, 0.0f)); // RT
    vertexList.push_back(VERTEXCOLOR(Right() - 1.0f , Bottom() - 1.0f, color, 0.0f)); // RB
    vertexList.push_back(VERTEXCOLOR(Left()  + 1.0f , Bottom() - 1.0f, color, 0.0f)); // LB

    indexList.push_back(0);
    indexList.push_back(1);
    indexList.push_back(2);
    indexList.push_back(3);
    indexList.push_back(0);

    D3DXMATRIX world;
    D3DXMatrixIdentity(&world);
    DEVICE->SetTransform(D3DTS_WORLD, &world);
    DEVICE->SetFVF(VERTEXCOLOR::FVF);
    DEVICE->DrawIndexedPrimitiveUP(D3DPT_LINESTRIP, 0, (UINT)vertexList.size(), (UINT)indexList.size() - 1, indexList.data(), D3DFMT_INDEX16, vertexList.data(), sizeof(VERTEXCOLOR));

    SetWorld();
}


/* === === === === ===
*  Getter / Setter
*  === === === === === */

inline float Camera::Left() 
{ 
    return -GetPos().x + m_reducer.x;
}
inline float Camera::Right() 
{ 
    return -GetPos().x + WIN_WIDTH - m_reducer.y;
}
inline float Camera::Top() 
{ 
    return -GetPos().y + m_reducer.x;
}
inline float Camera::Bottom() 
{ 
    return -GetPos().y + WIN_HEIGHT - m_reducer.y;
}
