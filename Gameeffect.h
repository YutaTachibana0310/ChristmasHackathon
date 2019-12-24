//=============================================================================
//
// �o���b�g���� [Gameeffect.h]
// Author : ��n�@���l
//
//=============================================================================
#pragma once


// �}�N����`
#define EFFECT_UP_TEXTURE			_T("data/TEXTURE/Particle/pipo-sweetseffect001_480.png")	// �摜
#define EFFECT_DOWN_TEXTURE			_T("data/TEXTURE/Particle/pipo-btleffect102h.png")	// �摜

#define EFFECT_TEXTURE_SIZE_X	(600/2)		// �e�N�X�`���T�C�Y
#define EFFECT_TEXTURE_SIZE_Y	(600/2)		// ����

#define EFFECT_UP_TEXTURE_PATTERN_DIVIDE_X	(5)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define EFFECT_UP_TEXTURE_PATTERN_DIVIDE_Y	(3)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define EFFECT_UP_ANIM_PATTERN_NUM			(EFFECT_UP_TEXTURE_PATTERN_DIVIDE_X*EFFECT_UP_TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define EFFECT_UP_TIME_ANIMATION			(4)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define EFFECT_DOWN_TEXTURE_PATTERN_DIVIDE_X	(10)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define EFFECT_DOWN_TEXTURE_PATTERN_DIVIDE_Y	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define EFFECT_DOWN_ANIM_PATTERN_NUM			(EFFECT_DOWN_TEXTURE_PATTERN_DIVIDE_X*EFFECT_DOWN_TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define EFFECT_DOWN_TIME_ANIMATION			(4)	// �A�j���[�V�����̐؂�ւ��J�E���g


#define EFFECT_MAX						(10)	// �o���b�g�̍ő吔

//*****************************************************************************
// �\���̐錾
//*****************************************************************************

typedef struct	// �o���b�g�\����
{
	bool			use;						// true:�g�p  false:���g�p
	int				type;						// �G�t�F�N�g�̎��
	D3DXVECTOR3		pos;						// �|���S���̈ړ���

	int				PatternAnim;				// �A�j���[�V�����p�^�[���i���o�[
	int				CountAnim;					// �A�j���[�V�����J�E���g

	//LPDIRECT3DTEXTURE9	Texture;				// �e�N�X�`�����
	VERTEX_2D		vertexWk[NUM_VERTEX];		// ���_���i�[���[�N

} EFFECT;

enum EFFCT_GAME{
	UP,
	DOWN,
	EFFCT_GAME_MAX,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEffect(int type);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, int type);
EFFECT *GetEffect(void);


