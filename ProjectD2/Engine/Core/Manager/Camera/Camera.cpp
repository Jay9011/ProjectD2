#include "Framework.h"
#include "Camera.h"

Camera::Camera() :
    m_speed(200)
    , m_target(nullptr)
    , m_offset(WIN_CENTER_X, WIN_CENTER_Y)
    , m_restrictRange(FLT_MIN, FLT_MIN, FLT_MAX, FLT_MAX)
{
}

Camera::~Camera() = default;

void Camera::Update()
{
}

void Camera::FreeMode()
{
}

void Camera::TargetMode()
{
}
