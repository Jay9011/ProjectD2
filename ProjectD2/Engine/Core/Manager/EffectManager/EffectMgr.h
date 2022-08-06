#pragma once
class Effect;
class EffectMgr
{
public:
    EffectMgr();
    ~EffectMgr();

    void Update();
    void FinalUpdate();
    void Render();

    void Add(const string& _key, const wstring& _path, UINT _poolCount, int _maxFrameX, int _maxFrameY, bool _isAdditive = true, float _speed = 0.1f);
    void Play(const string& _key, D3DXVECTOR2 _pos, D3DXCOLOR _color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

private:
    unordered_map<string, vector<Effect*>> m_effectMap;
};

