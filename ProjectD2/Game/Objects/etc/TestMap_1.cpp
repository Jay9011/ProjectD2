#include "Framework.h"
#include "TestMap_1.h"

#include "Engine/Object/GameObject.h"
#include "Engine/Resource/Shader.h"
#include "Engine/Component/Component.h"
#include "Engine/Component/Animator/Animator.h"
#include "Engine/Component/Collision/Colliders/AARect.h"
#include "Engine/Object/Effect.h"

#include "Game/Objects/Background/Background.h"
#include "Game/Objects/Platforms/PlatformRect.h"
#include "Game/Objects/Monster/MonsterFactory.h"
#include "Game/Objects/Interactive/ScreenButton.h"
#include "Game/Objects/Interactive/Door.h"
#include "Game/Objects/UI/DialogUI.h"
#include "Game/Objects/UI/DialogEvent.h"
#include "Game/Objects/Charactor/Player.h"
#include "Game/Objects/Monster/Monster.h"

TestMap_1::TestMap_1(Scene* _scene, int _updateOrder, GameObject* _parent) :
	GameObject(_scene, OBJECT_TYPE::DEFAULT, _updateOrder, _parent)
	, m_scene(_scene)
	, m_targetSFX(_scene->GetGame()->m_targetSFX)
{
	/*
	* 카메라 설정
	*/
	CAMERA->SetRestrictRange(-80.0f, -1000.0f, 1264.0f, FLT_MAX);
	CAMERA->SetSpeed(1500.0f);

	/*
	* Create Background
	*/
	Background* background = nullptr;

#pragma region 배경 설정
	background = new Background(TEXTURE->Add(L"Background\\2.png"), SHADER(L"AlphaShader"), { -0.6f, -0.6f }, _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(-350, 20);
	m_backgrounds.emplace_back(background);

	background = new Background(TEXTURE->Add(L"Background\\2.png"), SHADER(L"AlphaShader"), { -0.6f, -0.6f }, _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(-350 + background->LocalSize().x, 20);
	m_backgrounds.emplace_back(background);

	background = new Background(TEXTURE->Add(L"Background\\3.png"), SHADER(L"AlphaShader"), { -0.3f, -0.5f }, _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(0, 100);
	m_backgrounds.emplace_back(background);

	background = new Background(TEXTURE->Add(L"Background\\3.png"), SHADER(L"AlphaShader"), { -0.3f, -0.5f }, _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(0 - background->LocalSize().x, 100);
	m_backgrounds.emplace_back(background);

	background = new Background(TEXTURE->Add(L"Background\\4.png"), SHADER(L"AlphaShader"), { -0.1f, -0.3f }, _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(0 - background->LocalSize().x, 150);
	m_backgrounds.emplace_back(background);

	background = new Background(TEXTURE->Add(L"Background\\4.png"), SHADER(L"AlphaShader"), { -0.1f, -0.3f }, _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(0, 150);
	m_backgrounds.emplace_back(background);

	background = new Background(TEXTURE->Add(L"Background\\5.png"), SHADER(L"AlphaShader"), { 0.2f, -0.1f }, _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(0 - background->LocalSize().x, 200);
	m_backgrounds.emplace_back(background);

	background = new Background(TEXTURE->Add(L"Background\\5.png"), SHADER(L"AlphaShader"), { 0.2f, -0.1f }, _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(0, 200);
	m_backgrounds.emplace_back(background);
#pragma endregion

	/*
	* Create platforms
    */
	PlatformRect *platform, *platformLT, *platformMT, *platformRT, *platformLM, *platformMM, *platformRM, *platformLB, *platformMB, *platformRB;

#pragma region 왼쪽 벽 세트
	// 우하단
	platformRB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_16.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platformRB->SetPos(-368, 192);
	// 중하단
	platformMB = new PlatformRect({ 64, 32 }, L"Tile\\IndustrialTile_14.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRB);
	platformMB->SetPos(-48, 0);
	// 좌하단
	platformLB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_12.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMB);
	platformLB->SetPos(-48, 0);
	// 우중단
	platformRM = new PlatformRect({ 32, 448 }, L"Tile\\IndustrialTile_15.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRB);
	platformRM->SetPos(0, -240);
	platformRM->GetCollider()->options.slidable = true;
	platformRM->GetCollider()->options.resistance.y = 10.0f;
	// 중중단
	platformMM = new PlatformRect({ 64, 448 }, L"Tile\\IndustrialTile_14.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMB);
	platformMM->SetPos(0, -240);
	// 좌중단
	platformLM = new PlatformRect({ 32, 448 }, L"Tile\\IndustrialTile_13.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLB);
	platformLM->SetPos(0, -240);
	// 우상단
	platformRT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_06.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRM);
	platformRT->SetPos(0, -240);
	platformRT->GetCollider()->options.slidable = true;
	platformRT->GetCollider()->options.resistance.y = 10.0f;
	// 중상단
	platformMT = new PlatformRect({ 64, 32 }, L"Tile\\IndustrialTile_05.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMT->SetPos(0, -240);
	// 좌상단
	platformLT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_04.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLM);
	platformLT->SetPos(0, -240);
#pragma endregion
	platform = new PlatformRect({ 128, 320 }, L"Tile\\IndustrialTile_02.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMB);
	platform->SetPos(0, 176);

	// 굴뚝
	platform = new PlatformRect({ 32, 160 }, L"Tile\\IndustrialTile_63.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLT);
	platform->SetPos(32, -96);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_54.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -96);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_45.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -32);

#pragma region 아래쪽 바닥 세트 1
	// 좌상단
	platformLT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_31.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platformLT->SetPos(-336, 192);
	platformLT->GetCollider()->SetTag("LeftFloor");
	// 상단
	platformMT = new PlatformRect({ 704, 32 }, L"Tile\\IndustrialTile_32.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLT);
	platformMT->SetPos(368, 0);
	platformMT->GetCollider()->SetTag("Floor");
	// 우상단
	platformRT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_33.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMT);
	platformRT->SetPos(368, 0);
	platformRT->GetCollider()->options.slidable = true;
	platformRT->GetCollider()->options.resistance.y = 10.0f;
	// 좌중간
	platformLM = new PlatformRect({ 32, 384 }, L"Tile\\IndustrialTile_40.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLT);
	platformLM->SetPos(0, 208);
	// 중간
	platformMM = new PlatformRect({ 704, 384 }, L"Tile\\IndustrialTile_41.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMT);
	platformMM->SetPos(0, 208);
	// 우중단
	platformRM = new PlatformRect({ 32, 224 }, L"Tile\\IndustrialTile_42.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRT);
	platformRM->SetPos(0, 128);
	platformRM->GetCollider()->options.slidable = true;
	platformRM->GetCollider()->options.resistance.y = 10.0f;
	platformRM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_43.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRM);
	platformRM->SetPos(0, 128);
	platformRM = new PlatformRect({ 32, 128 }, L"Tile\\IndustrialTile_41.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRM);
	platformRM->SetPos(0, 80);
	// 좌하단
	platformLB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_49.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLM);
	platformLB->SetPos(0, 208);
	// 중하단
	platformMB = new PlatformRect({ 704, 32 }, L"Tile\\IndustrialTile_50.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMB->SetPos(0, 208);
	// 우하단
	platformRB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_50.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRM);
	platformRB->SetPos(0, 80);
#pragma endregion

#pragma region 아래쪽 바닥 세트 2
	// 좌하단
	platformLB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_50.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRB);
	platformLB->SetPos(32, 0);
	// 중하단
	platformMB = new PlatformRect({ 1408, 32 }, L"Tile\\IndustrialTile_50.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLB);
	platformMB->SetPos(720, 0);
	// 우하단
	platformRB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_51.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMB);
	platformRB->SetPos(720, 0);
	platformRB->GetCollider()->options.slidable = true;
	platformRB->GetCollider()->options.resistance.y = 10.0f;
	// 좌중간
	platformLM = new PlatformRect({ 32, 128 }, L"Tile\\IndustrialTile_41.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLB);
	platformLM->SetPos(0, -80);
	// 중간
	platformMM = new PlatformRect({ 1408, 128 }, L"Tile\\IndustrialTile_41.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMB);
	platformMM->SetPos(0, -80);
	// 우중단
	platformRM = new PlatformRect({ 32, 128 }, L"Tile\\IndustrialTile_42.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRB);
	platformRM->SetPos(0, -80);
	platformRM->GetCollider()->options.slidable = true;
	platformRM->GetCollider()->options.resistance.y = 10.0f;
	// 좌상단
	platformLT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_32.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLM);
	platformLT->SetPos(0, -80);
	// 상단
	platformMT = new PlatformRect({ 1408, 32 }, L"Tile\\IndustrialTile_32.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMT->SetPos(0, -80);
	// 우상단
	platformRT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_33.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRM);
	platformRT->SetPos(0, -80);
	platformRT->GetCollider()->options.slidable = true;
	platformRT->GetCollider()->options.resistance.y = 10.0f;
#pragma endregion

#pragma region 우측 벽 세트
	// 좌하단
	platformLB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_36.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRB);
	platformLB->SetPos(32, 0);
	// 중하단
	platformMB = new PlatformRect({ 160, 32 }, L"Tile\\IndustrialTile_36.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLB);
	platformMB->SetPos(96, 0);
	// 우하단
	platformRB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_36.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMB);
	platformRB->SetPos(96, 0);
	// 좌중단
	platformLM = new PlatformRect({ 32, 480 }, L"Tile\\IndustrialTile_13.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLB);
	platformLM->SetPos(0, -256);
	platformLM->GetCollider()->options.slidable = true;
	platformLM->GetCollider()->options.resistance.y = 10.0f;
	// 중중단
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_28.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLB);
	platformMM->SetPos(32, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_19.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_10.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_28.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_19.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_10.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_28.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_19.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_10.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_28.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_19.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_10.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_28.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_19.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_10.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 128, 480 }, L"Tile\\IndustrialTile_14.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMB);
	platformMM->SetPos(16, -256);
	// 우중단
	platformRM = new PlatformRect({ 32, 480 }, L"Tile\\IndustrialTile_15.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRB);
	platformRM->SetPos(0, -256);
	// 좌상단
	platformLT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_04.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLM);
	platformLT->SetPos(0, -256);
	platformRT->GetCollider()->options.slidable = true;
	platformRT->GetCollider()->options.resistance.y = 10.0f;
	// 중상단
	platformMT = new PlatformRect({ 160, 32 }, L"Tile\\IndustrialTile_05.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMT->SetPos(-16, -256);
	// 우상단
	platformRT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_06.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRM);
	platformRT->SetPos(0, -256);
#pragma endregion
	
	// 굴뚝
	platform = new PlatformRect({ 32, 160 }, L"Tile\\IndustrialTile_63.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLT);
	platform->SetPos(32, -96);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_54.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -96);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_45.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -32);

	// 굴뚝
	platform = new PlatformRect({ 32, 160 }, L"Tile\\IndustrialTile_63.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRT);
	platform->SetPos(-32, -96);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_54.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -96);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_45.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -32);

#pragma region 중앙 벤트
	// 중앙 벤트
	platformLT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_55.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platformLT->SetPos(432, 96);
	platformMT = new PlatformRect({ 128, 32 }, L"Tile\\IndustrialTile_56.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLT);
	platformMT->SetPos(80, 0);
	platformRT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_57.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMT);
	platformRT->SetPos(80, 0);
	platformLM = new PlatformRect({ 32, 192 }, L"Tile\\IndustrialTile_13.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRT);
	platformLM->SetPos(-64, 112);
	platformLM->GetCollider()->options.slidable = true;
	platformLM->GetCollider()->options.resistance.y = 10.0f;
    platformLB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_22.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLM);
	platformLB->SetPos(0, 112);
	platformLB->GetCollider()->options.slidable = true;
	platformLB->GetCollider()->options.resistance.y = 10.0f;
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_01.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRT);
	platformMM->SetPos(-32, 32);
	platformMM = new PlatformRect({ 32, 64 }, L"Tile\\IndustrialTile_19.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, 48);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_28.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, 48);
	platformMM = new PlatformRect({ 32, 64 }, L"Tile\\IndustrialTile_21.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, 48);
	platformMB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_23.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMB->SetPos(0, 48);
	platformRB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_24.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMB);
	platformRB->SetPos(32, 0);
	platformRM = new PlatformRect({ 32, 192 }, L"Tile\\IndustrialTile_15.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRB);
	platformRM->SetPos(0, -112);
	platformRM->GetCollider()->options.slidable = true;
	platformRM->GetCollider()->options.resistance.y = 10.0f;
#pragma endregion

	/*
	* 첫 입장 이벤트
	*/
	m_enterEvent = new DialogEvent({ -50, -50 }, { 50, 50 }, _scene, OBJECT_TYPE::EVENTFLAG);
	m_enterEvent->SetPos(150, 600);
	m_enterEvent->GetCollider()->SetCallbackOnCollisionEnter([this](Collider* _other) {
		m_dialogUI->SetText(L"안녕하세요. 저는 당신의 여정을 도울 D2라고 합니다.\n우선 대화창 우측 하단에 'F'키 입력 대기 아이콘이 나타나면\n'F'키를 눌러볼까요?");
		m_dialogUI->IsWaiting(true);
		m_dialogUI->SetState(OBJECT_STATE::ACTIVE);
		m_dialogUI->SetUpdateEvent([this]() {
			if (KEYDOWN('F') && m_dialogUI->IsWait())
			{
				m_dialogUI->Clear();
				m_dialogUI->SetText(L"잘하셨어요. 다음엔 저기 태블릿이 보이시나요?\n저 태블릿을 작동시켜야 문이 열릴거에요.");
                m_dialogUI->IsWaiting(true);
				CAMERA->SetTarget(button_01);
				m_targetSFX->Play(button_01->GetWorldPos());
				m_dialogUI->SetUpdateEvent([this]() {
					if (KEYDOWN('F') && m_dialogUI->IsWait())
					{
						CAMERA->SetTarget(m_player);
						m_targetSFX->Stop();
						m_dialogUI->Clear();
						m_dialogUI->SetText(L"저기로 한 번 가보죠.\n좌(←), 우(→) 방향키와 위(↑) 쪽 방향키로 움직일 수 있어요.\n위(↑)쪽 방향키를 누르면 점프를 할 수 있죠.");
						m_dialogUI->IsWaiting(false);
						m_dialogUI->SetWaitTime(2.0f);
						m_player->SetPreventKey(false);
						m_dialogUI->SetUpdateEvent([this]() {
							if (KEYDOWN(VK_UP) || KEYDOWN(VK_RIGHT) || KEYDOWN(VK_LEFT))
							{
								m_dialogUI->SetState(OBJECT_STATE::HIDDEN);
								m_dialogUI->Clear();
								m_enterEvent->GetCollider()->IsActive(false);
							}
							else if (m_dialogUI->IsWaitOver())
							{
								m_dialogUI->IsFirst(true);
								m_dialogUI->SetText(L"아! 말하는 것을 잊었네요.\n대화가 끝났지만 우측아래 'F'키 입력 대기 아이콘이 나타나지 않는다면\n해당 목표를 완료할 때까지 기다리는 거에요.\n자, 방향키를 눌러 목표로 가보죠.");
							}
						});
					}
				});
			}
		});
	});
    
	/*
	* Event 생성 (문 열기)
	*/
    door_01 = new Door(_scene, OBJECT_TYPE::FOREBLOCK, _updateOrder, this);
	door_01->SetPos(400, 144);
	door_01->SetOpenFunc([](){
		CAMERA->SetRestrictRange(-80.0f, -1000.0f, FLT_MAX, FLT_MAX);
	});
    
	button_01 = new ScreenButton(_scene, OBJECT_TYPE::INTERACTIVE, _updateOrder, this);
	button_01->SetPos(300, 155);
	button_01->SetInteractObject(door_01);

    platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_71.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, door_01);
	platform->SetPos(0, -48);
    platform = new PlatformRect({ 32, 320 }, L"Tile\\IndustrialTile_73.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -176);

	m_doorOpenEvent = new DialogEvent({ -50, -40 }, { 50, 30 }, _scene, OBJECT_TYPE::EVENTFLAG);
	m_doorOpenEvent->SetPos(1100, 600);
	m_doorOpenEvent->GetCollider()->SetCallbackOnCollisionEnter([this](Collider* _other) {
		if (_other->GetOwner()->GetType() == OBJECT_TYPE::PLAYER && m_doorOpenEvent->IsFirst())	// 플레이어가 첫 진입인 경우
		{
			m_player->SetPreventKey(true);    // 키 입력 방지
			m_dialogUI->Clear();
			m_dialogUI->SetText(L"좋아요. 이제 태블릿 앞에서 'F'키를 눌러 문을 열어볼까요?\n태블릿 앞으로 정확히 다가가면 태블릿 위에 'F' 키보드 모양이 뜰 거에요.");
			m_dialogUI->SetState(OBJECT_STATE::ACTIVE);
			m_dialogUI->IsWaiting(false);
			m_doorOpenEvent->IsFirst(false);
		}
	});
	m_doorOpenEvent->GetCollider()->SetCallbackOnCollisionStay([this](Collider* _other) {
		if (_other->GetOwner()->GetType() == OBJECT_TYPE::PLAYER && m_dialogUI->IsEnd())	// 대화가 끝난 경우 움직일 수 있게 한다.
			m_player->SetPreventKey(false);
		
		if (door_01->IsUsed())
		{
			button_01->GetInteractBox()->IsActive(false);
			m_doorOpenEvent->GetCollider()->IsActive(false);
			m_dialogUI->SetState(OBJECT_STATE::HIDDEN);
			m_dialogUI->Clear();
		}
	});

    
    /*
	* 몬스터 발견 이벤트
	*/
	monster_01 = MonsterFactory::CreateMonster(m_scene, MONSTERS::MMM, { 800, 300 }, this);
	monsterFindEvent = new DialogEvent({ -100, -80 }, { 100, 80 }, _scene, OBJECT_TYPE::EVENTFLAG);
	monsterFindEvent->SetPos(1350, 930);
	monsterFindEvent->GetCollider()->SetCallbackOnCollisionEnter([this](Collider* _other) {
		if (_other->GetOwner()->GetType() == OBJECT_TYPE::PLAYER)
		{
			m_player->SetPreventKey(true);    // 키 입력 방지
			m_dialogUI->Clear();
			m_dialogUI->SetText(L"잠깐만요! 저기 오토마톤이에요!\n유한상태 오토마타로 만들어진 녀석으로 저희를 발견하면 공격할거에요.");
			m_dialogUI->IsWaiting(true);
			m_dialogUI->SetState(OBJECT_STATE::ACTIVE);
			m_targetSFX->Play(monster_01->GetWorldPos());
			CAMERA->SetTarget(monster_01);
			m_dialogUI->SetUpdateEvent([this]() {
				m_targetSFX->SetPos(monster_01->GetWorldPos());
				if (KEYDOWN('F') && m_dialogUI->IsWait())
				{
					m_targetSFX->Stop();
					m_dialogUI->Clear();
					m_dialogUI->SetText(L"우선, 'TAB'키를 누르면 무기를 바꿀 수 있어요.\n원거리 공격은 멀리서 공격할 수 있지만 약하고\n근거리 공격은 저 녀석을 한 방에 쓰러트릴 정도로 강하죠.");
					m_dialogUI->IsWaiting(true);
					CAMERA->SetTarget(m_player);
					m_dialogUI->SetUpdateEvent([this]() {
						if (KEYDOWN('F') && m_dialogUI->IsWait())
						{
							m_dialogUI->Clear();
							m_dialogUI->SetText(L"어떻게 공격할지는 자유에요!\n자, 다음으로 넘어가기 위해 저 녀석을 부숴버리죠!");
							m_dialogUI->IsWaiting(true);
							m_dialogUI->SetUpdateEvent([this]() {
								if (KEYDOWN('F') && m_dialogUI->IsWait())
								{
									m_player->SetPreventKey(false);
									monsterFindEvent->GetCollider()->IsActive(false);
									m_dialogUI->SetState(OBJECT_STATE::HIDDEN);
									m_dialogUI->Clear();
								}
							});
						}
					});
				}
			});
		}
	});

    /*
	*/
	m_PlayerStartFlag = new PlatformRect({ 10, 10 }, false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, this);
	m_PlayerStartFlag->SetPos(-336, 152);

	SetMonsters();

}

TestMap_1::~TestMap_1()
{
}

void TestMap_1::CameraInit()
{
	for (auto& bg : m_backgrounds)
	{
		bg->InitCameraPos();
	}
}

void TestMap_1::UpdateObject()
{
	for (auto& bg : m_backgrounds)
	{
		D3DXVECTOR2 camDir = bg->GetCameraDir();
		if (camDir.x == 0 && camDir.y == 0)
            continue;
        
		if (camDir.x > 0 && bg->Right() < CAMERA->Left())
		{
			bg->AddPos((bg->LocalSize().x * 2.0f), 0.0f);
		}
        else if (camDir.x < 0 && bg->Left() > CAMERA->Right())
        {
            bg->AddPos(-(bg->LocalSize().x * 2.0f), 0.0f);
        }
	}
}

void TestMap_1::RenderObject()
{
}

D3DXVECTOR2 TestMap_1::GetPlayerStartPoint()
{
	assert(m_PlayerStartFlag);

	D3DXVECTOR2 pos = m_PlayerStartFlag->GetWorldPos();
	
	return pos;
}

void TestMap_1::SetMonsters()
{
	
}
