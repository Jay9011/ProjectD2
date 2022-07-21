#include "Framework.h"
#include "Collision.h"

bool Collision(Collider* _a, Collider* _b)
{
    if (!_a->IsActive() || !_b->IsActive())
        return false;

    COLLIDER_TYPE typeA = _a->GetColliderType();

    switch (typeA)
    {
    case COLLIDER_TYPE::LINE:
        break;
    case COLLIDER_TYPE::AARECT:
        Collision((AARect*)_a, _b);
        break;
    case COLLIDER_TYPE::CIRCLE:
        Collision((Circle*)_a, _b);
        break;
    default:
        break;
    }

    return false;
}

bool Collision(const D3DXVECTOR2& _point, Collider* _other)
{
    if (!_other->IsActive())
        return false;
	
	COLLIDER_TYPE typeB = _other->GetColliderType();
	
    switch (typeB)
    {
    case COLLIDER_TYPE::LINE:
        break;
    case COLLIDER_TYPE::AARECT:
		Collision((AARect*)_other, _point);
        break;
    case COLLIDER_TYPE::CIRCLE:
		Collision((Circle*)_other, _point);
        break;
    default:
        break;
    }

	
    return false;
}

bool Collision(AARect* _rect, Collider* _other)
{
    if (!_rect->IsActive() || !_other->IsActive())
        return false;
	
	COLLIDER_TYPE typeB = _other->GetColliderType();
	
    switch (typeB)
    {
    case COLLIDER_TYPE::LINE:
        break;
    case COLLIDER_TYPE::AARECT:
		Collision(_rect, (AARect*)_other);
        break;
    case COLLIDER_TYPE::CIRCLE:
		Collision(_rect, (Circle*)_other);
        break;
    default:
        break;
    }
	
    return false;
}

bool Collision(AARect* _rect, const D3DXVECTOR2& _point)
{
	if (!_rect->IsActive())
		return false;
	
    if (_rect->Contains(_point))
    {
        _rect->IsCollided(true);
		
        return true;
    }

    return false;
}

bool Collision(AARect* _rect, AARect* _other)
{
	if (!_rect->IsActive() || !_other->IsActive())
		return false;

	D3DXVECTOR2 rectMin = _rect->GetMin();
	D3DXVECTOR2 rectMax = _rect->GetMax();
	D3DXVECTOR2 otherMin = _other->GetMin();
	D3DXVECTOR2 otherMax = _other->GetMax();
	
    bool separatingAxis =
        rectMax.x < otherMin.x ||
        rectMin.x > otherMax.x ||
        rectMax.y < otherMin.y ||
        rectMin.y > otherMax.y;

    if (!separatingAxis)
    {
		_rect->IsCollided(true);
		_other->IsCollided(true);
		
		return true;
    }
	
    return false;
}

bool Collision(AARect* _rect, Circle* _circle)
{
    if (!_rect->IsActive() || !_circle->IsActive())
        return false;
	
    float distSq = _rect->MinDistSq(_circle->GetCenter());

    if (distSq <= _circle->GetRadiusSq())
    {
		_rect->IsCollided(true);
		_circle->IsCollided(true);
		
		return true;
    }
	
    return false;
}

bool Collision(Circle* _circle, Collider* _other)
{
	if (!_circle->IsActive() || !_other->IsActive())
		return false;

	COLLIDER_TYPE typeB = _other->GetColliderType();
	
    switch (typeB)
    {
    case COLLIDER_TYPE::LINE:
        break;
    case COLLIDER_TYPE::AARECT:
		Collision(_circle, (AARect*)_other);
        break;
    case COLLIDER_TYPE::CIRCLE:
		Collision(_circle, (Circle*)_other);
        break;
    default:
        break;
    }
	
	
    return false;
}

bool Collision(Circle* _circle, const D3DXVECTOR2& _point)
{
    if (!_circle->IsActive())
        return false;

    if (_circle->Contains(_point))
    {
		_circle->IsCollided(true);
		
		return true;
    }
	
    return false;
}

bool Collision(Circle* _circle, Circle* _other)
{
	if (!_circle->IsActive() || !_other->IsActive())
		return false;
	
	D3DXVECTOR2 distCenter = _circle->GetCenter() - _other->GetCenter();
	
    float distSq = D3DXVec2LengthSq(&distCenter);
    float radiusSum = _circle->GetRadius() + _other->GetRadius();

    if (distSq <= radiusSum * radiusSum)
    {
		_circle->IsCollided(true);
		 _other->IsCollided(true);

        return true;
    }

    return false;
}
