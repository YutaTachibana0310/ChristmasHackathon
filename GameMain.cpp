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

#include "Framework\Resource\FontManager.h"

/**************************************
コンストラクタ
***************************************/
GameMain::GameMain(HINSTANCE hInstance, HWND hWnd) :
	BaseGame(hInstance, hWnd)
{
	FontManager::Instance()->LoadFont("data/FONT/Makinas-4-Square.otf");
	FontManager::Instance()->LoadFont("data/FONT/Text_cinecaption226.ttf");
	FontManager::Instance()->LoadFont("data/FONT/mplus-2c-heavy.ttf");
	FontManager::Instance()->LoadFont("data/FONT/mplus-2m-thin.ttf");

	sceneManager->Add(GameConfig::GameScene, new GameScene(renderTexture, renderSurface));

	sceneManager->ChangeScene(GameConfig::GameScene);
}

/**************************************
デストラクタ
***************************************/
GameMain::~GameMain()
{
	FontManager::Instance()->RemoveFont("data/FONT/Makinas-4-Square.otf");
	FontManager::Instance()->RemoveFont("data/FONT/Text_cinecaption226.ttf");
	FontManager::Instance()->RemoveFont("data/FONT/mplus-2c-heavy.ttf");
	FontManager::Instance()->RemoveFont("data/FONT/mplus-2m-thin.ttf");
}
