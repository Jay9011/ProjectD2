#pragma once

#define SAFE_DELETE(instance) if(instance != nullptr) { delete instance; instance = nullptr;}
#define SAFE_DELETE_VEC(_vec) Safe_Delete_Vec(_vec)
#define SAFE_DELETE_MAP(_map) Safe_Delete_Map(_map)

using std::vector;
using std::map;
/*
*	Safe Delete Vector And Map
*/
template<typename T>
void Safe_Delete_Vec(vector<T>&_vec)
{
	for (auto& i : _vec)
	{
		SAFE_DELETE(i);
	}
	_vec.clear();
}

template<typename T1, typename T2>
void Safe_Delete_Map(map<T1, T2>&_map)
{
	for (auto& i : _map)
	{
		SAFE_DELETE(i.second);
	}
	_map.clear();
}