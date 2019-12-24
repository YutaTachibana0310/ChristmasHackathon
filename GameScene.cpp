//=====================================
//
//GameScene.cpp
//�@�\:�Q�[���V�[������
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "GameScene.h"

#include "Framework/Camera/Camera.h"
#include "Framework\Collider\ColliderManager.h"

#include "GameUI_size.h"
#include "GameUI_distance.h"
#include "Gameeffect.h"

#include "Player.h"
#include "GameBG.h"
#include "Road.h"
#include "CreamRoad.h"

#include "title.h"

/**************************************
����������
***************************************/
void GameScene::Init()
{
	sceneCamera = new Camera();
	Camera::SetMainCamera(sceneCamera);

	InitUI_size(0);
	InitUI_distance(0);
	InitEffect(0);

	InitGameBG();
	InitPlayer();
	InitRoad();
	InitCreamRoad();

	InitTitle(0);
}

/**************************************
�I������
***************************************/
void GameScene::Uninit()
{
	SAFE_DELETE(sceneCamera);

	UninitUI_size();
	UninitUI_distance();
	UninitEffect();

	UninitGameBG();
	UninitPlayer();
	UninitRoad();
	UninitCreamRoad();

	UninitTitle();
}

/**************************************
�X�V����
***************************************/
void GameScene::Update()
{
	sceneCamera->Update();

	UpdatePlayer();
	UpdateRoad();
	UpdateCreamRoad();

	//�����蔻��
	ColliderManager::Instance()->CheckRoundRobin("Player", "CreamRoad");

	UpdateUI_size();
	UpdateUI_distance();
	UpdateEffect();

	UpdateTitle();
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
	DrawCreamRoad();

	DrawUI_size();
	DrawUI_distance();
	DrawEffect();

	DrawTitle();
}
