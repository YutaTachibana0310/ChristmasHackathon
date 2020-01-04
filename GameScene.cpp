//=====================================
//
//GameScene.cpp
//機能:ゲームシーン処理
//Author:GP12B332 21 立花雄太
//
//=====================================
#include "GameScene.h"
#include "GameItem.h"
#include "Framework/Camera/Camera.h"
#include "Framework\Collider\ColliderManager.h"
#include "Framework\Input\input.h"
#include "Framework\Transition\TransitionController.h"
#include "Framework\Core\SceneManager.h"
#include "Framework\Sound\BackgroundMusic.h"

#include "GameUI_size.h"
#include "GameUI_distance.h"
#include "Gameeffect.h"

#include "Player.h"
#include "GameBG.h"
#include "Road.h"
#include "CreamRoad.h"
#include "GameParticleManager.h"
#include "GameConfig.h"
#include "FinishTelop.h"

#include "title.h"

/**************************************
初期化処理
***************************************/
void GameScene::Init()
{
	sceneCamera = new Camera();
	Camera::SetMainCamera(sceneCamera);

	finishTelop = new FinishTelop();

	InitGameItem();
	StartGameItemTime();

	InitUI_size(0);
	InitUI_distance(0);
	InitEffect(0);

	InitGameBG();
	InitPlayer();
	InitRoad();
	InitCreamRoad();

	GameParticleManager::Instance()->Init();

	distance = 0.0f;
	inGame = true;

	int type = TransitionType::HexaPop;
	TransitionController::Instance()->SetTransition(true, TransitionType(type));

	BGM::FadeIn(GameConfig::GameScene, 1.0f, 60, false);
}

/**************************************
終了処理
***************************************/
void GameScene::Uninit()
{
	SAFE_DELETE(sceneCamera);
	SAFE_DELETE(finishTelop);

	UninitGameItem();

	UninitUI_size();
	UninitUI_distance();
	UninitEffect();

	UninitGameBG();
	UninitPlayer();
	UninitRoad();
	UninitCreamRoad();

	GameParticleManager::Instance()->Uninit();

	ColliderManager::Instance()->Clear();
}

/**************************************
更新処理
***************************************/
void GameScene::Update()
{
	sceneCamera->Update();

	UpdateGameItem();

	UpdatePlayer();
	UpdateRoad();
	UpdateCreamRoad();

	//当たり判定
	ColliderManager::Instance()->CheckRoundRobin("Player", "CreamRoad");
	ColliderManager::Instance()->CheckRoundRobin("Player", "Item");

	UpdateUI_size();
	UpdateUI_distance();

	UpdateEffect();

	GameParticleManager::Instance()->Update();
	
	if (inGame)
	{
		UpdatePlayer();
		UpdateRoad();
		UpdateCreamRoad();

		//当たり判定
		ColliderManager::Instance()->CheckRoundRobin("Player", "CreamRoad");

		UpdateUI_size();
		UpdateUI_distance();

		distance += 1.0f;
		AddUI_distance(1);

		float sizeCake = GetPlayerSize();
		AddUI_size((int)sizeCake);

		//60フレーム*30秒でゴール
		if (distance >= 60.0f * 30.0f&& inGame)
		{
			inGame = false;
			finishTelop->Set([this]()
			{
				OnFinishScene();
			});
		}
	}
}

/**************************************
描画処理
***************************************/
void GameScene::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	sceneCamera->Set();

	DrawGameBG();

	DrawRoad();
	DrawPlayer();

	DrawCreamRoad();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);


	DrawGameItem();

	GameParticleManager::Instance()->Draw();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	DrawUI_size();
	DrawUI_distance();
	DrawEffect();

	finishTelop->Draw();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

/**************************************
シーン終了時の処理
***************************************/
void GameScene::OnFinishScene()
{
	TransitionController::Instance()->SetTransition(false, TransitionType::HexaPop, []()
	{
		BGM::Fade(0.0f, 60, true);
		SceneManager::ChangeScene(GameConfig::TitleScene);
	});
}
