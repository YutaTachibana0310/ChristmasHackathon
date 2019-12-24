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
#include "Framework\Collider\ColliderManager.h"

#include "GameUI_size.h"
#include "GameUI_distance.h"
#include "Gameeffect.h"

#include "Player.h"
#include "GameBG.h"
#include "Road.h"
#include "CreamRoad.h"
#include "GameParticleManager.h"

/**************************************
����������
***************************************/
void GameScene::Init()
{
	sceneCamera = new Camera();
	Camera::SetMainCamera(sceneCamera);

	InitGameItem();
	StartGameItemTime();

	InitUI_size(0);
	InitUI_distance(0);
	InitEffect(0);

	InitGameBG();
	InitPlayer();
	InitRoad();
	InitCreamRoad();

	GameParticleManager::Instance()->Init();
}

/**************************************
�I������
***************************************/
void GameScene::Uninit()
{
	SAFE_DELETE(sceneCamera);

	UninitGameItem();

	UninitUI_size();
	UninitUI_distance();
	UninitEffect();

	UninitGameBG();
	UninitPlayer();
	UninitRoad();
	UninitCreamRoad();

	GameParticleManager::Instance()->Uninit();
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
	UpdateCreamRoad();

	//�����蔻��
	ColliderManager::Instance()->CheckRoundRobin("Player", "CreamRoad");

	UpdateUI_size();
	UpdateUI_distance();
	UpdateEffect();

	GameParticleManager::Instance()->Update();
}

/**************************************
�`�揈��
***************************************/
void GameScene::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	sceneCamera->Set();

	DrawGameBG();

	DrawRoad();
	DrawPlayer();

	DrawCreamRoad();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);


	DrawGameItem();

	GameParticleManager::Instance()->Draw();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	DrawUI_size();
	DrawUI_distance();
	DrawEffect();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

}
