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

/**************************************
�O���[�o���ϐ�
***************************************/
GameMain::GameMain(HINSTANCE hInstance, HWND hWnd) :
	BaseGame(hInstance, hWnd)
{
	sceneManager->Add(GameConfig::GameScene, new GameScene(renderTexture, renderSurface));

	sceneManager->ChangeScene(GameConfig::GameScene);
}
