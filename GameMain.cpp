//=====================================
//
//GameMain.cpp
//�@�\:�Q�[�����C���@�\
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "GameMain.h"

#include "GameConfig.h"
#include "GameScene.h"

#include "Framework\Resource\FontManager.h"

/**************************************
�R���X�g���N�^
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
�f�X�g���N�^
***************************************/
GameMain::~GameMain()
{
	FontManager::Instance()->RemoveFont("data/FONT/Makinas-4-Square.otf");
	FontManager::Instance()->RemoveFont("data/FONT/Text_cinecaption226.ttf");
	FontManager::Instance()->RemoveFont("data/FONT/mplus-2c-heavy.ttf");
	FontManager::Instance()->RemoveFont("data/FONT/mplus-2m-thin.ttf");
}
