#pragma once
class Transform
{
public:
	Transform(Transform* _parent = nullptr);
	virtual ~Transform();

	void UpdateWorld();
	void SetWorld();

private:
	void SumParentValues();

protected:
	Transform* m_parent;
	
	D3DXMATRIX  m_world;
	D3DXMATRIX  m_S;
	D3DXMATRIX  m_R;
	D3DXMATRIX  m_T;

private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_scale;
	float       m_angle;
	
	D3DXVECTOR3 m_pivot;

	bool m_recomputeWorld;

	D3DXVECTOR3 m_sumParentsPos;
	D3DXVECTOR3 m_sumParentsScale;
	float       m_sumParentsAngle;
	D3DXVECTOR3 m_sumParentsPivot;

/* === === === === ===
*  Getter / Setter
* === === === === === */
public:
	virtual const Transform* GetParent() const             { return m_parent; }
	void                     SetParent(Transform* _parent) { m_parent = _parent; }
	
	const D3DXVECTOR3& GetPos() const                        { return m_pos; }
	void               SetPos(const D3DXVECTOR3& position)   { m_pos = position; m_recomputeWorld = true; }
	void               SetPos(float x, float y, float z = 0) { SetPos({ x, y, z }); }
	void               AddPos(const D3DXVECTOR3& position)   { m_pos += position; m_recomputeWorld = true; }
	void               AddPos(float x, float y, float z = 0) { AddPos({ x, y, z }); }

	const D3DXVECTOR3& GetScale() const                        { return m_scale; }
	void               SetScale(const D3DXVECTOR3& scale)      { m_scale = scale; m_recomputeWorld = true; }
	void               SetScale(float x, float y, float z = 0) { SetScale({ x, y, z }); }
	void               AddScale(const D3DXVECTOR3& scale)      { m_scale += scale; m_recomputeWorld = true; }
	void               AddScale(float x, float y, float z = 0) { AddScale({ x, y, z }); }

	float GetAngle() const      { return m_angle; }
	void  SetAngle(float angle) { m_angle = angle; m_recomputeWorld = true; }
	void  AddAngle(float angle) { m_angle += angle; m_recomputeWorld = true; }

	const D3DXVECTOR3& GetPivot() const                        { return m_pivot; }
	//void               SetPivot(const D3DXVECTOR3& pivot)      { m_pivot = pivot; m_recomputeWorld = true; }
	//void               SetPivot(float x, float y, float z = 0) { SetPivot({ x, y, z }); }
	//void               AddPivot(const D3DXVECTOR3& pivot)      { m_pivot += pivot; m_recomputeWorld = true; }
	//void               AddPivot(float x, float y, float z = 0) { AddPivot({ x, y, z }); }

	const D3DXMATRIX& GetWorld() const { return m_world; }
	const D3DXMATRIX& GetS() const     { return m_S; }
	const D3DXMATRIX& GetR() const     { return m_R; }
	const D3DXMATRIX& GetT() const     { return m_T; }

	D3DXVECTOR3 GetWorldPos()   { return m_sumParentsPos; }
	D3DXVECTOR3 GetWorldScale() { return D3DXVECTOR3(m_sumParentsScale.x * m_scale.x, m_sumParentsScale.y * m_scale.y, m_sumParentsScale.z * m_scale.z); }
	float       GetWorldAngle() { return m_sumParentsAngle + m_angle; }
	D3DXVECTOR3 GetWorldPivot() { return m_sumParentsPivot + m_pivot; }

	bool IsRecomputeWorld() const              { return m_recomputeWorld; }
	bool IsRecomputeWorld(bool recomputeWorld) { m_recomputeWorld = recomputeWorld; return m_recomputeWorld; }
};

