#pragma once
class IntroScene : public Scene
{
public:
    IntroScene(Game* _game);
    ~IntroScene() override;

    // Scene을(를) 통해 상속됨
    virtual void Init() override;
    virtual void Release() override;
    virtual void UpdateScene() override;
    virtual void RenderScene() override;
    virtual void SceneEnter() override;
    virtual void SceneExit() override;

private:
    class Image* titleImg;
    class Image* TextImg;
    bool  isTextRendering = false;
    float textRenderTimer = 0.0f;

    SoundDesc* intro;
    bool isIntroPlaying = false;
    bool isLoopPlaying = false;
};

