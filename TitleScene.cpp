//=====================================
//
//TitleScene.cpp
//機能:タイトルシーン
//Author:GP12B332 21 立花雄太
//
//=====================================
#include "TitleScene.h"
#include "Framework/Tool/DebugWindow.h"
#include "Framework\Transition\TransitionController.h"
#include "Framework\Input\input.h"
#include "Framework\Core\SceneManager.h"
#include "GameConfig.h"

/**************************************
初期化処理
***************************************/
void TitleScene::Init()
{
	//カメラ作成
	sceneCamera = new Camera();
	Camera::SetMainCamera(sceneCamera);

	TransitionController::Instance()->SetTransition(true, TransitionType::HexaPop);
}

/**************************************
終了処理
***************************************/
void TitleScene::Uninit()
{
	SAFE_DELETE(sceneCamera);
}

/**************************************
更新処理
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
描画処理
***************************************/
void TitleScene::Draw()
{
	Debug::Log("CurrentScene:Title");
}
