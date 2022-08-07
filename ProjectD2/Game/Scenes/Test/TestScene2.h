#pragma once
class TestScene2 : public Scene
{
public:
    TestScene2(Game* _game);
    ~TestScene2() override;

    // Scene을(를) 통해 상속됨
    virtual void Init() override;
    virtual void Release() override;
    virtual void UpdateScene() override;
    virtual void RenderScene() override;
    virtual void SceneEnter() override;
    virtual void SceneExit() override;

private:
    class Image* titleImg;

    SoundDesc* intro;
    bool isIntroPlaying = false;
    bool isLoopPlaying = false;
};

