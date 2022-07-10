#pragma once
class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void Init() = 0;
	virtual void Release() = 0;
	
	virtual void Update() = 0;
	virtual void FinalUpdate() = 0;
	virtual void Render() = 0;

	void          SetName(const string& _name) { m_SceneName = _name; }
	const string& GetName() { return m_SceneName; }

private:
	string m_SceneName;
	
};
