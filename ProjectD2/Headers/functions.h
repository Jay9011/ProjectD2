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
void Safe_Delete_Vec(vector<T>& _vec)
{
	for (auto& i : _vec)
	{
		SAFE_DELETE(i);
	}
	_vec.clear();
}

template<typename T1, typename T2>
void Safe_Delete_Map(map<T1, T2>& _map)
{
	for (auto& i : _map)
	{
		SAFE_DELETE(i.second);
	}
	_map.clear();
}
template<typename T1, typename T2>
void Safe_Delete_Map(unordered_map<T1, T2>& _map)
{
	for (auto& i : _map)
	{
		SAFE_DELETE(i.second);
	}
	_map.clear();
}

namespace Math
{
	bool NearZero(float _val, float _epsilon = 0.0001f);

	float       NearZeroValue(float       _val, float _epsilon = 0.0001f);
	D3DXVECTOR2 NearZeroValue(D3DXVECTOR2 _val, float _epsilon = 0.0001f);
	D3DXVECTOR3 NearZeroValue(D3DXVECTOR3 _val, float _epsilon = 0.0001f);

	float DistanceSq(D3DXVECTOR2 _v1, D3DXVECTOR2 _v2);
	float DistanceSq(D3DXVECTOR3 _v1, D3DXVECTOR3 _v2);

	float Distance(D3DXVECTOR2 _v1, D3DXVECTOR2 _v2);
	float Distance(D3DXVECTOR3 _v1, D3DXVECTOR3 _v2);
	
	float DegreeToRadian(float _degree);
	float RadianToDegree(float _radian);

	int   Random(const int&   _min, const int&   _max);
	float Random(const float& _min, const float& _max);

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