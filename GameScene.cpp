//=====================================
//
//GameScene.cpp
//機能:ゲームシーン処理
//Author:GP12B332 21 立花雄太
//
//=====================================
#include "GameScene.h"

#include "Framework/Camera/Camera.h"

#include "GameUI_size.h"
#include "GameUI_distance.h"
#include "Gameeffect.h"

#include "Player.h"
#include "GameBG.h"
#include "Road.h"

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
}

/**************************************
更新処理
***************************************/
void GameScene::Update()
{
	sceneCamera->Update();

	UpdatePlayer();
	UpdateRoad();

	UpdateUI_size();
	UpdateUI_distance();
	UpdateEffect();
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

	DrawUI_size();
	DrawUI_distance();
	DrawEffect();
}
