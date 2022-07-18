#include "Framework.h"
#include "GameObject.h"

#include "Engine/Core/Core.h"
#include "Engine/Component/Component.h"

GameObject::GameObject(Scene* _scene, GameObject* _parent) :
	GameObject(_scene, OBJECT_TYPE::DEFAULT, _parent)
{}

GameObject::GameObject(Scene* _scene, OBJECT_TYPE _type, GameObject* _parent) :
	Transform(_parent)
	, m_scene(_scene)
	, m_type(_type)
	, m_state(OBJECT_STATE::ACTIVE)
{
	D3DXMatrixIdentity(&m_S);
	D3DXMatrixIdentity(&m_R);
	D3DXMatrixIdentity(&m_T);
	D3DXMatrixIdentity(&m_world);

	m_scene->AddObject(this, _type);

#if _DEBUG
	SetVertexData();
#endif // _DEBUG

}

GameObject::~GameObject()
{
	SAFE_DELETE_VEC(m_componentList);
	m_scene->DeleteObject(this);
}

void GameObject::Update()
{
	if (m_state == OBJECT_STATE::ACTIVE)
	{
		UpdateWorld();

		UpdateComponent();
		UpdateObject();
		
		UpdateWorld();
	}
}

void GameObject::UpdateObject()
{
}

void GameObject::UpdateComponent()
{
	for (auto& component : m_componentList)
	{
		component->Update();
	}
}

void GameObject::Render()
{
	SetWorld();
	for (auto& component : m_componentList)
	{
		component->Render();
	}
#if _DEBUG
	DEVICE->SetFVF(VERTEXCOLOR::FVF);
	DEVICE->SetStreamSource(0, m_vertexBuffer, 0, sizeof(VERTEXCOLOR));
	DEVICE->SetIndices(m_indexBuffer);
	DEVICE->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, m_vertexCount, 0, m_indexCount / 2);
#endif // 테스트 렌더링 (Object 위치 표시)
}

void GameObject::AddComponent(Component* _component)
{
	int componentOrder = _component->GetUpdateOrder();
	auto iter = m_componentList.begin();
	for (; iter != m_componentList.end(); iter++)
	{
		if (componentOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}
	
	m_componentList.insert(iter, _component);
}

void GameObject::RemoveComponent(Component* _component)
{
	auto iter = std::find(m_componentList.begin(), m_componentList.end(), _component);
	if (iter != m_componentList.end())
	{
		m_componentList.erase(iter);
	}
}

void GameObject::SetVertexData()
{
	vector<VERTEXCOLOR> vertexList;
	vector<WORD>        indexList;
	
	float angle = PI * 2.f * 0.083f;

	D3DCOLOR color = 0xFF3AB0FF;
	
	for (int i = 0; i < 13; i++)
	{
		D3DXVECTOR3 pos;
		pos.x =  cos(angle * (float)i) * 8.f;
		pos.y = -sin(angle * (float)i) * 8.f;
		
		vertexList.push_back(VERTEXCOLOR(pos.x, pos.y, color, -1.f));
		indexList.push_back((WORD)i);
		indexList.push_back((WORD)i + 1);
	}
	indexList.pop_back();
	indexList.push_back(0);
	
	indexList.push_back((WORD)vertexList.size());
	indexList.push_back((WORD)vertexList.size() + 1);
	vertexList.push_back(VERTEXCOLOR(0, -12, color, -1.f));
	vertexList.push_back(VERTEXCOLOR(0, +12, color, -1.f));
	
	indexList.push_back((WORD)vertexList.size());
	indexList.push_back((WORD)vertexList.size() + 1);
	vertexList.push_back(VERTEXCOLOR(-12, 0, color, -1.f));
	vertexList.push_back(VERTEXCOLOR(+12, 0, color, -1.f));

	m_vertexCount = vertexList.size();
	m_indexCount = indexList.size();
	UINT vertexSize = m_vertexCount * sizeof(VERTEXCOLOR);
	UINT indexSize = m_indexCount * sizeof(WORD);
	
	DEVICE->CreateVertexBuffer(vertexSize, 0, VERTEXCOLOR::FVF, D3DPOOL_DEFAULT, &m_vertexBuffer, nullptr);

	VOID* pVoid;
	m_vertexBuffer->Lock(0, vertexSize, &pVoid, 0);
	memcpy(pVoid, vertexList.data(), vertexSize);
	m_vertexBuffer->Unlock();
	
	DEVICE->CreateIndexBuffer(indexSize, 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_indexBuffer, nullptr);
	
	m_indexBuffer->Lock(0, indexSize, &pVoid, 0);
	memcpy(pVoid, indexList.data(), indexSize);
	m_indexBuffer->Unlock();
}
