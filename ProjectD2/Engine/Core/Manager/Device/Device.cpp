#include "Framework.h"
#include "Device.h"

Device::Device(HWND* _hWnd, PRESOLUTION _resolution)
{
	m_phWnd = _hWnd;
	m_pResolution = _resolution;
	
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	
	D3DPRESENT_PARAMETERS param{};

	param.Windowed = true;
	param.SwapEffect = D3DSWAPEFFECT_DISCARD;
	param.EnableAutoDepthStencil = true;
	param.AutoDepthStencilFormat = D3DFMT_D16;
	param.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	d3d->CreateDevice
	(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		*m_phWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&param,
		&device
	);

	SetProjection(_resolution);
}

Device::~Device()
{
	d3d->Release();
}

void Device::SetProjection(const PRESOLUTION& _resolution)
{
	D3DXMatrixOrthoOffCenterLH
	(
		&projection,
		0.f, (float)_resolution->width,
		(float)_resolution->height, 0.f,
		-1.f, 1.f
	);

	device->SetTransform(D3DTS_PROJECTION, &projection);
	device->SetRenderState(D3DRS_LIGHTING, false);
}
