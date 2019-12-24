//=====================================
//
//TitleScene.cpp
//�@�\:�^�C�g���V�[��
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "TitleScene.h"
#include "Framework/Tool/DebugWindow.h"
#include "Framework\Transition\TransitionController.h"
#include "Framework\Input\input.h"
#include "Framework\Core\SceneManager.h"
#include "GameConfig.h"

#include "title.h"

/**************************************
����������
***************************************/
void TitleScene::Init()
{
	//�J�����쐬
	sceneCamera = new Camera();
	Camera::SetMainCamera(sceneCamera);

	InitTitle(0);

	TransitionController::Instance()->SetTransition(true, TransitionType::HexaPop);
}

/**************************************
�I������
***************************************/
void TitleScene::Uninit()
{
	UninitTitle();
	SAFE_DELETE(sceneCamera);
}

/**************************************
�X�V����
***************************************/
void TitleScene::Update()
{
	UpdateTitle();

	if (Keyboard::GetTrigger(DIK_RETURN))
	{
		TransitionController::Instance()->SetTransition(false, TransitionType::HexaPop, []()
		{
			SceneManager::ChangeScene(GameConfig::GameScene);
		});
	}
}

/**************************************
�`�揈��
***************************************/
void TitleScene::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	DrawTitle();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}
