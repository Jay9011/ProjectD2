#include "Framework.h"
#include "GameObject.h"

#include "Engine/Core/Core.h"
#include "Engine/Component/Component.h"

GameObject::GameObject(Scene* _scene, int _updateOrder, GameObject* _parent) :
	GameObject(_scene, OBJECT_TYPE::DEFAULT, _updateOrder, _parent)
{}

GameObject::GameObject(Scene* _scene, OBJECT_TYPE _type, int _updateOrder, GameObject* _parent) :
	Transform(_parent)
	, m_scene(_scene)
	, m_type(_type)
	, m_state(OBJECT_STATE::ACTIVE)
	, m_updateOrder(_updateOrder)
	, m_isUpdating(false)
	, m_isRendering(false)
{
	m_scene->AddObject(this, _type);

#if _DEBUG
	SetVertexData();
#endif // _DEBUG
}

GameObject::~GameObject()
{
	/*     Buffer Release     */
	m_vertexBuffer->Release();
	 m_indexBuffer->Release();
	
	/*     Component Release       */
	while (!m_componentList.empty())
	{
		m_componentList.back()->Delete();
		//delete m_componentList.back();
	}
	
	/*     GameObject Release        */
	m_scene->DeleteObject(this);
}

void GameObject::Update()
{
	if (m_isUpdating)
		return;
	
	m_isUpdating = true;

	if (m_state == OBJECT_STATE::ACTIVE)
	{
		//UpdateWorld();
		
		UpdateObject();
		UpdateComponent();
		UpdateComponentWorldTransform();
		
		UpdateWorld();
	}
	
	m_isUpdating = false;
}

void GameObject::UpdateComponent()
{
	for (auto& component : m_componentList)
	{
		component->Update();
	}
}

void GameObject::UpdateComponentWorldTransform()
{
	for (auto& component : m_componentList)
	{
		component->OnUpdateWorldTransform();
	}
}

void GameObject::FinalUpdate()
{
	if (m_isUpdating)
		return;

	m_isUpdating = true;

	if (m_state == OBJECT_STATE::ACTIVE)
	{
		m_direction = Math::NearZeroValue(GetPos() - m_beforePos);
		
		FinalUpdateComponent();
		FinalUpdateObject();
		
		m_beforePos = GetPos();
	}
	
	m_isUpdating = false;
}

void GameObject::FinalUpdateComponent()
{
	for (auto& component : m_componentList)
	{
		component->FinalUpdate();
	}

}

void GameObject::Render()
{
	if (m_isRendering)
		return;

	m_isRendering = true;

	SetWorld();
	RenderComponent();
	RenderObject();
	
	m_isRendering = false;
}

void GameObject::RenderComponent()
{
	for (auto& component : m_componentList)
	{
		if (!component->AutoRendering())
			return;
		
		component->Render();
	}
}

void GameObject::RenderObject()
{
	if (Game::IsDbgRendering())
	{
		DEVICE->SetFVF(VERTEXCOLOR::FVF);
		DEVICE->SetStreamSource(0, m_vertexBuffer, 0, sizeof(VERTEXCOLOR));
		DEVICE->SetIndices(m_indexBuffer);
		DEVICE->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, m_vertexCount, 0, m_indexCount / 2);

		//DrawDirectionVertex();
	}
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

	m_vertexCount = (UINT)vertexList.size();
	m_indexCount  = (UINT)indexList.size();
	UINT vertexSize = m_vertexCount * sizeof(VERTEXCOLOR);
	UINT indexSize  = m_indexCount * sizeof(WORD);
	
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

void GameObject::DrawDirectionVertex()
{
	vector<VERTEXCOLOR> vertexList;
	vector<WORD>        indexList;

	D3DCOLOR color = 0xFFFFB03A;

	D3DXVECTOR3 scale = GetScale();
	
	// Direction Vertex
	vertexList.push_back(VERTEXCOLOR(0, 0, color, -1.f)); // start
	vertexList.push_back(VERTEXCOLOR(m_direction.x * abs(scale.x) * 30, m_direction.y * abs(scale.y) * 30, color, -1.f)); // end
	
	indexList.push_back(0);
	indexList.push_back(1);

	
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);

	D3DXMATRIX P;
	D3DXMATRIX IP;
	D3DXVECTOR3 pivot = GetPivot();
	D3DXMatrixTranslation(&P, pivot.x, pivot.y, pivot.z);
	D3DXMatrixInverse(&IP, nullptr, &P);

	world = IP * m_T * P;
	
	DEVICE->SetTransform(D3DTS_WORLD, &world);
	DEVICE->SetFVF(VERTEXCOLOR::FVF);
	DEVICE->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, vertexList.size(), indexList.size() / 2, indexList.data(), D3DFMT_INDEX16, vertexList.data(), sizeof(VERTEXCOLOR));

	SetWorld();
}
