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

#include "Player.h"
#include "GameBG.h"
#include "Road.h"
#include "CreamRoad.h"

/**************************************
初期化処理
***************************************/
void GameScene::Init()
{
	sceneCamera = new Camera();
	Camera::SetMainCamera(sceneCamera);

	InitGameBG();
	InitPlayer();
	InitRoad();
	InitCreamRoad();
}

/**************************************
終了処理
***************************************/
void GameScene::Uninit()
{
	SAFE_DELETE(sceneCamera);

	UninitGameBG();
	UninitPlayer();
	UninitRoad();
	UninitCreamRoad();
}

/**************************************
更新処理
***************************************/
void GameScene::Update()
{
	sceneCamera->Update();

	UpdatePlayer();
	UpdateRoad();
	UpdateCreamRoad();

	//当たり判定
	ColliderManager::Instance()->CheckRoundRobin("Player", "CreamRoad");
}

/**************************************
描画処理
***************************************/
void GameScene::Draw()
{
	sceneCamera->Set();

	DrawGameBG();
	DrawRoad();
	DrawPlayer();
	DrawCreamRoad();
}
