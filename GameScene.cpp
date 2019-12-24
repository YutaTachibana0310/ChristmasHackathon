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
#include "GameParticleManager.h"

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

	GameParticleManager::Instance()->Init();

	distance = 0.0f;
	inGame = true;
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

	GameParticleManager::Instance()->Uninit();
}

/**************************************
�X�V����
***************************************/
void GameScene::Update()
{
	sceneCamera->Update();

	UpdateEffect();
	GameParticleManager::Instance()->Update();
	
	if (inGame)
	{
		UpdatePlayer();
		UpdateRoad();
		UpdateCreamRoad();

		//�����蔻��
		ColliderManager::Instance()->CheckRoundRobin("Player", "CreamRoad");

		UpdateUI_size();
		UpdateUI_distance();

		distance += 1.0f;
		AddUI_distance(1);

		float sizeCake = GetPlayerSize();
		AddUI_size((int)sizeCake);

		if (distance >= 60.0f * 60.0f)
		{
			inGame = false;
		}
	}
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

	GameParticleManager::Instance()->Draw();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	DrawUI_size();
	DrawUI_distance();
	DrawEffect();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

}
