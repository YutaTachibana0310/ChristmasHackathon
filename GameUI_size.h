//=============================================================================
//
// �X�R�A���� [GameUI_size.h]
// Author : ��n�@���l
//
//=============================================================================
#pragma once

// �}�N����`
#define UI_SIZE_NUM_GAME_FILE		_T("data/TEXTURE/UI/num1.png")	// �傫���p�̐����p
#define UI_SIZE_TEXT_GAME_FILE		_T("data/TEXTURE/UI/UI_size.png")	// �u�傫���v�̕����\���p

#define UI_SIZE_SIZE_W		(100)		// �e�N�X�`���T�C�Y
#define UI_SIZE_SIZE_H		(UI_SIZE_SIZE_W*2)		// ����
#define UI_SIZE_POS_X			(900)		// �|���S���̏����ʒuX
#define UI_SIZE_POS_Y			(20)		// ����
#define UI_SIZE_MAX			(999)		// �X�R�A�̍ő�l
#define UI_SIZE_DIGIT			(3)			// ����


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitUI_size(int type);
void UninitUI_size(void);
void UpdateUI_size(void);
void DrawUI_size(void);

void AddUI_size(int add);

