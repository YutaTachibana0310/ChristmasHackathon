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
#include "GameSkybox.h"

/**************************************
初期化処理
***************************************/
void GameScene::Init()
{
	sceneCamera = new Camera();
	Camera::SetMainCamera(sceneCamera);
	InitGameItem();
	StartGameItemTime();
	InitSkybox();
}

/**************************************
終了処理
***************************************/
void GameScene::Uninit()
{
	SAFE_DELETE(sceneCamera);
	UninitGameItem();
	UninitSkybox();
}

/**************************************
更新処理
***************************************/
void GameScene::Update()
{
	sceneCamera->Update();
	UpdateGameItem();
	UpdateSkybox();
}

/**************************************
描画処理
***************************************/
void GameScene::Draw()
{
	sceneCamera->Set();
	DrawGameItem();
	DrawSkybox();
}
