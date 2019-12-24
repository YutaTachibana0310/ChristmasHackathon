//=============================================================================
//
// �X�R�A���� [GameUI_size.cpp]
// Author : ��n�@���l
//
//=============================================================================
#include "main.h"
#include "GameUI_size.h"
#include <stdio.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexUI_size(void);
void SetTextureUI_size(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTexture_num = NULL;			// �e�N�X�`���ւ̃|���S��
static VERTEX_2D				g_vertexWk_num[UI_SIZE_DIGIT][NUM_VERTEX];	// ���_���i�[���[�N

static LPDIRECT3DTEXTURE9		g_pD3DTexture_text = NULL;			// �e�N�X�`���ւ̃|���S��
static VERTEX_2D				g_vertexWk_text[NUM_VERTEX];	// ���_���i�[���[�N


static D3DXVECTOR3				g_posUI_size;						// �|���S���̈ړ���

static int						g_UI_size;						// �X�R�A

//=============================================================================
// ����������
//=============================================================================
HRESULT InitUI_size(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���[�̏��������s���H

	//�e�L�X�g�̂ق�
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			UI_SIZE_TEXT_GAME_FILE,					// �t�@�C���̖��O
			&g_pD3DTexture_text);					// �ǂݍ��ރ������̃|�C���^
	}

	//num�̂ق�
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			UI_SIZE_NUM_GAME_FILE,					// �t�@�C���̖��O
			&g_pD3DTexture_num);					// �ǂݍ��ރ������̃|�C���^
	}

	g_posUI_size = D3DXVECTOR3((float)UI_SIZE_POS_X, (float)UI_SIZE_POS_Y, 0.0f);
	g_UI_size = 0;

	// ���_���̍쐬
	MakeVertexUI_size();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitUI_size(void)
{
	if (g_pD3DTexture_text != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTexture_text->Release();
		g_pD3DTexture_text = NULL;
	}

	if (g_pD3DTexture_num != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DTexture_num->Release();
		g_pD3DTexture_num = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateUI_size(void)
{
	// ���t���[�����s����鏈�����L�q����

	g_UI_size += 1;
	SetTextureUI_size();

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawUI_size(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i;

	// ������
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTexture_text);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &g_vertexWk_text[0], sizeof(VERTEX_2D));


	// ������
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTexture_num);

	// �X�R�A
	for (i = 0; i < UI_SIZE_DIGIT; i++)
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &g_vertexWk_num[i][0], sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexUI_size(void)
{
	int i;
	float habaX = UI_SIZE_SIZE_W;	// �����̉���

	// �e�L�X�g��
	// ���_���W�̐ݒ�
	g_vertexWk_text[0].vtx = D3DXVECTOR3(-habaX * UI_SIZE_DIGIT*2 + g_posUI_size.x, g_posUI_size.y*1.2f, 0.0f);
	g_vertexWk_text[1].vtx = D3DXVECTOR3(-habaX * UI_SIZE_DIGIT + g_posUI_size.x + UI_SIZE_SIZE_W, g_posUI_size.y*1.2f, 0.0f);
	g_vertexWk_text[2].vtx = D3DXVECTOR3(-habaX * UI_SIZE_DIGIT*2 + g_posUI_size.x, g_posUI_size.y*1.2f + (float)UI_SIZE_SIZE_H*0.8f , 0.0f);
	g_vertexWk_text[3].vtx = D3DXVECTOR3(-habaX * UI_SIZE_DIGIT + g_posUI_size.x + UI_SIZE_SIZE_W, g_posUI_size.y*1.2f + (float)UI_SIZE_SIZE_H*0.8f , 0.0f);

	// rhw�̐ݒ�
	g_vertexWk_text[0].rhw =
		g_vertexWk_text[1].rhw =
		g_vertexWk_text[2].rhw =
		g_vertexWk_text[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWk_text[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk_text[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk_text[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk_text[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWk_text[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWk_text[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWk_text[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWk_text[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// ��������������
	for (i = 0; i < UI_SIZE_DIGIT; i++)
	{
		// ���_���W�̐ݒ�
		g_vertexWk_num[i][0].vtx.x = -habaX * i + g_posUI_size.x;
		g_vertexWk_num[i][0].vtx.y = g_posUI_size.y;
		g_vertexWk_num[i][0].vtx.z = 0.0f;
		g_vertexWk_num[i][1].vtx.x = -habaX * i + g_posUI_size.x + UI_SIZE_SIZE_W;
		g_vertexWk_num[i][1].vtx.y = g_posUI_size.y;
		g_vertexWk_num[i][1].vtx.z = 0.0f;
		g_vertexWk_num[i][2].vtx.x = -habaX * i + g_posUI_size.x;
		g_vertexWk_num[i][2].vtx.y = g_posUI_size.y + UI_SIZE_SIZE_H;
		g_vertexWk_num[i][2].vtx.z = 0.0f;
		g_vertexWk_num[i][3].vtx.x = -habaX * i + g_posUI_size.x + UI_SIZE_SIZE_W;
		g_vertexWk_num[i][3].vtx.y = g_posUI_size.y + UI_SIZE_SIZE_H;
		g_vertexWk_num[i][3].vtx.z = 0.0f;

		// rhw�̐ݒ�
		g_vertexWk_num[i][0].rhw =
			g_vertexWk_num[i][1].rhw =
			g_vertexWk_num[i][2].rhw =
			g_vertexWk_num[i][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		g_vertexWk_num[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWk_num[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWk_num[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWk_num[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		g_vertexWk_num[i][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		g_vertexWk_num[i][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		g_vertexWk_num[i][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		g_vertexWk_num[i][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}
//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetTextureUI_size(void)
{
	int i;
	int number = g_UI_size;
	int irokae = g_UI_size;

	if (irokae > 255)
	{
		irokae = 255;
	}

	for (i = 0; i < UI_SIZE_DIGIT; i++)
	{
		// �e�N�X�`�����W�̐ݒ�
		float x = (float)(number % 10);
		g_vertexWk_num[i][0].tex = D3DXVECTOR2(0.1f * x, 0.0f);
		g_vertexWk_num[i][1].tex = D3DXVECTOR2(0.1f * (x + 1), 0.0f);
		g_vertexWk_num[i][2].tex = D3DXVECTOR2(0.1f * x, 1.0f);
		g_vertexWk_num[i][3].tex = D3DXVECTOR2(0.1f * (x + 1), 1.0f);
		number /= 10;

		// ���ˌ��̐ݒ�
		g_vertexWk_num[i][0].diffuse = D3DCOLOR_RGBA(255, 255 - irokae, 255 - irokae, 255);
		g_vertexWk_num[i][1].diffuse = D3DCOLOR_RGBA(255, 255 - irokae, 255 - irokae, 255);
		g_vertexWk_num[i][2].diffuse = D3DCOLOR_RGBA(255, 255 - irokae, 255 - irokae, 255);
		g_vertexWk_num[i][3].diffuse = D3DCOLOR_RGBA(255, 255 - irokae, 255 - irokae, 255);

	}

}
//=============================================================================
// HP�f�[�^���Z�b�g����
// ����:add :�ǉ�����_���B�}�C�i�X���\�A�������ȂǂɁB
//=============================================================================
void AddUI_size(int add)
{
	g_UI_size += add;
	if (g_UI_size > UI_SIZE_MAX)
	{
		g_UI_size = UI_SIZE_MAX;
	}

}