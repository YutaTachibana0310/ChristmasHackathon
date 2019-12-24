//=====================================
//
//GameScene.cpp
//�@�\:�Q�[���V�[������
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "GameScene.h"

#include "Framework/Camera/Camera.h"
#include "GameSkybox.h"
#include "GameUI_size.h"
#include "GameUI_distance.h"
#include "Gameeffect.h"


/**************************************
����������
***************************************/
void GameScene::Init()
{
	sceneCamera = new Camera();
	Camera::SetMainCamera(sceneCamera);

	InitSkybox();

	InitUI_size(0);
	InitUI_distance(0);
	InitEffect(0);
}

/**************************************
�I������
***************************************/
void GameScene::Uninit()
{
	SAFE_DELETE(sceneCamera);

	UninitSkybox();

	UninitUI_size();
	UninitUI_distance();
	UninitEffect();
}

/**************************************
�X�V����
***************************************/
void GameScene::Update()
{
	sceneCamera->Update();

	UpdateSkybox();

	UpdateUI_size();
	UpdateUI_distance();
	UpdateEffect();
}

/**************************************
�`�揈��
***************************************/
void GameScene::Draw()
{
	sceneCamera->Set();

	DrawSkybox();

	DrawUI_size();
	DrawUI_distance();
	DrawEffect();
}
