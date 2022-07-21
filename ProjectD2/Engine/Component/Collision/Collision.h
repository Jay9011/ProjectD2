#pragma once
#include "Collider.h"
#include "Colliders/AARect.h"
#include "Colliders/Circle.h"

class Collision
{
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

bool Collision(Circle* _circle, Collider* _other);
bool Collision(Circle* _circle, const D3DXVECTOR2& _point);
bool Collision(Circle* _circle, Circle* _other);

bool Collision(Collider* _a, const D3DXVECTOR2& _point) { return Collision(_point, _a); }
bool Collision(Collider* _a, AARect* _b)                { return Collision(_b, _a); }
bool Collision(Collider* _a, Circle* _b)                { return Collision(_b, _a); }

bool Collision(const D3DXVECTOR2& _point, AARect*   _rect)   { return Collision(_rect, _point); }
bool Collision(const D3DXVECTOR2& _point, Circle*   _circle) { return Collision(_circle, _point); }

bool Collision(Circle* _circle, AARect* _rect) { return Collision(_rect, _circle); }
