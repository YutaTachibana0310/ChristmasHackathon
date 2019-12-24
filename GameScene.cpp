//=====================================
//
//GameScene.cpp
//�@�\:�Q�[���V�[������
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "GameScene.h"

#include "Framework/Camera/Camera.h"

#include "Player.h"
#include "GameBG.h"

/**************************************
����������
***************************************/
void GameScene::Init()
{
	sceneCamera = new Camera();
	Camera::SetMainCamera(sceneCamera);

	InitGameBG();
	InitPlayer();
}

/**************************************
�I������
***************************************/
void GameScene::Uninit()
{
	SAFE_DELETE(sceneCamera);

	UninitGameBG();
	UninitPlayer();
}

/**************************************
�X�V����
***************************************/
void GameScene::Update()
{
	sceneCamera->Update();

	UpdatePlayer();
}

/**************************************
�`�揈��
***************************************/
void GameScene::Draw()
{
	sceneCamera->Set();

	DrawGameBG();
	DrawPlayer();
}
