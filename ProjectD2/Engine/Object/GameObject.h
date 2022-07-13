#pragma once
class Game;
class Component;
class GameObject
{
public:
	GameObject(Game* _game, Scene* _scene, GameObject* _parent = nullptr);
	GameObject(Game* _game, Scene* _scene, OBJECT_TYPE _type, GameObject* _parent = nullptr);
	virtual ~GameObject();

	void Update();
	void UpdateWorld();
	void SetWorld();
	void UpdateComponent();			// Component Update 용
	void RenderComponent();			// Component Render 용
	virtual void UpdateObject() = 0;// 상속받은 GameObject의 Update 용
	virtual void Render();

	void AddComponent(Component* _component);
	void RemoveComponent(Component* _component);
	
private:
	Game* m_game;
	Scene* m_scene;
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
	void               SetPivot(const D3DXVECTOR3& pivot)      { m_pivot = pivot; m_recomputeWorld = true; }
	void               SetPivot(float x, float y, float z = 0) { SetPivot({ x, y, z }); }
	void               AddPivot(const D3DXVECTOR3& pivot)      { m_pivot += pivot; m_recomputeWorld = true; }
	void               AddPivot(float x, float y, float z = 0) { AddPivot({ x, y, z }); }
	
	const D3DXMATRIX& GetWorld() const { return m_world; }
	const D3DXMATRIX& GetS() const     { return m_S; }
	const D3DXMATRIX& GetR() const     { return m_R; }
	const D3DXMATRIX& GetT() const     { return m_T; }
	

/* === === === === ===
*  테스트용 VertexBuffer, IndexBuffer
* === === === === === */
#if _DEBUG
private:
	vector<VERTEXCOLOR> m_vertexList;
	vector<WORD>        m_indexList;
	void SetVertexData();
#endif // GameObject 위치 탐색용 Vertices
};
