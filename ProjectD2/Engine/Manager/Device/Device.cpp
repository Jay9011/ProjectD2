#include "Framework.h"
#include "Device.h"

Device::Device(HWND _hWnd, RESOLUTION _resolution)
{
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
		_hWnd,
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

void Device::SetProjection(RESOLUTION _resolution)
{
	D3DXMatrixOrthoOffCenterLH
	(
		&projection,
		0.f, (float)_resolution.WIN_WIDTH,
		(float)_resolution.WIN_HEIGHT, 0.f,
		-1.f, 1.f
	);

	device->SetTransform(D3DTS_PROJECTION, &projection);
	device->SetRenderState(D3DRS_LIGHTING, false);
}
