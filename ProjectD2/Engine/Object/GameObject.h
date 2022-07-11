#pragma once
class Game;
class Component;
class GameObject
{
public:
	GameObject(Game* _game);
	virtual ~GameObject();

	void Update();
	void UpdateWorld();
	void UpdateComponent();			// Component Update 용
	virtual void UpdateObject() = 0;// 상속받은 GameObject의 Update 용
	virtual void Render();

	void AddComponent(Component* _component);
	void RemoveComponent(Component* _component);
	
private:
	Game* m_game;
	GameObject* m_parent;
	OBJECT_TYPE  m_type;
	OBJECT_STATE m_state;
	
/* === === === === ===
*     Transform
* === === === === === */
protected:
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

/* === === === === ===
*     Component
* === === === === === */
private:
	vector<Component*> m_componentList;

/* === === === === ===
*  Getter / Setter
* === === === === === */
public:
	const Game* GetGame() const { return m_game; }
	
	const GameObject* GetParent() const              { return m_parent; }
	void              SetParent(GameObject* _parent) { m_parent = _parent; }

	OBJECT_TYPE GetType() const            { return m_type; }
	void        SetType(OBJECT_TYPE _type) { m_type = _type; }

	OBJECT_STATE GetState() const              { return m_state; }
	void         SetState(OBJECT_STATE _state) { m_state = _state; }
	bool         IsDead() const                { return m_state == OBJECT_STATE::DEAD; }

	const D3DXVECTOR3& GetPos() const                        { return m_pos; }
	void               SetPos(const D3DXVECTOR3& position)   { m_pos = position; m_recomputeWorld = true; }
	void               SetPos(float x, float y, float z = 0) { SetPos({ x, y, z }); }
	
	const D3DXVECTOR3& GetScale() const                        { return m_scale; }
	void               SetScale(const D3DXVECTOR3& scale)      { m_scale = scale; m_recomputeWorld = true; }
	void               SetScale(float x, float y, float z = 0) { SetScale({ x, y, z }); }
	
	float GetAngle() const      { return m_angle; }
	void  SetAngle(float angle) { m_angle = angle; m_recomputeWorld = true; }

	const D3DXVECTOR3& GetPivot() const                   { return m_pivot; }
	void               SetPivot(const D3DXVECTOR3& pivot) { m_pivot = pivot; m_recomputeWorld = true; }
	
	const D3DXMATRIX& GetWorld() const { return m_world; }
	const D3DXMATRIX& GetS() const     { return m_S; }
	const D3DXMATRIX& GetR() const     { return m_R; }
	const D3DXMATRIX& GetT() const     { return m_T; }
	
};
