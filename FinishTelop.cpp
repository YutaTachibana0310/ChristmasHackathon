//=====================================
//
// FinishTelop.cpp
// 機能:フィニッシュテロップ
// Author:GP12B332 21 立花雄太
//
//=====================================
#include "FinishTelop.h"
#include "Framework/Tween/Tween.h"

/**************************************
グローバル変数
***************************************/

/**************************************
コンストラクタ
***************************************/
FinishTelop::FinishTelop()
{
	LoadTexture("data/TEXTURE/UI/finishTelop.png");

	SetActive(false);
	SetSize({ 480.0f, 75.0f });
}

/**************************************
デストラクタ
***************************************/
FinishTelop::~FinishTelop()
{
}

/**************************************
セット処理
***************************************/
void FinishTelop::Set(std::function<void()> callback)
{
	SetActive(true);
	transform->SetPosition({ SCREEN_WIDTH + 480.0f, SCREEN_CENTER_Y, 0.0f });

	onFinishTelopOut = callback;

	Tween::Move(*this, { SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f }, 60, EaseType::OutCubic, [this]()
	{
		OnFinishTelopIn();
	});
}

/**************************************
セット処理
***************************************/
void FinishTelop::OnFinishTelopIn()
{
	Tween::Move(*this, { -480.0f, SCREEN_CENTER_Y, 0.0f }, 60, EaseType::InCubic, onFinishTelopOut);
}
