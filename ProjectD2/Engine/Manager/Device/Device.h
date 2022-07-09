#pragma once
class Device
{
public:
	Device(HWND* _hWnd, PRESOLUTION _resolution);
	~Device();

	void SetProjection(const PRESOLUTION& _resolution);
	
	LPDIRECT3DDEVICE9 Get() const { return device; }
	
private:
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 device;
	
	D3DXMATRIX projection;

	/* === === === === ===
	* 주입 받은 포인터 저장용
	* === === === === === */
	HWND* m_phWnd;
	PRESOLUTION m_pResolution;
};
