#pragma once

constexpr unsigned int MAX_CHANNEL = 16;

using SoundDesc = struct tagSoundDesc
{
    FMOD::Sound* sound;
    FMOD::Channel* channel;

    tagSoundDesc() : sound(nullptr), channel(nullptr) {}
    ~tagSoundDesc() { sound->release(); }
};

class SoundMgr
{
public:
    SoundMgr();
    ~SoundMgr();

public:
    void Update();
    
    SoundDesc* AddSound(const string& _key, const string& _fileName, bool _isLoop = false);

    void Play(const string& _key, float _volume = 0.5f, bool _isBGM = false);
    void Stop(const string& _key);
    void Pause(const string& _key);
    void Resume(const string& _key);
    void SetVolume(const string& _key, float _volume);
    void SetPitch(const string& _key, float _pitch);
    void AllStop();
    
private:
    FMOD::System* m_system;
    FMOD_RESULT   m_result;
    
    unordered_map<string, SoundDesc*> m_soundMap;
    SoundDesc* m_bgm;

public:
    SoundDesc* GetNowBGM() { return m_bgm; }
};

