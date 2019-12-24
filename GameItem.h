/**********************************************************************
[[�Q�[���A�C�e���w�b�_�[(GameItem.h)]]
	��ҁF���c�@�^�K

	GameItem.cpp�Ɋւ���w�b�_�[
***********************************************************************/
#pragma once
//#include <Windows.h>
//#include <d3dx9.h>
#include "main.h"

#define ITEM_SIZE_X		(20.0f/2)	// �A�C�e���̃T�C�Y�w��(x)
#define ITEM_SIZE_Y		(20.0f/2)	// (y)

// �A�C�e���X�s�[�h
#define ITEM_SPD	(0.4f)

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------

// �e�N�X�`���ꂫ��^
enum ITEM_TEX_TYPE {
	NO_TEX,
	NANKA_TEX,
	MAX_TEXTYPE
};

// Ai�񋓌^
enum ITEM_AI_TYPE {
	NO_AI,

	MAX_AITYPE
};

// �A�C�e���f�[�^�\����
typedef struct ENTITY_ITEM {
	bool				isUse;								// �g�p�\���ǂ���
	LPDIRECT3DTEXTURE9	pTex;								// �A�C�e���̃e�N�X�`��
	D3DXVECTOR3			pos;								// �ʒu
	D3DXVECTOR3			rot;								// ��]
	D3DXVECTOR3			scl;								// �X�P�[���i����1.0f�j
	bool				(*UpdateEachGameItem)(ENTITY_ITEM*);// �A�C�e�����Ƃ̍X�V�֐��|�C���^
	bool				bPlus;								// ���̃A�C�e�����E������ڲ԰�Ƀv���X(true)�}�C�i�X(false)
}ENTITY_ITEM;
//---------------------------------------------------------------------
//	�v���g�^�C�v�錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------
void StartGameItemTime(void);
HRESULT InitGameItem(void);
void UpdateGameItem(void);
void DrawGameItem(void);
void UninitGameItem(void);