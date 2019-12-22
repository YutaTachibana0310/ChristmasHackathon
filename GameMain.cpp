//=====================================
//
//GameMain.cpp
//機能:ゲームメイン機能
//Author:GP12B332 21 立花雄太
//
//=====================================
#include "GameMain.h"

#include "GameConfig.h"
#include "GameScene.h"

/**************************************
グローバル変数
***************************************/
GameMain::GameMain(HINSTANCE hInstance, HWND hWnd) :
	BaseGame(hInstance, hWnd)
{
	sceneManager->Add(GameConfig::GameScene, new GameScene(renderTexture, renderSurface));

	sceneManager->ChangeScene(GameConfig::GameScene);
}
