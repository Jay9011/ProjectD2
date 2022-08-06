#include "Framework.h"
#include "TestMap_1.h"

#include "Engine/Object/GameObject.h"
#include "Engine/Resource/Shader.h"
#include "Engine/Component/Component.h"
#include "Engine/Component/Animator/Animator.h"
#include "Engine/Component/Collision/Colliders/AARect.h"
#include "Engine/Object/Effect.h"

#include "Game/Objects/Background/Background.h"
#include "Game/Objects/Interactive/ScreenButton.h"
#include "Game/Objects/Interactive/Door.h"
#include "Game/Objects/Platforms/PlatformRect.h"
#include "Game/Objects/Platforms/FallPlatform.h"
#include "Game/Objects/UI/DialogUI.h"
#include "Game/Objects/UI/DialogEvent.h"
#include "Game/Objects/Charactor/Player.h"
#include "Game/Objects/Monster/Monster.h"
#include "Game/Objects/Monster/MonsterFactory.h"

TestMap_1::TestMap_1(Scene* _scene, int _updateOrder, GameObject* _parent) :
	GameObject(_scene, OBJECT_TYPE::DEFAULT, _updateOrder, _parent)
	, m_scene(_scene)
	, m_targetSFX(_scene->GetGame()->m_targetSFX)
{
	/*
	* ī�޶� ����
	*/
	//CAMERA->SetRestrictRange(-80.0f, -1000.0f, 1264.0f, FLT_MAX);
	CAMERA->SetSpeed(1500.0f);

	/*
	* Create Background
	*/
	Background* background = nullptr;

#pragma region ��� ����
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
    FallPlatform *fallPlatform;

#pragma region ���� �� ��Ʈ
	// ���ϴ�
	platformRB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_16.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platformRB->SetPos(-368, 192);
	// ���ϴ�
	platformMB = new PlatformRect({ 64, 32 }, L"Tile\\IndustrialTile_14.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRB);
	platformMB->SetPos(-48, 0);
	// ���ϴ�
	platformLB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_12.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMB);
	platformLB->SetPos(-48, 0);
	// ���ߴ�
	platformRM = new PlatformRect({ 32, 448 }, L"Tile\\IndustrialTile_15.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRB);
	platformRM->SetPos(0, -240);
	platformRM->GetCollider()->options.slidable = true;
	platformRM->GetCollider()->options.resistance.y = 10.0f;
	// ���ߴ�
	platformMM = new PlatformRect({ 64, 448 }, L"Tile\\IndustrialTile_14.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMB);
	platformMM->SetPos(0, -240);
	// ���ߴ�
	platformLM = new PlatformRect({ 32, 448 }, L"Tile\\IndustrialTile_13.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLB);
	platformLM->SetPos(0, -240);
	// ����
	platformRT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_06.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRM);
	platformRT->SetPos(0, -240);
	platformRT->GetCollider()->options.slidable = true;
	platformRT->GetCollider()->options.resistance.y = 10.0f;
	// �߻��
	platformMT = new PlatformRect({ 64, 32 }, L"Tile\\IndustrialTile_05.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMT->SetPos(0, -240);
	// �»��
	platformLT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_04.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLM);
	platformLT->SetPos(0, -240);
    
	platform = new PlatformRect({ 128, 320 }, L"Tile\\IndustrialTile_02.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMB);
	platform->SetPos(0, 176);

	// ����
	platform = new PlatformRect({ 32, 160 }, L"Tile\\IndustrialTile_63.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLT);
	platform->SetPos(32, -96);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_54.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -96);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_45.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -32);
    
#pragma endregion

#pragma region �Ʒ��� �ٴ� ��Ʈ 1
	// �»��
	platformLT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_31.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platformLT->SetPos(-336, 192);
	platformLT->GetCollider()->SetTag("LeftFloor");
	// ���
	platformMT = new PlatformRect({ 704, 32 }, L"Tile\\IndustrialTile_32.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLT);
	platformMT->SetPos(368, 0);
	platformMT->GetCollider()->SetTag("Floor");
	// ����
	platformRT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_33.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMT);
	platformRT->SetPos(368, 0);
	platformRT->GetCollider()->options.slidable = true;
	platformRT->GetCollider()->options.resistance.y = 10.0f;
	// ���߰�
	platformLM = new PlatformRect({ 32, 384 }, L"Tile\\IndustrialTile_40.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLT);
	platformLM->SetPos(0, 208);
	// �߰�
	platformMM = new PlatformRect({ 704, 384 }, L"Tile\\IndustrialTile_41.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMT);
	platformMM->SetPos(0, 208);
	// ���ߴ�
	platformRM = new PlatformRect({ 32, 224 }, L"Tile\\IndustrialTile_42.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRT);
	platformRM->SetPos(0, 128);
	platformRM->GetCollider()->options.slidable = true;
	platformRM->GetCollider()->options.resistance.y = 10.0f;
	platformRM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_43.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRM);
	platformRM->SetPos(0, 128);
	platformRM = new PlatformRect({ 32, 128 }, L"Tile\\IndustrialTile_41.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRM);
	platformRM->SetPos(0, 80);
	// ���ϴ�
	platformLB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_49.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLM);
	platformLB->SetPos(0, 208);
	// ���ϴ�
	platformMB = new PlatformRect({ 704, 32 }, L"Tile\\IndustrialTile_50.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMB->SetPos(0, 208);
	// ���ϴ�
	platformRB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_50.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRM);
	platformRB->SetPos(0, 80);
    
	// ��ĵ�
	platform = new PlatformRect({ 32, 36 }, L"Tile\\Locker4.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platformLT);
	platform->SetPos(170, -34);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\Locker3.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(25, 4);
	platform = new PlatformRect({ 32, 36 }, L"Tile\\Locker4.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(25, -4);
	platform = new PlatformRect({ 32, 36 }, L"Tile\\Locker4.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(25, 0);
	platform = new PlatformRect({ 32, 36 }, L"Tile\\Mop.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(0, 0);
	platform = new PlatformRect({ 32, 36 }, L"Tile\\Bucket.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(15, 0);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\Box6.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(123, 4);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\Box3.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(17, 0);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\Box4.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(20, 0);
	platform = new PlatformRect({ 32, 36 }, L"Tile\\Locker4.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(100, -4);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\Box4.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(2, -34);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\Locker3.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(20, 39);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\Fence1.png", false, _scene, OBJECT_TYPE::FOREGROUND, _updateOrder, platform);
	platform->SetPos(40, -3);
	platform = new PlatformRect({ 64, 32 }, L"Tile\\Fence2.png", false, _scene, OBJECT_TYPE::FOREGROUND, _updateOrder, platform);
	platform->SetPos(48, 0);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\Fence3.png", false, _scene, OBJECT_TYPE::FOREGROUND, _updateOrder, platform);
	platform->SetPos(32, 0);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\Pointer2.png", false, _scene, OBJECT_TYPE::FOREGROUND, _updateOrder, platform);
	platform->SetPos(40, 0);
#pragma endregion

#pragma region �Ʒ��� �ٴ� ��Ʈ 2
	// ���ϴ�
	platformLB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_50.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRB);
	platformLB->SetPos(32, 0);
	// ���ϴ�
	platformMB = new PlatformRect({ 1408, 32 }, L"Tile\\IndustrialTile_50.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLB);
	platformMB->SetPos(720, 0);
	// ���ϴ�
	platformRB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_51.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMB);
	platformRB->SetPos(720, 0);
	platformRB->GetCollider()->options.slidable = true;
	platformRB->GetCollider()->options.resistance.y = 10.0f;
	// ���߰�
	platformLM = new PlatformRect({ 32, 128 }, L"Tile\\IndustrialTile_41.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLB);
	platformLM->SetPos(0, -80);
	// �߰�
	platformMM = new PlatformRect({ 1408, 128 }, L"Tile\\IndustrialTile_41.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMB);
	platformMM->SetPos(0, -80);
	// ���ߴ�
	platformRM = new PlatformRect({ 32, 128 }, L"Tile\\IndustrialTile_42.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRB);
	platformRM->SetPos(0, -80);
	platformRM->GetCollider()->options.slidable = true;
	platformRM->GetCollider()->options.resistance.y = 10.0f;
	// �»��
	platformLT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_32.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLM);
	platformLT->SetPos(0, -80);
	// ���
	platformMT = new PlatformRect({ 1408, 32 }, L"Tile\\IndustrialTile_32.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMT->SetPos(0, -80);
	// ����
	platformRT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_33.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRM);
	platformRT->SetPos(0, -80);
	platformRT->GetCollider()->options.slidable = true;
	platformRT->GetCollider()->options.resistance.y = 10.0f;

	platform = new PlatformRect({ 32, 32 }, L"Tile\\Pointer1.png", false, _scene, OBJECT_TYPE::FOREGROUND, _updateOrder, platformRT);
	platform->SetPos(0, -32);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\Pointer1.png", false, _scene, OBJECT_TYPE::FOREGROUND, _updateOrder, platform);
	platform->SetPos(-12, 0);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\Pointer1.png", false, _scene, OBJECT_TYPE::FOREGROUND, _updateOrder, platform);
	platform->SetPos(-12, 0);
#pragma endregion

#pragma region ���� �� ��Ʈ
	// ���ϴ�
	platformLB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_36.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRB);
	platformLB->SetPos(32, 0);
	// ���ϴ�
	platformMB = new PlatformRect({ 160, 32 }, L"Tile\\IndustrialTile_36.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLB);
	platformMB->SetPos(96, 0);
	// ���ϴ�
	platformRB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_36.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMB);
	platformRB->SetPos(96, 0);
	// ���ߴ�
	platformLM = new PlatformRect({ 32, 480 }, L"Tile\\IndustrialTile_13.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLB);
	platformLM->SetPos(0, -256);
	platformLM->GetCollider()->options.slidable = true;
	platformLM->GetCollider()->options.resistance.y = 10.0f;
	// ���ߴ�
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
	// ���ߴ�
	platformRM = new PlatformRect({ 32, 480 }, L"Tile\\IndustrialTile_15.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRB);
	platformRM->SetPos(0, -256);
	// �»��
	platformLT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_04.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLM);
	platformLT->SetPos(0, -256);
	platformRT->GetCollider()->options.slidable = true;
	platformRT->GetCollider()->options.resistance.y = 10.0f;
	// �߻��
	platformMT = new PlatformRect({ 160, 32 }, L"Tile\\IndustrialTile_05.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMT->SetPos(-16, -256);
	// ����
	platformRT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_06.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRM);
	platformRT->SetPos(0, -256);

	// ����
	platform = new PlatformRect({ 32, 160 }, L"Tile\\IndustrialTile_63.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLT);
	platform->SetPos(32, -96);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_54.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -96);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_45.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -32);

	// ����
	platform = new PlatformRect({ 32, 160 }, L"Tile\\IndustrialTile_63.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRT);
	platform->SetPos(-32, -96);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_54.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -96);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_45.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -32);

#pragma endregion
	
#pragma region �߾� ��Ʈ
	// �߾� ��Ʈ
	platformLT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_55.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platformLT->SetPos(432, 96);
	platformMT = new PlatformRect({ 96, 32 }, L"Tile\\IndustrialTile_56.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLT);
	platformMT->SetPos(64, 0);
	platformRT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_57.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMT);
	platformRT->SetPos(64, 0);
	platformRM = new PlatformRect({ 32, 192 }, L"Tile\\IndustrialTile_15.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRT);
	platformRM->SetPos(0, 112);
	platformRM->GetCollider()->options.slidable = true;
	platformRM->GetCollider()->options.resistance.y = 10.0f;
	platformRB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_24.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRM);
	platformRB->SetPos(0, 112);
	platformRB->GetCollider()->options.slidable = true;
	platformRB->GetCollider()->options.resistance.y = 10.0f;
	platformLB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_22.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRB);
	platformLB->SetPos(-32, 0);
	platformLB->GetCollider()->options.slidable = true;
	platformLB->GetCollider()->options.resistance.y = 10.0f;
	platformLM = new PlatformRect({ 32, 192 }, L"Tile\\IndustrialTile_13.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLB);
	platformLM->SetPos(0, -112);
	platformLM->GetCollider()->options.slidable = true;
	platformLM->GetCollider()->options.resistance.y = 10.0f;
    
	platform = new PlatformRect({ 96, 124 }, L"Tile\\IndustrialTile_01.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platformRT);
	platform->SetPos(-96, 78);
	platform = new PlatformRect({ 96, 128 }, L"Tile\\IndustrialTile_19.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(0, 80);
	platform = new PlatformRect({ 96, 32 }, L"Tile\\IndustrialTile_28.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(0, 80);
	platform = new PlatformRect({ 160, 96 }, L"Tile\\IndustrialTile_21.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(32, 50);
    
	platform = new PlatformRect({ 32, 64 }, L"Tile\\Flag.png", false, _scene, OBJECT_TYPE::FOREGROUND, _updateOrder, platformRT);
	platform->SetPos(0, 32);
	platform = new PlatformRect({ 32, 64 }, L"Tile\\Flag.png", false, _scene, OBJECT_TYPE::FOREGROUND, _updateOrder, platform);
	platform->SetPos(-32, 0);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\Pointer1.png", false, _scene, OBJECT_TYPE::FOREGROUND, _updateOrder, platform);
	platform->SetPos(32, 288);
    
#pragma endregion

#pragma region ù ���� Event

	m_enterEvent = new DialogEvent({ -50, -50 }, { 50, 50 }, _scene, OBJECT_TYPE::EVENTFLAG);
	m_enterEvent->SetPos(150, 600);
	m_enterEvent->GetCollider()->SetCallbackOnCollisionEnter([this](Collider* _other) {
		m_dialogUI->SetText(L"�ȳ��ϼ���. ���� ����� ������ ���� D2��� �մϴ�.\n�켱 ��ȭâ ���� �ϴܿ� 'F'Ű �Է� ��� �������� ��Ÿ����\n'F'Ű�� ���������?");
		m_dialogUI->IsWaiting(true);
		m_dialogUI->SetState(OBJECT_STATE::ACTIVE);
		m_dialogUI->SetUpdateEvent([this]() {
			if (KEYDOWN('F') && m_dialogUI->IsWait())
			{
				m_dialogUI->Clear();
				m_dialogUI->SetText(L"���ϼ̾��. ������ ���� �º��� ���̽ó���?\n�� �º��� �۵����Ѿ� ���� �����ſ���.");
				m_dialogUI->IsWaiting(true);
				CAMERA->SetTarget(button_01);
				m_targetSFX->Play(button_01->GetWorldPos());
				m_dialogUI->SetUpdateEvent([this]() {
					if (KEYDOWN('F') && m_dialogUI->IsWait())
					{
						CAMERA->SetTarget(m_player);
						m_targetSFX->Stop();
						m_dialogUI->Clear();
						m_dialogUI->SetText(L"����� �� �� ������.\n��(��), ��(��) ����Ű�� ��(��) �� ����Ű�� ������ �� �־��.\n��(��)�� ����Ű�� ������ ������ �� �� ����.");
						m_dialogUI->IsWaiting(false);
						m_dialogUI->SetWaitTime(2.0f);
						m_player->SetPreventKey(false);
						m_dialogUI->SetUpdateEvent([this]() {
							if (KEYPRESS(VK_UP) || KEYPRESS(VK_RIGHT) || KEYPRESS(VK_LEFT))
							{
								m_dialogUI->SetState(OBJECT_STATE::HIDDEN);
								m_dialogUI->Clear();
								m_enterEvent->GetCollider()->IsActive(false);
							}
							else if (m_dialogUI->IsWaitOver())
							{
								m_dialogUI->IsFirst(true);
								m_dialogUI->SetText(L"��! ���ϴ� ���� �ؾ��׿�.\n��ȭ�� �������� �����Ʒ� 'F'Ű �Է� ��� �������� ��Ÿ���� �ʴ´ٸ�\n�ش� ��ǥ�� �Ϸ��� ������ ��ٸ��� �ſ���.\n��, ����Ű�� ���� ��ǥ�� ������.");
							}
							});
					}
					});
			}
			});
		});

#pragma endregion

#pragma region ������ Event

	door_01 = new Door(_scene, OBJECT_TYPE::FOREBLOCK, _updateOrder, this);
	door_01->SetPos(400, 144);
	door_01->SetOpenFunc([]() {
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
		if (_other->GetOwner()->GetType() == OBJECT_TYPE::PLAYER && m_doorOpenEvent->IsFirst())	// �÷��̾ ù ������ ���
		{
			m_player->SetPreventKey(true);    // Ű �Է� ����
			m_dialogUI->Clear();
			m_dialogUI->SetText(L"���ƿ�. ���� �º� �տ��� 'F'Ű�� ���� ���� ������?\n�º� ������ ��Ȯ�� �ٰ����� �º� ���� 'F' Ű���� ����� �� �ſ���.");
			m_dialogUI->SetState(OBJECT_STATE::ACTIVE);
			m_dialogUI->IsWaiting(false);
			m_doorOpenEvent->IsFirst(false);
		}
	});
	m_doorOpenEvent->GetCollider()->SetCallbackOnCollisionStay([this](Collider* _other) {
		if (_other->GetOwner()->GetType() == OBJECT_TYPE::PLAYER && m_dialogUI->IsEnd())	// ��ȭ�� ���� ��� ������ �� �ְ� �Ѵ�.
			m_player->SetPreventKey(false);

		if (door_01->IsUsed())
		{
			CAMERA->Shake(0.5f, 30.0f);
			button_01->GetInteractBox()->IsActive(false);
			m_doorOpenEvent->GetCollider()->IsActive(false);
			m_dialogUI->Clear();
			m_dialogUI->SetState(OBJECT_STATE::HIDDEN);
		}
	});


#pragma endregion

#pragma region ���� �߰� Event

	monster_01 = MonsterFactory::CreateMonster(m_scene, MONSTERS::MMM, { 750, 300 }, this);
	monsterFindEvent = new DialogEvent({ -100, -80 }, { 100, 80 }, _scene, OBJECT_TYPE::EVENTFLAG);
	monsterFindEvent->SetPos(1350, 930);
	monsterFindEvent->GetCollider()->SetCallbackOnCollisionEnter([this](Collider* _other) {
		if (_other->GetOwner()->GetType() == OBJECT_TYPE::PLAYER)
		{
			m_player->SetPreventKey(true);    // Ű �Է� ����
			m_dialogUI->Clear();
			m_dialogUI->SetFace(DialogFace::Surprised);
			m_dialogUI->SetText(L"��񸸿�! ���� ���ѻ��� ���丶Ÿ�� ������� ���丶���̿���!\n���� �߰��ϸ� �����ϰԲ� ���α׷��� �Ǿ�����...");
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
					m_dialogUI->SetFace(DialogFace::Serious);
					m_dialogUI->SetText(L"'TAB'Ű�� ������ ���⸦ �ٲ� �� �־��.\n���Ÿ� ����� �ָ��� ���� ���������� ���ϰ�\n�ٰŸ� ����� �� �༮�� �� ���� �μ� ������ ������.\n�켱, 'TAB'Ű�� ����������?");
					m_dialogUI->IsWaiting(false);
					CAMERA->SetTarget(m_player);
					m_dialogUI->SetUpdateEvent([this]() {
						if (KEYDOWN(VK_TAB))
						{
							m_player->SwitchingWeapon();
							m_dialogUI->Clear();
							m_dialogUI->SetFace(DialogFace::Normal);
							m_dialogUI->SetText(L"'D'Ű�� ������ ������ �� �־��.\n��, �������� �Ѿ�� ���� �� �༮�� �ν�������!");
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
#pragma endregion

#pragma region �߾� ������� 1
	platform = new PlatformRect({ 320, 224 }, L"Tile\\IndustrialTile_46.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, this);
    platform->SetPos(1216, 320);
	platformLB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_22.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
    platformLB->SetPos(-208, 32);
	platformLB->GetCollider()->options.slidable = true;
	platformLB->GetCollider()->options.resistance.y = 10.0f;
	platformLM = new PlatformRect({ 32, 96 }, L"Tile\\IndustrialTile_13.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLB);
    platformLM->SetPos(0, -64);
	platformLM->GetCollider()->options.slidable = true;
	platformLM->GetCollider()->options.resistance.y = 10.0f;
	platformLT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_04.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLM);
    platformLT->SetPos(0, -64);
	platformLT->GetCollider()->options.slidable = true;
	platformLT->GetCollider()->options.resistance.y = 10.0f;
	platformMT = new PlatformRect({ 192, 32 }, L"Tile\\IndustrialTile_05.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLT);
    platformMT->SetPos(112, 0);
	platformRT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_06.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMT);
    platformRT->SetPos(112, 0);
	platformRT->GetCollider()->options.slidable = true;
	platformRT->GetCollider()->options.resistance.y = 10.0f;
	platformRM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_24.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRT);
    platformRM->SetPos(0, 32);
	platformMM = new PlatformRect({ 160, 32 }, L"Tile\\IndustrialTile_23.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRM);
    platformMM->SetPos(-96, 0);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_07.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
    platformMM->SetPos(-96, 0);
	platformMB = new PlatformRect({ 32, 64 }, L"Tile\\IndustrialTile_15.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
    platformMB->SetPos(0, 48);
	platformMB->GetCollider()->options.slidable = true;
	platformMB->GetCollider()->options.resistance.y = 10.0f;
	platformRB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_24.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMB);
    platformRB->SetPos(0, 48);
	platformRB->GetCollider()->options.slidable = true;
	platformRB->GetCollider()->options.resistance.y = 10.0f;
    platform = new PlatformRect({ 64, 64 }, L"Tile\\IndustrialTile_46.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platformRB);
	platform->SetPos(-16, 48);
    platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_47.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(48, 0);
    platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_47.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(80, -98);
    platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_61.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(16, -16);
	monsterRegionObject = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_70.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	monsterRegionObject->SetPos(0, 32);
    
	fallPlatform = new FallPlatform(_scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRT);
	fallPlatform->SetPos(32, 0);
	fallPlatform = new FallPlatform(_scene, OBJECT_TYPE::PLATFORM, _updateOrder, fallPlatform);
	fallPlatform->SetPos(32, 0);
	fallPlatform = new FallPlatform(_scene, OBJECT_TYPE::PLATFORM, _updateOrder, fallPlatform);
	fallPlatform->SetPos(32, 0);
	fallPlatform = new FallPlatform(_scene, OBJECT_TYPE::PLATFORM, _updateOrder, fallPlatform);
	fallPlatform->SetPos(32, 0);

	platformLT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_73.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, fallPlatform);
	platformLT->SetPos(32, 0);
	platformLM = new PlatformRect({ 32, 192 }, L"Tile\\IndustrialTile_71.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, fallPlatform);
	platformLM->SetPos(32, 112);
	platformRT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_06.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLT);
	platformRT->SetPos(32, 0);
	platformRT->GetCollider()->options.slidable = true;
	platformRT->GetCollider()->options.resistance.y = 10.0f;
	platformRM = new PlatformRect({ 32, 192 }, L"Tile\\IndustrialTile_15.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRT);
	platformRM->SetPos(0, 112);
	platformRM->GetCollider()->options.slidable = true;
	platformRM->GetCollider()->options.resistance.y = 10.0f;
    
	platform = new PlatformRect({ 32, 36 }, L"Tile\\Bench.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platformLB);
	platform->SetPos(-70, 64);
	platform = new PlatformRect({ 32, 36 }, L"Tile\\Bench.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(32, 0);
	platform = new PlatformRect({ 32, 36 }, L"Tile\\Fire-extinguisher1.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(32, 0);
	platform = new PlatformRect({ 18, 26 }, L"Tile\\Barrel1.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(80, -18);
	platform = new PlatformRect({ 18, 26 }, L"Tile\\Barrel1.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(10, 21);
	platform = new PlatformRect({ 18, 26 }, L"Tile\\Barrel2.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(-15, 0);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\Board3.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(3, -72);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\Board2.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(32, 40);
	platform = new PlatformRect({ 32, 36 }, L"Tile\\Fire-extinguisher3.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(16, 27);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\Box7.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(175, 2);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\Box6.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(20, 0);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\Box7.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(20, 0);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\Box1.png", false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, platform);
	platform->SetPos(20, 0);
    
	platform = new PlatformRect({ 32, 32 }, L"Tile\\Fence3.png", false, _scene, OBJECT_TYPE::FOREGROUND, _updateOrder, platformRT);
	platform->SetPos(6, -32);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\Fence1.png", false, _scene, OBJECT_TYPE::FOREGROUND, _updateOrder, platform);
	platform->SetPos(-32, 0);
#pragma endregion

#pragma region ������� ���� ��ȯ �̺�Ʈ
	monsterRegion = new DialogEvent({ -250, -100 }, { 250, 80 }, _scene, OBJECT_TYPE::EVENTFLAG);
	monsterRegion->SetPos(2450, 930);
	monsterRegion->GetCollider()->SetCallbackOnCollisionEnter([this](Collider* _other) {
		if (_other->GetOwner()->GetType() == OBJECT_TYPE::MONSTER)
		{
			monsterExist = true;
		}
	});
	monsterRegion->GetCollider()->SetCallbackOnCollisionExit([this](Collider* _other) {
		if (_other->GetOwner()->GetType() == OBJECT_TYPE::MONSTER) // ���Ͱ� ���忡�� �������
		{
			monsterExist = false;
		}
	});
#pragma endregion

#pragma region ���� �߰� �̺�Ʈ
	factoryFindEvent = new DialogEvent({ -100, -80 }, { 100, 80 }, _scene, OBJECT_TYPE::EVENTFLAG);
	factoryFindEvent->SetPos(2000, 930);
	factoryFindEvent->GetCollider()->SetCallbackOnCollisionEnter([this](Collider* _other) {
		if (_other->GetOwner()->GetType() == OBJECT_TYPE::PLAYER)
		{
			monsterExist = false;
			m_player->SetPreventKey(true);    // Ű �Է� ����
			m_dialogUI->Clear();
			m_dialogUI->SetFace(DialogFace::Serious);
			m_dialogUI->SetText(L"���丮����...\n���⿡�� ���ѻ��� ���丶���� �̾Ƴ�������.");
			m_dialogUI->IsWaiting(true);
			m_dialogUI->SetState(OBJECT_STATE::ACTIVE);
			m_dialogUI->SetUpdateEvent([this]() {
				if (KEYDOWN('F') && m_dialogUI->IsWait())
				{
					m_dialogUI->Clear();
					m_dialogUI->SetFace(DialogFace::Normal);
					m_dialogUI->SetText(L"������ �� ���ʿ��� ���� ���� �� ������ ���� �ö���.\n���� ���ؼ� ����(��)�� �ϸ� ���� Ÿ�� �ö� �� �־��.");
					m_dialogUI->IsWaiting(true);
					m_dialogUI->SetUpdateEvent([this]() {
						if (KEYDOWN('F') && m_dialogUI->IsWait())
						{
							m_player->SetPreventKey(false);
							m_dialogUI->Clear();
							m_dialogUI->SetState(OBJECT_STATE::HIDDEN);
							factoryFindEvent->GetCollider()->IsActive(false);
						}
					});
				}
			});
		}
	});
#pragma endregion

#pragma region ������ �̺�Ʈ
	finishEvent = new DialogEvent({ -100, -80 }, { 100, 80 }, _scene, OBJECT_TYPE::EVENTFLAG);
	finishEvent->SetPos(3400, 930);
	finishEvent->GetCollider()->SetCallbackOnCollisionEnter([this](Collider* _other) {
		if (_other->GetOwner()->GetType() == OBJECT_TYPE::PLAYER)
		{
			m_player->SetPreventKey(true);    // Ű �Է� ����
			m_dialogUI->Clear();
			m_dialogUI->SetFace(DialogFace::Normal);
			m_dialogUI->SetText(L"�����ϼ̾��. �̹� ������ �����������.");
			m_dialogUI->IsWaiting(true);
			m_dialogUI->SetState(OBJECT_STATE::ACTIVE);
			m_dialogUI->SetUpdateEvent([this]() {
				if (KEYDOWN('F') && m_dialogUI->IsWait())
				{
					m_dialogUI->Clear();
					m_dialogUI->SetText(L"������ ����������� �� �� �ѷ����� �ʹٸ� �ѷ����Ƶ� ���ƿ�.\n�� ���� �� ���丮���� ���丶���� ������ �����Ǵ� �װɷ� ��Ƶ� ������.");
					m_dialogUI->IsWaiting(true);
					m_dialogUI->SetUpdateEvent([this]() {
						if (KEYDOWN('F') && m_dialogUI->IsWait())
						{
							m_dialogUI->Clear();
							m_dialogUI->SetText(L"�� ��������� �ȳ��ҰԿ�.\n������� ������༭ ������.");
							m_dialogUI->IsWaiting(true);
							m_dialogUI->SetUpdateEvent([this]() {
								if (KEYDOWN('F') && m_dialogUI->IsWait())
								{
									m_player->SetPreventKey(false);
									m_dialogUI->Clear();
									m_dialogUI->SetState(OBJECT_STATE::HIDDEN);
									finishEvent->GetCollider()->IsActive(false);
								}
							});
						}
					});
				}
			});
		}
	});
#pragma endregion

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

    /*
	* ���� ��ȯ �̺�Ʈ
	*/
	if (!monsterExist)	// ���Ͱ� ������ ������
	{
		monsterTimer += fDT;
		if (monsterTimer >= 3.0f)
		{
			MonsterFactory::CreateMonster(m_scene, MONSTERS::MMM, { 0, 0 }, monsterRegionObject);
            monsterTimer = 0.0f;
		}
	}
	else
	{
		monsterTimer = 0.0f;
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
	MonsterFactory::CreateMonster(m_scene, MONSTERS::MMM, { 1600, 300 }, this);
}
