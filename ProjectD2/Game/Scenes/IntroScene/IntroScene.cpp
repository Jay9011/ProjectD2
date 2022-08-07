#include "Framework.h"
#include "IntroScene.h"

#include "Engine/Resource/Shader.h"

#include "Game/Objects/UI/Image.h"

IntroScene::IntroScene(Game* _game) :
    Scene(_game)
{
    titleImg = new Image(TEXTURE->Add(L"Title\\Title.jpg", (UINT)WIN_WIDTH, (UINT)WIN_HEIGHT), this);
    titleImg->SetPos(WIN_CENTER_X, WIN_CENTER_Y);

    TextImg = new Image(TEXTURE->Add(L"Title\\Text.png"), this);
    TextImg->SetPos(WIN_CENTER_X, WIN_HEIGHT - 100.0f);
    TextImg->SetShader(SHADER(L"AlphaShader"));
}

IntroScene::~IntroScene()
{
}

void IntroScene::Init()
{
    intro = SOUND->AddSound("Title_Intro", "Title_Intro.ogg");
    SOUND->AddSound("Title_Loop", "Title_Loop.ogg", true);
}

void IntroScene::Release()
{
}

void IntroScene::UpdateScene()
{
    intro->channel->isPlaying(&isIntroPlaying);
    if (!isIntroPlaying && !isLoopPlaying)
    {
        SOUND->Play("Title_Loop", 0.3f, true);
        isLoopPlaying = true;
    }
    
    if (KEYDOWN(VK_RETURN))
    {
        GetGame()->NextScene("Test");
    }

    textRenderTimer += fDT;
    if ((isTextRendering && textRenderTimer >= 1.0f)
        || (!isTextRendering && textRenderTimer >= 0.5f))
    {
        textRenderTimer = 0.0f;
        isTextRendering = !isTextRendering;
    }

    TextImg->SetRendering(isTextRendering);
}

void IntroScene::RenderScene()
{

}

void IntroScene::SceneEnter()
{
    SOUND->Play("Title_Intro", 0.3f, true);
}

void IntroScene::SceneExit()
{
    SOUND->AllStop();
}
