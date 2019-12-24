//=============================================================================
//
// �o���b�g���� [Gameeffect.cpp]
// Author : ��n�@���l
//
//=============================================================================
#include "main.h"
#include "Gameeffect.h"
#include <stdio.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexEffect(int no);
void SetTextureEffect(int no, int cntPattern);
void SetVertexEffect(int no);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureEffect_UP = NULL;		// �e�N�X�`���ւ̃|���S��
static LPDIRECT3DTEXTURE9		g_pD3DTextureEffect_DOWN = NULL;		// �e�N�X�`���ւ̃|���S��

static EFFECT					GameeffectWk[EFFECT_MAX];			// �o���b�g�\����

int test;	//�e�X�g�p�J�E���g
//=============================================================================
// ����������
//=============================================================================
HRESULT InitEffect(int type)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();

	// UP�p�e�N�X�`���̓ǂݍ���
	if (type == 0)
	{
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			EFFECT_UP_TEXTURE,						// �t�@�C���̖��O
			&g_pD3DTextureEffect_UP);							// �ǂݍ��ރ������̃|�C���^
	}

	// DOWN�p�e�N�X�`���̓ǂݍ���
	if (type == 0)
	{
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			EFFECT_DOWN_TEXTURE,						// �t�@�C���̖��O
			&g_pD3DTextureEffect_DOWN);							// �ǂݍ��ރ������̃|�C���^
	}

	// �o���b�g�̏���������
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		GameeffectWk[i].use = false;								// ���g�p�i���˂���Ă��Ȃ��e�j
		GameeffectWk[i].pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);		// ���W�f�[�^��������
		GameeffectWk[i].PatternAnim = 0;							// �A�j���p�^�[���ԍ��������_���ŏ�����
		GameeffectWk[i].CountAnim = 0;								// �A�j���J�E���g��������

		MakeVertexEffect(i);									// ���_���̍쐬
	}


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEffect(void)
{
	if (g_pD3DTextureEffect_UP != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTextureEffect_UP->Release();
		g_pD3DTextureEffect_UP = NULL;
	}

	if (g_pD3DTextureEffect_DOWN != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTextureEffect_DOWN->Release();
		g_pD3DTextureEffect_DOWN = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEffect(void)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (GameeffectWk[i].use == true)		// �g�p���Ă����ԂȂ�X�V����
		{
			// �A�j���[�V����
			GameeffectWk[i].CountAnim++;

			switch (GameeffectWk[i].type)
			{
			case UP:
				if ((GameeffectWk[i].CountAnim % EFFECT_UP_TIME_ANIMATION) == 0)
				{
					// �p�^�[���̐؂�ւ�
					GameeffectWk[i].PatternAnim = (GameeffectWk[i].PatternAnim + 1) % EFFECT_UP_ANIM_PATTERN_NUM;

					// �e�N�X�`�����W��ݒ�
					SetTextureEffect(i, GameeffectWk[i].PatternAnim);
				}
				SetVertexEffect(i);				// �ړ���̍��W�Œ��_��ݒ�

				if (GameeffectWk[i].CountAnim >= EFFECT_UP_TIME_ANIMATION * EFFECT_UP_ANIM_PATTERN_NUM)
				{
					GameeffectWk[i].use = false;
					GameeffectWk[i].CountAnim = 0;
				}
				break;

			case DOWN:
				if ((GameeffectWk[i].CountAnim % EFFECT_DOWN_TIME_ANIMATION) == 0)
				{
					// �p�^�[���̐؂�ւ�
					GameeffectWk[i].PatternAnim = (GameeffectWk[i].PatternAnim + 1) % EFFECT_DOWN_ANIM_PATTERN_NUM;

					// �e�N�X�`�����W��ݒ�
					SetTextureEffect(i, GameeffectWk[i].PatternAnim);
				}
				SetVertexEffect(i);				// �ړ���̍��W�Œ��_��ݒ�

				if (GameeffectWk[i].CountAnim >= EFFECT_DOWN_TIME_ANIMATION * EFFECT_DOWN_ANIM_PATTERN_NUM)
				{
					GameeffectWk[i].use = false;
					GameeffectWk[i].CountAnim = 0;
				}
				break;

			default:
				break;
			}
		}
	}

	test++;

	if (test < 121)
	{
		if (test % 120 == 0)
		{
			SetEffect(D3DXVECTOR3(500.0f, 500.0f, 500.0f), UP);
		}
		if (test % 60 == 0)
		{
			SetEffect(D3DXVECTOR3(800.0f, 800.0f, 800.0f), DOWN);
		}
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (GameeffectWk[i].use == true)			// �g�p���Ă����ԂȂ�X�V����
		{
			switch (GameeffectWk[i].type)	// �G�t�F�N�g��type�ɉ����ĕ\������e�N�X�`����ς���
			{
			case UP:
				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pD3DTextureEffect_UP);

				// �|���S���̕`��
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, GameeffectWk[i].vertexWk, sizeof(VERTEX_2D));
				break;

			case DOWN:
				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pD3DTextureEffect_DOWN);

				// �|���S���̕`��
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, GameeffectWk[i].vertexWk, sizeof(VERTEX_2D));
				break;


			default:
				break;
			}
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexEffect(int i)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	SetVertexEffect(i);

	// rhw�̐ݒ�
	GameeffectWk[i].vertexWk[0].rhw =
		GameeffectWk[i].vertexWk[1].rhw =
		GameeffectWk[i].vertexWk[2].rhw =
		GameeffectWk[i].vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	GameeffectWk[i].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	GameeffectWk[i].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	GameeffectWk[i].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	GameeffectWk[i].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	GameeffectWk[i].vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	GameeffectWk[i].vertexWk[1].tex = D3DXVECTOR2(1.0f / EFFECT_UP_TEXTURE_PATTERN_DIVIDE_X, 0.0f);
	GameeffectWk[i].vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / EFFECT_UP_TEXTURE_PATTERN_DIVIDE_Y);
	GameeffectWk[i].vertexWk[3].tex = D3DXVECTOR2(1.0f / EFFECT_UP_TEXTURE_PATTERN_DIVIDE_X, 1.0f / EFFECT_UP_TEXTURE_PATTERN_DIVIDE_Y);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureEffect(int i, int cntPattern)
{
	int x;
	int y;
	float sizeX;
	float sizeY;

	switch (GameeffectWk[i].type)	// �G�t�F�N�g��type�ɉ����ĕ\������e�N�X�`����ς���
	{
	case UP:
		// �e�N�X�`�����W�̐ݒ�
		 x = cntPattern % EFFECT_UP_TEXTURE_PATTERN_DIVIDE_X;
		 y = cntPattern / EFFECT_UP_TEXTURE_PATTERN_DIVIDE_X;
		 sizeX = 1.0f / EFFECT_UP_TEXTURE_PATTERN_DIVIDE_X;
		 sizeY = 1.0f / EFFECT_UP_TEXTURE_PATTERN_DIVIDE_Y;

		GameeffectWk[i].vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		GameeffectWk[i].vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		GameeffectWk[i].vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		GameeffectWk[i].vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		break;

	case DOWN:
		// �e�N�X�`�����W�̐ݒ�
		x = cntPattern % EFFECT_DOWN_TEXTURE_PATTERN_DIVIDE_X;
		y = cntPattern / EFFECT_DOWN_TEXTURE_PATTERN_DIVIDE_X;
		 sizeX = 1.0f / EFFECT_DOWN_TEXTURE_PATTERN_DIVIDE_X;
		 sizeY = 1.0f / EFFECT_DOWN_TEXTURE_PATTERN_DIVIDE_Y;

		GameeffectWk[i].vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		GameeffectWk[i].vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		GameeffectWk[i].vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		GameeffectWk[i].vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		break;

	}
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexEffect(int i)
{
	//���_���W�̐ݒ�
	GameeffectWk[i].vertexWk[0].vtx.x = GameeffectWk[i].pos.x - EFFECT_TEXTURE_SIZE_X;
	GameeffectWk[i].vertexWk[0].vtx.y = GameeffectWk[i].pos.y - EFFECT_TEXTURE_SIZE_Y;
	GameeffectWk[i].vertexWk[0].vtx.z = 0.0f;

	GameeffectWk[i].vertexWk[1].vtx.x = GameeffectWk[i].pos.x + EFFECT_TEXTURE_SIZE_X;
	GameeffectWk[i].vertexWk[1].vtx.y = GameeffectWk[i].pos.y - EFFECT_TEXTURE_SIZE_Y;
	GameeffectWk[i].vertexWk[1].vtx.z = 0.0f;

	GameeffectWk[i].vertexWk[2].vtx.x = GameeffectWk[i].pos.x - EFFECT_TEXTURE_SIZE_X;
	GameeffectWk[i].vertexWk[2].vtx.y = GameeffectWk[i].pos.y + EFFECT_TEXTURE_SIZE_Y;
	GameeffectWk[i].vertexWk[2].vtx.z = 0.0f;

	GameeffectWk[i].vertexWk[3].vtx.x = GameeffectWk[i].pos.x + EFFECT_TEXTURE_SIZE_X;
	GameeffectWk[i].vertexWk[3].vtx.y = GameeffectWk[i].pos.y + EFFECT_TEXTURE_SIZE_Y;
	GameeffectWk[i].vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// �o���b�g�̔��ːݒ�@type��UP�œ��_�G�t�F�N�g�ADOWN�Ō��_�G�t�F�N�g
//=============================================================================
void SetEffect(D3DXVECTOR3 pos, int type)
{
	// �������g�p�̒e�����������甭�˂��Ȃ�( =����ȏ㌂�ĂȂ����Ď� )
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (GameeffectWk[i].use == false)		// ���g�p��Ԃ̃o���b�g��������
		{
			GameeffectWk[i].use = true;			// �g�p��Ԃ֕ύX����
			GameeffectWk[i].pos = pos;			// ���W���Z�b�g
			GameeffectWk[i].type = type;		// �e�̎�ނ��Z�b�g


			//// SE�Đ�
			//PlaySound(SOUND_LABEL_SE_shot000);
			return;							// 1���Z�b�g�����̂ŏI������
		}
	}
}

//=============================================================================
// �G�l�~�[�擾�֐�
//=============================================================================
EFFECT *GetEffect(void)
{
	return(&GameeffectWk[0]);
}

