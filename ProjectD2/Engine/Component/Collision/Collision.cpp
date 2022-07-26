#include "Framework.h"
#include "Collision.h"

bool TestSide(float _start, float _end, float _negd, const D3DXVECTOR2& _norm, vector<std::pair<float, D3DXVECTOR2>>& _out);

CollisionMgr::CollisionMgr(Game* _game) :
	m_game(_game)
{
}

CollisionMgr::~CollisionMgr() = default;

bool CollisionMgr::CheckCollision(Collider* _chkCollider, OBJECT_TYPE _type, OUT vector<std::pair<Collider*, Collider*>>& _vecCollList)
{
    bool result = false;
	
	if (_chkCollider == nullptr || !_chkCollider->IsActive())
		return result;
	
    _vecCollList.clear();
	
	D3DXVECTOR2 chkMin = _chkCollider->GetMin();
    D3DXVECTOR2 chkMax = _chkCollider->GetMax();
	
    for (auto& collider : m_colliders[(UINT)_type])
    {
        if (_chkCollider == collider)
            continue;
		
        if (chkMin.x > collider->GetMax().x)
            continue;
		
        if (chkMax.x < collider->GetMin().x)
            break;
        
        if (Collision(_chkCollider, collider))
        {
			_vecCollList.push_back(std::make_pair(_chkCollider, collider));
            result = true;
        }
    }

    return result;
}

bool CollisionMgr::CheckCollision(OBJECT_TYPE _typeA, OBJECT_TYPE _typeB, OUT vector<std::pair<Collider*, Collider*>>& _vecCollList)
{
	bool result = false;

	_vecCollList.clear();
	
    for (size_t i = 0; i < m_colliders[(UINT)_typeA].size(); i++)
    {
        Collider* colliderA = m_colliders[(UINT)_typeA][i];
		D3DXVECTOR2 aMin = colliderA->GetMin();
		D3DXVECTOR2 aMax = colliderA->GetMax();
		
        for (size_t j = 0; j < m_colliders[(UINT)_typeB].size(); j++)
        {
            Collider* colliderB = m_colliders[(UINT)_typeB][j];
			
			if (colliderA == colliderB)
				continue;
			
			if (aMin.x > colliderB->GetMax().x)
				continue;
			
            if (aMax.x < colliderB->GetMin().x)
                break;
			
            if (Collision(colliderA, colliderB))
            {
				_vecCollList.push_back(std::make_pair(colliderA, colliderB));
				result = true;
            }
        }
    }
	
    return result;
}

void CollisionMgr::Update()
{

}

// 반드시! 마지막에 위치에 따라 정렬하게 해야 CheckCollision이 정상 동작함
void CollisionMgr::FinalUpdate()
{
    for (auto& collisionType : m_colliders)
    {
        std::sort(collisionType.begin(), collisionType.end(),
            [](Collider* a, Collider* b)
            {
                return a->GetMin().x < b->GetMin().x;
            }
        );
    }
}

void CollisionMgr::AddCollider(OBJECT_TYPE _type, Collider* _collider)
{
	m_colliders[(UINT)_type].emplace_back(_collider);
}

void CollisionMgr::RemoveCollider(OBJECT_TYPE _type, Collider* _collider)
{
	auto iter = std::find(m_colliders[(UINT)_type].begin(), m_colliders[(UINT)_type].end(), _collider);
    if (iter != m_colliders[(UINT)_type].end())
    {
        std::iter_swap(iter, m_colliders[(UINT)_type].end() - 1);
		m_colliders[(UINT)_type].pop_back();
    }
}

void CollisionMgr::RemoveColliderList(OBJECT_TYPE _type)
{
	m_colliders[(UINT)_type].clear();
}

void CollisionMgr::RemoveAllColliders()
{
    for (auto& type : m_colliders)
    {
		type.clear();
    }
}

bool Collision(Collider* _a, Collider* _b)
{
    if (!_a->IsActive() || !_b->IsActive())
        return false;

    COLLIDER_TYPE typeA = _a->GetColliderType();

    bool result = false;
    switch (typeA)
    {
    case COLLIDER_TYPE::LINE:
        result = Collision((Line*)_a, _b);
        break;
    case COLLIDER_TYPE::AARECT:
        result = Collision((AARect*)_a, _b);
        break;
    case COLLIDER_TYPE::CIRCLE:
        result = Collision((Circle*)_a, _b);
        break;
    default:
        break;
    }

    return result;
}

bool Collision(const D3DXVECTOR2& _point, Collider* _other)
{
    if (!_other->IsActive())
        return false;
	
	COLLIDER_TYPE typeB = _other->GetColliderType();
	
    bool result = false;
    switch (typeB)
    {
    case COLLIDER_TYPE::LINE:
		result = Collision((Line*)_other, _point);
        break;
    case COLLIDER_TYPE::AARECT:
		result = Collision((AARect*)_other, _point);
        break;
    case COLLIDER_TYPE::CIRCLE:
		result = Collision((Circle*)_other, _point);
        break;
    default:
        break;
    }

	
    return result;
}

bool Collision(AARect* _rect, Collider* _other)
{
    if (!_rect->IsActive() || !_other->IsActive())
        return false;
	
	COLLIDER_TYPE typeB = _other->GetColliderType();
	
    bool result = false;
    switch (typeB)
    {
    case COLLIDER_TYPE::LINE:
		result = Collision(_rect, (Line*)_other);
        break;
    case COLLIDER_TYPE::AARECT:
		result = Collision(_rect, (AARect*)_other);
        break;
    case COLLIDER_TYPE::CIRCLE:
		result = Collision(_rect, (Circle*)_other);
        break;
    default:
        break;
    }
	
    return result;
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
		
        _rect->OnCollision(_other);
		_other->OnCollision(_rect);
		
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

		_rect->OnCollision(_circle);
		_circle->OnCollision(_rect);
		
		return true;
    }
	
    return false;
}

bool Collision(AARect* _rect, Line* _line)
{
	if (!_rect->IsActive() || !_line->IsActive())
		return false;
	
    vector<std::pair<float, D3DXVECTOR2>> tValues;

	D3DXVECTOR2 lineStart = _line->GetStart();
	D3DXVECTOR2 lineEnd   = _line->GetEnd();
	D3DXVECTOR2 rectMin   = _rect->GetMin();
	D3DXVECTOR2 rectMax   = _rect->GetMax();
	
	
	TestSide(lineStart.x, lineEnd.x, rectMin.x, {-1,  0}, tValues);
	TestSide(lineStart.x, lineEnd.x, rectMax.x, { 1,  0}, tValues);
	TestSide(lineStart.y, lineEnd.y, rectMin.y, { 0, -1}, tValues);
	TestSide(lineStart.y, lineEnd.y, rectMax.y, { 0,  1}, tValues);
	
	std::sort(tValues.begin(), tValues.end(), 
        [](const std::pair<float, D3DXVECTOR2>& a, const std::pair<float, D3DXVECTOR2>& b) 
        { return a.first < b.first; }
    );

    D3DXVECTOR2 point;
    for (auto& t : tValues)
    {
        point = _line->PointOnLine(t.first);
        if (_rect->Contains(point))
        {
            // 충돌지점 t.first
            // 노멀벡터 t.second

            _line->IsCollided(true);
            _rect->IsCollided(true);

			_line->OnCollision(_rect);
			_rect->OnCollision(_line);

            return true;
        }
    }
    
    return false;
}

bool Collision(Circle* _circle, Collider* _other)
{
	if (!_circle->IsActive() || !_other->IsActive())
		return false;

	COLLIDER_TYPE typeB = _other->GetColliderType();
	
    bool result = false;
    switch (typeB)
    {
    case COLLIDER_TYPE::LINE:
		result = Collision(_circle, (Line*)_other);
        break;
    case COLLIDER_TYPE::AARECT:
		result = Collision((AARect*)_other, _circle);
        break;
    case COLLIDER_TYPE::CIRCLE:
		result = Collision(_circle, (Circle*)_other);
        break;
    default:
        break;
    }
	
	
    return result;
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

		_circle->OnCollision(_other);
		_other->OnCollision(_circle);

        return true;
    }

    return false;
}

bool Collision(Circle* _circle, Line* _line)
{
    if(!_circle->IsActive() || !_line->IsActive())
		return false;
	
	D3DXVECTOR2 x = _line->GetStart() - _circle->GetCenter();
	D3DXVECTOR2 y = _line->GetEnd() - _line->GetStart();
    float a = D3DXVec2Dot(&y, &y);
	float b = 2.0f * D3DXVec2Dot(&x, &y);
	float c = D3DXVec2Dot(&x, &x) - _circle->GetRadiusSq();
	// 판별식
	float discriminant = b * b - 4 * a * c;
    if (discriminant < 0.0f)
    {
        return false;
    }
    else
    {
        discriminant = sqrt(discriminant);
		
		float t1 = (-b - discriminant) / (2.0f * a);
		float t2 = (-b + discriminant) / (2.0f * a);
		
        if (t1 >= 0.0f && t1 <= 1.0f)
        {
			_circle->IsCollided(true);
			_line->IsCollided(true);

			_circle->OnCollision(_line);
            _line->OnCollision(_circle);
			
			// 충돌지점 t = t1
            return true;
        }
        else if (t2 >= 0.0f && t2 <= 1.0f)
        {
			_circle->IsCollided(true);
			_line->IsCollided(true);

			_circle->OnCollision(_line);
			_line->OnCollision(_circle);

			// 충돌지점 t = t2
			return true;
        }
        else
        {
            return false;
        }
    }
    
    return false;
}

bool Collision(Line* _line, Collider* _other)
{
    if (!_line->IsActive() || !_other->IsActive())
        return false;

    COLLIDER_TYPE typeB = _other->GetColliderType();

    bool result = false;
    switch (typeB)
    {
    case COLLIDER_TYPE::LINE:
        result = Collision(_line, (Line*)_other);
        break;
    case COLLIDER_TYPE::AARECT:
        result = Collision((AARect*)_other, _line);
        break;
    case COLLIDER_TYPE::CIRCLE:
        result = Collision((Circle*)_other, _line);
        break;
    default:
        break;
    }
    
    return result;
}

bool Collision(Line* _line, const D3DXVECTOR2& _point)
{
    if(!_line->IsActive())
		return false;

    if (_line->Contains(_point))
    {
		_line->IsCollided(true);
		
		return true;
    }
	
    return false;
}

bool Collision(Line* _line, Line* _other)
{
    if (!_line->IsActive() && !_other->IsActive())
        return false;
	
    if (Math::NearZero(_line->MinDistSq(_other), 1.0))
    {
        _line->IsCollided(true);
		_other->IsCollided(true);
		
		_line->OnCollision(_other);
		_other->OnCollision(_line);

        return true;
    }
    
    return false;
}

FRECT GetCollisionRect(AARect* _rect1, AARect* _rect2)
{
	float left   = Math::Max(_rect1->GetMin().x, _rect2->GetMin().x);
	float top    = Math::Max(_rect1->GetMin().y, _rect2->GetMin().y);
	float right  = Math::Min(_rect1->GetMax().x, _rect2->GetMax().x);
	float bottom = Math::Min(_rect1->GetMax().y, _rect2->GetMax().y);
	
    return FRECT(left, top, right, bottom);
}



bool TestSide(float _start, float _end, float _negd, const D3DXVECTOR2& _norm, vector<std::pair<float, D3DXVECTOR2>>& _out)
{
    float denom = _end - _start;
    if (Math::NearZero(denom))
    {
        return false;
    }
    else
    {
        float numer = -_start + _negd;
        float t = numer / denom;
		
        if (t >= 0.0f && t <= 1.0f)
        {
            _out.emplace_back(t, _norm);
            return true;
        }
        else
        {
            return false;
        }
    }
}
