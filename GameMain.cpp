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
#include "TitleScene.h"

#include "Framework\Resource\FontManager.h"
#include "Framework\Sound\BackgroundMusic.h"

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
	sceneManager->Add(GameConfig::TitleScene, new TitleScene(renderTexture, renderSurface));

	BGM::Load("data/BGM/bgm_game1.wav", GameConfig::GameScene);
	BGM::Load("data/BGM/bgm_title.wav", GameConfig::TitleScene);

	sceneManager->ChangeScene(GameConfig::TitleScene);
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
