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

/**************************************
����������
***************************************/
void TitleScene::Init()
{
	//�J�����쐬
	sceneCamera = new Camera();
	Camera::SetMainCamera(sceneCamera);

	TransitionController::Instance()->SetTransition(true, TransitionType::HexaPop);
}

/**************************************
�I������
***************************************/
void TitleScene::Uninit()
{
	SAFE_DELETE(sceneCamera);
}

/**************************************
�X�V����
***************************************/
void TitleScene::Update()
{
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
	Debug::Log("CurrentScene:Title");
}
