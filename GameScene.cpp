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
#include "Framework\Input\input.h"
#include "Framework\Transition\TransitionController.h"
#include "Framework\Core\SceneManager.h"
#include "Framework\Sound\BackgroundMusic.h"

#include "GameUI_size.h"
#include "GameUI_distance.h"
#include "Gameeffect.h"

#include "Player.h"
#include "GameBG.h"
#include "Road.h"
#include "CreamRoad.h"
#include "GameParticleManager.h"
#include "GameConfig.h"
#include "FinishTelop.h"

#include "title.h"

/**************************************
����������
***************************************/
void GameScene::Init()
{
	sceneCamera = new Camera();
	Camera::SetMainCamera(sceneCamera);

	finishTelop = new FinishTelop();

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

	distance = 0.0f;
	inGame = true;

	int type = TransitionType::HexaPop;
	TransitionController::Instance()->SetTransition(true, TransitionType(type));

	BGM::FadeIn(GameConfig::GameScene, 1.0f, 60, false);
}

/**************************************
�I������
***************************************/
void GameScene::Uninit()
{
	SAFE_DELETE(sceneCamera);
	SAFE_DELETE(finishTelop);

	UninitGameItem();

	UninitUI_size();
	UninitUI_distance();
	UninitEffect();

	UninitGameBG();
	UninitPlayer();
	UninitRoad();
	UninitCreamRoad();

	GameParticleManager::Instance()->Uninit();

	ColliderManager::Instance()->Clear();
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
	ColliderManager::Instance()->CheckRoundRobin("Player", "Item");

	UpdateUI_size();
	UpdateUI_distance();

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

		//60�t���[��*30�b�ŃS�[��
		if (distance >= 60.0f * 30.0f&& inGame)
		{
			inGame = false;
			finishTelop->Set([this]()
			{
				OnFinishScene();
			});
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

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);


	DrawGameItem();

	GameParticleManager::Instance()->Draw();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	DrawUI_size();
	DrawUI_distance();
	DrawEffect();

	finishTelop->Draw();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

/**************************************
�V�[���I�����̏���
***************************************/
void GameScene::OnFinishScene()
{
	TransitionController::Instance()->SetTransition(false, TransitionType::HexaPop, []()
	{
		BGM::Fade(0.0f, 60, true);
		SceneManager::ChangeScene(GameConfig::TitleScene);
	});
}
