#include "Framework.h"
#include "SoundMgr.h"

SoundMgr::SoundMgr()
{
    m_result = FMOD::System_Create(&m_system);
    if (m_result != FMOD_OK)
    {
        wstring error = L"Failed to Get Sound System";
        MessageBox(nullptr, error.c_str(), L"Error", MB_OK);
        return;
    }
    
    m_result = m_system->init(MAX_CHANNEL, FMOD_INIT_NORMAL, nullptr);
    if (m_result != FMOD_OK)
    {
        wstring error = L"Failed to Init System";
        MessageBox(nullptr, error.c_str(), L"Error", MB_OK);
        return;
    }
}

SoundMgr::~SoundMgr()
{
    for (auto& s : m_soundMap)
    {
        delete s.second;
    }
    m_soundMap.clear();
    m_system->close();
    m_system->release();
}

void SoundMgr::Update()
{
    m_system->update();
}

SoundDesc* SoundMgr::AddSound(const string& _key, const string& _fileName, bool _isLoop)
{
    SoundDesc* soundDesc = new SoundDesc;

    wstring wPath = PATH->GetContentPath();
    wPath += L"Sounds\\";
    string path;
    path.assign(wPath.begin(), wPath.end());
    path += _fileName;
    wPath.assign(path.begin(), path.end());
    
    if (_isLoop)
    {
        m_result = m_system->createStream(path.c_str(), FMOD_LOOP_NORMAL, nullptr, &soundDesc->sound);
    }
    else
    {
        m_result = m_system->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &soundDesc->sound);
    }

    if (m_result != FMOD_OK)
    {
        wstring error = L"Failed to load Sound : " + wPath;
        MessageBox(nullptr, error.c_str(), L"Error", MB_OK);
        return nullptr;
    }
    
    m_soundMap.insert(make_pair(_key, soundDesc));

    return soundDesc;
}

void SoundMgr::Play(const string& _key, float _volume, bool _isBGM)
{
    auto iter = m_soundMap.find(_key);
    if (iter == m_soundMap.end())
    {
        return;
    }

    SoundDesc* soundDesc = iter->second;
    
    m_system->playSound(soundDesc->sound, nullptr, false, &soundDesc->channel);
    soundDesc->channel->setVolume(_volume);
    
    if (_isBGM)
    {
        m_bgm = soundDesc;
        m_bgm->channel->setPriority(0);
    }
}

void SoundMgr::Stop(const string& _key)
{
    auto iter = m_soundMap.find(_key);
    if (iter == m_soundMap.end())
    {
        return;
    }

    SoundDesc* soundDesc = iter->second;
    soundDesc->channel->stop();
}

void SoundMgr::Pause(const string& _key)
{
    auto iter = m_soundMap.find(_key);
    if (iter == m_soundMap.end())
    {
        return;
    }

    SoundDesc* soundDesc = iter->second;
    soundDesc->channel->setPaused(true);
}

void SoundMgr::Resume(const string& _key)
{
    auto iter = m_soundMap.find(_key);
    if (iter == m_soundMap.end())
    {
        return;
    }

    SoundDesc* soundDesc = iter->second;
    soundDesc->channel->setPaused(false);
}

void SoundMgr::SetVolume(const string& _key, float _volume)
{
    auto iter = m_soundMap.find(_key);
    if (iter == m_soundMap.end())
    {
        return;
    }

    SoundDesc* soundDesc = iter->second;
    soundDesc->channel->setVolume(_volume);
}

void SoundMgr::SetPitch(const string& _key, float _pitch)
{
    auto iter = m_soundMap.find(_key);
    if (iter == m_soundMap.end())
    {
        return;
    }

    SoundDesc* soundDesc = iter->second;
    soundDesc->channel->setPitch(_pitch);
}

void SoundMgr::AllStop()
{
    for (auto& s : m_soundMap)
    {
        s.second->channel->stop();
    }
}
