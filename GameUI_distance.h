//=============================================================================
//
// �X�R�A���� [GameUI_distance.h]
// Author : ��n�@���l
//
//=============================================================================
#pragma once

// �}�N����`
#define UI_DISTANCE_NUM_GAME_FILE		_T("data/TEXTURE/UI/num1.png")	// �T���v���p�摜
#define UI_DISTANCE_TEXT_GAME_FILE		_T("data/TEXTURE/UI/UI_meter.png")	// �T���v���p�摜

#define UI_DISTANCE_SIZE_W		(100)		// �e�N�X�`���T�C�Y
#define UI_DISTANCE_SIZE_H		(UI_DISTANCE_SIZE_W*2)		// ����
#define UI_DISTANCE_POS_X			(1500)		// �|���S���̏����ʒuX
#define UI_DISTANCE_POS_Y			(800)		// ����
#define UI_DISTANCE_MAX			(99999)		// �X�R�A�̍ő�l
#define UI_DISTANCE_DIGIT			(4)			// ����


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitUI_distance(int type);
void UninitUI_distance(void);
void UpdateUI_distance(void);
void DrawUI_distance(void);

void AddUI_distance(int add);

