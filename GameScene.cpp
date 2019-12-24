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
#include "GameSkybox.h"

/**************************************
����������
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
�I������
***************************************/
void GameScene::Uninit()
{
	SAFE_DELETE(sceneCamera);
	UninitGameItem();
	UninitSkybox();
}

/**************************************
�X�V����
***************************************/
void GameScene::Update()
{
	sceneCamera->Update();
	UpdateGameItem();
	UpdateSkybox();
}

/**************************************
�`�揈��
***************************************/
void GameScene::Draw()
{
	sceneCamera->Set();
	DrawGameItem();
	DrawSkybox();
}
