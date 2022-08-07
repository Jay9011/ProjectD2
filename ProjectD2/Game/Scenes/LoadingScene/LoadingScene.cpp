#include "Framework.h"
#include "LoadingScene.h"

#include "Game/Objects/UI/Image.h"

LoadingScene::LoadingScene(Game* _game) : 
    Scene(_game)
{
    loadingImg = new Image(TEXTURE->Add(L"Title\\Loading.png", (UINT)WIN_WIDTH, (UINT)WIN_HEIGHT), this);
    loadingImg->SetPos(WIN_CENTER_X, WIN_CENTER_Y);
}

LoadingScene::~LoadingScene()
{
}

void LoadingScene::Init()
{
}

void LoadingScene::Release()
{
}

void LoadingScene::UpdateScene()
{
    SCENE->ChangeScene(nextScene);
}

void LoadingScene::RenderScene()
{
}

void LoadingScene::SceneEnter()
{
}

void LoadingScene::SceneExit()
{
}

void LoadingScene::SetNextScene(const string& _nextScene)
{
    nextScene = _nextScene;
}
