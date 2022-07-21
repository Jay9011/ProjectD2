#include "Framework.h"
#include "functions.h"

bool Math::NearZero(float _val, float _epsilon)
{
	return abs(_val) <= _epsilon;
}

float Math::NearZeroValue(float _val, float _epsilon)
{
	return abs(_val) <= _epsilon ? 0.0f : _val;
}

D3DXVECTOR2 Math::NearZeroValue(D3DXVECTOR2 _val, float _epsilon)
{
	return D3DXVECTOR2(NearZeroValue(_val.x, _epsilon), NearZeroValue(_val.y, _epsilon));
}

D3DXVECTOR3 Math::NearZeroValue(D3DXVECTOR3 _val, float _epsilon)
{
	return D3DXVECTOR3(NearZeroValue(_val.x, _epsilon), NearZeroValue(_val.y, _epsilon), NearZeroValue(_val.z, _epsilon));
}

float Math::DistanceSq(D3DXVECTOR2 _v1, D3DXVECTOR2 _v2)
{
	return (_v1.x - _v2.x) * (_v1.x - _v2.x) + (_v1.y - _v2.y) * (_v1.y - _v2.y);
}

float Math::Distance(D3DXVECTOR2 _v1, D3DXVECTOR2 _v2)
{
	return sqrt(DistanceSq(_v1, _v2));
}

float Math::DistanceSq(D3DXVECTOR3 _v1, D3DXVECTOR3 _v2)
{
	return (_v1.x - _v2.x) * (_v1.x - _v2.x) + (_v1.y - _v2.y) * (_v1.y - _v2.y) + (_v1.z - _v2.z) * (_v1.z - _v2.z);
}

float Math::Distance(D3DXVECTOR3 _v1, D3DXVECTOR3 _v2)
{
	return sqrt(DistanceSq(_v1, _v2));
}

float Math::DegreeToRadian(float _degree)
{
	return _degree * PI / 180.0f;
}

float Math::RadianToDegree(float _radian)
{
	return _radian * 180.0f / PI;
}

int Math::Random(const int& _min, const int& _max)
{
	return rand() % (_max - _min) + _min;
}

float Math::Random(const float& _min, const float& _max)
{
	return ((_max - _min) * ((float)rand() / (float)RAND_MAX)) + _min;
}
