#pragma once
class Device
{
public:
	Device();
	~Device();

	void SetProjection();
	
	LPDIRECT3DDEVICE9 Get() const { return device; }
	
private:
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 device;
	
	D3DXMATRIX projection;
};
