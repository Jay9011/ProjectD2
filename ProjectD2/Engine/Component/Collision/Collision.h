#pragma once

#include "Collider.h"
#include "Colliders/AARect.h"
#include "Colliders/Circle.h"
#include "Colliders/Line.h"

class CollisionMgr
{
public:
	CollisionMgr(Game* _game);
	~CollisionMgr();

public:
	bool CheckCollision(Collider* _chkCollider, OBJECT_TYPE _type , OUT vector<std::pair<Collider*, Collider*>>& _vecCollList);
	bool CheckCollision(OBJECT_TYPE _typeA    , OBJECT_TYPE _typeB, OUT vector<std::pair<Collider*, Collider*>>& _vecCollList);

	void Update();
	void FinalUpdate();

	void AddCollider(OBJECT_TYPE _type, Collider* _collider);
	void RemoveCollider(OBJECT_TYPE _type, Collider* _collider);
	void RemoveColliderList(OBJECT_TYPE _type);
	void RemoveAllColliders();

private:
	Game* m_game;
	
	vector<Collider*> m_colliders[(UINT)OBJECT_TYPE::FIN];
};

/* === === === === ===
*   전역 충돌 함수
* === === === === === */

bool Collision(Collider* _a, Collider* _b);

bool Collision(const D3DXVECTOR2& _point, Collider* _other);

bool Collision(AARect* _rect, Collider* _other);
bool Collision(AARect* _rect, const D3DXVECTOR2& _point);
bool Collision(AARect* _rect, AARect* _other);
bool Collision(AARect* _rect, Circle* _circle);
bool Collision(AARect* _rect, Line* _line);

bool Collision(Circle* _circle, Collider* _other);
bool Collision(Circle* _circle, const D3DXVECTOR2& _point);
bool Collision(Circle* _circle, Circle* _other);
bool Collision(Circle* _circle, Line* _line);

bool Collision(Line* _line, Collider* _other);
bool Collision(Line* _line, const D3DXVECTOR2& _point);
bool Collision(Line* _line, Line* _other);

bool Collision(Collider* _a, const D3DXVECTOR2& _point) { return Collision(_point, _a); }
bool Collision(Collider* _a, AARect* _b)                { return Collision(_b, _a); }
bool Collision(Collider* _a, Circle* _b)                { return Collision(_b, _a); }
bool Collision(Collider* _a, Line*   _b)                { return Collision(_b, _a); }

bool Collision(const D3DXVECTOR2& _point, AARect* _rect)   { return Collision(_rect, _point); }
bool Collision(const D3DXVECTOR2& _point, Circle* _circle) { return Collision(_circle, _point); }
bool Collision(const D3DXVECTOR2& _point, Line*   _line)   { return Collision(_line, _point); }

bool Collision(Circle* _circle, AARect* _rect) { return Collision(_rect, _circle); }

bool Collision(Line* _line, AARect* _rect)   { return Collision(_rect, _line); }
bool Collision(Line* _line, Circle* _circle) { return Collision(_circle, _line); }
