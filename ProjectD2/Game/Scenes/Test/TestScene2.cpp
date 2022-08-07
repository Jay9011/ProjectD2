#include "Framework.h"
#include "TestScene2.h"

#include "Game/Objects/UI/Image.h"

TestScene2::TestScene2(Game* _game) :
    Scene(_game)
{
    titleImg = new Image(TEXTURE->Add(L"Title\\Title.jpg", (UINT)WIN_WIDTH, (UINT)WIN_HEIGHT), this);
    titleImg->SetPos(WIN_CENTER_X, WIN_CENTER_Y);
}

TestScene2::~TestScene2()
{
}

void TestScene2::Init()
{
    intro = SOUND->AddSound("Title_Intro", "Title_Intro.ogg");
    SOUND->AddSound("Title_Loop", "Title_Loop.ogg", true);
}

void TestScene2::Release()
{
}

void TestScene2::UpdateScene()
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
}

void TestScene2::RenderScene()
{
}

void TestScene2::SceneEnter()
{
    SOUND->Play("Title_Intro", 0.3f, true);
}

void TestScene2::SceneExit()
{
    SOUND->AllStop();
}
