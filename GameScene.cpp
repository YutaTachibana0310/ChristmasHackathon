//=====================================
//
//GameScene.cpp
//機能:ゲームシーン処理
//Author:GP12B332 21 立花雄太
//
//=====================================
#include "GameScene.h"

#include "Framework/Camera/Camera.h"
#include "Framework\Collider\ColliderManager.h"

#include "GameUI_size.h"
#include "GameUI_distance.h"
#include "Gameeffect.h"

#include "Player.h"
#include "GameBG.h"
#include "Road.h"
#include "CreamRoad.h"
#include "GameParticleManager.h"

/**************************************
初期化処理
***************************************/
void GameScene::Init()
{
	sceneCamera = new Camera();
	Camera::SetMainCamera(sceneCamera);

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
}

/**************************************
終了処理
***************************************/
void GameScene::Uninit()
{
	SAFE_DELETE(sceneCamera);

	UninitUI_size();
	UninitUI_distance();
	UninitEffect();

	UninitGameBG();
	UninitPlayer();
	UninitRoad();
	UninitCreamRoad();

	GameParticleManager::Instance()->Uninit();
}

/**************************************
更新処理
***************************************/
void GameScene::Update()
{
	sceneCamera->Update();

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

		if (distance >= 60.0f * 60.0f)
		{
			inGame = false;
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

	GameParticleManager::Instance()->Draw();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	DrawUI_size();
	DrawUI_distance();
	DrawEffect();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

}
