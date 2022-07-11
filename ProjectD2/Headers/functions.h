#pragma once
/*
*  Function Macro
*/
#define SAFE_DELETE_VEC(_vec) Safe_Delete_Vec(_vec)
#define SAFE_DELETE_MAP(_map) Safe_Delete_Map(_map)

/*
*	Safe Delete Vector And Map
*/
template<typename T>
void Safe_Delete_Vec(std::vector<T>& _vec)
{
	for (auto& i : _vec)
	{
		SAFE_DELETE(i);
	}
	_vec.clear();
}

template<typename T1, typename T2>
void Safe_Delete_Map(std::map<T1, T2>& _map)
{
	for (auto& i : _map)
	{
		SAFE_DELETE(i.second);
	}
	_map.clear();
}
template<typename T1, typename T2>
void Safe_Delete_Map(std::unordered_map<T1, T2>& _map)
{
	for (auto& i : _map)
	{
		SAFE_DELETE(i.second);
	}
	_map.clear();
}

namespace Math
{
	bool NearZero(float _val, float _epsilon = 0.0001f)
	{
		return abs(_val) <= _epsilon;
	}

	float DistanceSq(D3DXVECTOR2 _v1, D3DXVECTOR2 _v2)
	{
		return (_v1.x - _v2.x) * (_v1.x - _v2.x) + (_v1.y - _v2.y) * (_v1.y - _v2.y);
	}

	float Distance(D3DXVECTOR2 _v1, D3DXVECTOR2 _v2)
	{
		return sqrt(DistanceSq(_v1, _v2));
	}

	float DistanceSq(D3DXVECTOR3 _v1, D3DXVECTOR3 _v2)
	{
		return (_v1.x - _v2.x) * (_v1.x - _v2.x) + (_v1.y - _v2.y) * (_v1.y - _v2.y) + (_v1.z - _v2.z) * (_v1.z - _v2.z);
	}

	float Distance(D3DXVECTOR3 _v1, D3DXVECTOR3 _v2)
	{
		return sqrt(DistanceSq(_v1, _v2));
	}
	
	float DegreeToRadian(float _degree)
	{
		return _degree * PI / 180.0f;
	}
	
	float RadianToDegree(float _radian)
	{
		return _radian * 180.0f / PI;
	}

	int Random(const int& _min, const int& _max)
	{
		return rand() % (_max - _min) + _min;
	}

	float Random(const float& _min, const float& _max)
	{
		return ((_max - _min) * ((float)rand() / (float)RAND_MAX)) + _min;
	}

	template <typename T>
	T Max(const T& _a, const T& _b)
	{
		return (_a > _b ? _a : _b);
	}

	template <typename T>
	T Min(const T& _a, const T& _b)
	{
		return (_a < _b ? _a : _b);
	}
}