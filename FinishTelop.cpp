//=====================================
//
// FinishTelop.cpp
// �@�\:�t�B�j�b�V���e���b�v
// Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "FinishTelop.h"
#include "Framework/Tween/Tween.h"

/**************************************
�O���[�o���ϐ�
***************************************/

/**************************************
�R���X�g���N�^
***************************************/
FinishTelop::FinishTelop()
{
	LoadTexture("data/TEXTURE/UI/finishTelop.png");

	SetActive(false);
	SetSize({ 480.0f, 75.0f });
}

/**************************************
�f�X�g���N�^
***************************************/
FinishTelop::~FinishTelop()
{
}

/**************************************
�Z�b�g����
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
�Z�b�g����
***************************************/
void FinishTelop::OnFinishTelopIn()
{
	Tween::Move(*this, { -480.0f, SCREEN_CENTER_Y, 0.0f }, 60, EaseType::InCubic, onFinishTelopOut);
}
