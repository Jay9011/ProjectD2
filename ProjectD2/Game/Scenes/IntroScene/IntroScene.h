#pragma once
class IntroScene : public Scene
{
public:
    IntroScene(Game* _game);
    ~IntroScene() override;

    // Scene��(��) ���� ��ӵ�
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

