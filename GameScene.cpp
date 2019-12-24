//=====================================
//
//GameScene.cpp
//�@�\:�Q�[���V�[������
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "GameScene.h"
#include "GameItem.h"
#include "Framework/Camera/Camera.h"

#include "Player.h"
#include "GameBG.h"
#include "Road.h"

/**************************************
����������
***************************************/
void GameScene::Init()
{
	sceneCamera = new Camera();
	Camera::SetMainCamera(sceneCamera);

	InitGameItem();
	StartGameItemTime();

	InitGameBG();
	InitPlayer();
	InitRoad();
}

/**************************************
�I������
***************************************/
void GameScene::Uninit()
{
	SAFE_DELETE(sceneCamera);

	UninitGameItem();

	UninitGameBG();
	UninitPlayer();
	UninitRoad();
}

/**************************************
�X�V����
***************************************/
void GameScene::Update()
{
	sceneCamera->Update();

	UpdateGameItem();

	UpdatePlayer();
	UpdateRoad();
}

/**************************************
�`�揈��
***************************************/
void GameScene::Draw()
{
	sceneCamera->Set();

	DrawGameBG();

	DrawRoad();
	DrawPlayer();
	DrawGameItem();
}
