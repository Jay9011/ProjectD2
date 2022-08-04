#pragma once
#include "Engine/Object/Transform.h"

enum class OBJECT_STATE
{
	DEAD,
	ACTIVE,
	INACTIVE,
    HIDDEN
};

class Component;
class GameObject : public Transform
{
public:
	GameObject(Scene* _scene, int _updateOrder = 100, GameObject* _parent = nullptr);
	GameObject(Scene* _scene, OBJECT_TYPE _type, int _updateOrder = 100, GameObject* _parent = nullptr);
	virtual ~GameObject() override;

	void Update();
	void UpdateComponent();					// Component Update ��
	void UpdateComponentWorldTransform();	// Component World Transform Update ��
	void FinalUpdate();
	void FinalUpdateComponent();			// Component FinalUpdate ��
	void Render();
	void RenderComponent();					// Component Render ��
	
	virtual void UpdateObject() = 0;		// ��ӹ��� GameObject�� Update ��
	virtual void PostUpdateObject() {};
	virtual void FinalUpdateObject() {};
	virtual void RenderObject();

	void AddComponent(Component* _component);
	void RemoveComponent(Component* _component);
	
private:
	Scene* m_scene;
	OBJECT_TYPE  m_type;
	OBJECT_STATE m_state;
	int  m_updateOrder;
	bool m_isUpdating;
	bool m_isRendering;

	D3DXVECTOR2 m_beforePos;
	D3DXVECTOR2 m_direction;
	D3DXVECTOR2 m_directionPower;
	
/* === === === === ===
*     Component
* === === === === === */
private:
	vector<Component*> m_componentList;

/* === === === === ===
*  Getter / Setter
* === === === === === */
public:
	const GameObject* GetParent() const override { return (GameObject*)m_parent; }
	
	Scene* GetScene() const { return m_scene; }

	OBJECT_TYPE GetType() const            { return m_type; }
	void        SetType(OBJECT_TYPE _type) { m_type = _type; }

	OBJECT_STATE GetState() const              { return m_state; }
	void         SetState(OBJECT_STATE _state) { m_state = _state; }
	bool         IsDead() const                { return m_state == OBJECT_STATE::DEAD; }

	int GetOrder() const { return m_updateOrder; }

	const D3DXVECTOR2& GetDirection() const { return m_direction; }
	const D3DXVECTOR2& GetBeforePos() const { return m_beforePos; }

/* === === === === ===
*  �׽�Ʈ�� VertexBuffer, IndexBuffer
* === === === === === */
private:
	UINT m_vertexCount;
	UINT m_indexCount;
	LPDIRECT3DVERTEXBUFFER9 m_vertexBuffer;
	LPDIRECT3DINDEXBUFFER9  m_indexBuffer;
	bool drawDirection = false;
	void SetVertexData();
	void DrawDirectionVertex();
public:
	void SetDrawDirection(bool _bool) { drawDirection = _bool; }
};
