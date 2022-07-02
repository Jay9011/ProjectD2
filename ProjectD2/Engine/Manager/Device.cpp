#include "Framework.h"
#include "Device.h"

#include "Engine/Core/Core.h"

Device::Device()
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	
	D3DPRESENT_PARAMETERS param{};

	param.Windowed = true;
	param.SwapEffect = D3DSWAPEFFECT_DISCARD;
	param.EnableAutoDepthStencil = true;
	param.AutoDepthStencilFormat = D3DFMT_D16;

	d3d->CreateDevice
	(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		Core::Get()->GetHWND(),
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&param,
		&device
	);

	SetProjection();
}

Device::~Device()
{
	d3d->Release();
}

void Device::SetProjection()
{
	D3DXMatrixOrthoOffCenterLH
	(
		&projection,
		0.f, (float)Core::Get()->WIN_WIDTH(),
		(float)Core::Get()->WIN_HEIGHT(), 0.f,
		-1.f, 1.f
	);

	device->SetTransform(D3DTS_PROJECTION, &projection);
	device->SetRenderState(D3DRS_LIGHTING, false);
}
