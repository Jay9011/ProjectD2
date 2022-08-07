#pragma once
class LoadingScene : public Scene
{
public:
    LoadingScene(Game* _game);
    ~LoadingScene() override;

    // Scene을(를) 통해 상속됨
    virtual void Init() override;
    virtual void Release() override;
    virtual void UpdateScene() override;
    virtual void RenderScene() override;
    virtual void SceneEnter() override;
    virtual void SceneExit() override;

    void SetNextScene(const string& _nextScene);
    
private:
    class Image* loadingImg;
    string nextScene;
};

