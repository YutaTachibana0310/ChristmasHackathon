//=============================================================================
//
// ���U���g��ʏ��� [title.cpp]
// Author : ��n�@���l
//
//=============================================================================
#include "main.h"
#include "title.h"
#include <stdio.h>


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTitle(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTitle = NULL;		// �e�N�X�`���ւ̃|�C���^

VERTEX_2D				g_vertexWkTitle[NUM_VERTEX];			// ���_���i�[���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitle(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_TITLE,				// �t�@�C���̖��O
			&g_pD3DTextureTitle);		// �ǂݍ��ރ������[

	}

	// ���_���̍쐬
	MakeVertexTitle();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
{
	if (g_pD3DTextureTitle != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureTitle->Release();
		g_pD3DTextureTitle = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTitle(void)
{
	//if (GetKeyboardTrigger(DIK_RETURN))
	//{// Enter��������A�X�e�[�W��؂�ւ���
	//	InitGame();				// �Q�[���̍ď���������
	//	SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_sample000);

	//}
	//else if (IsButtonTriggered(0, BUTTON_B))
	//{
	//	InitGame();				// �Q�[���̍ď���������
	//	SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_sample000);
	//}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureTitle);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitle, sizeof(VERTEX_2D));

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexTitle(void)
{
	// ���_���W�̐ݒ�
	g_vertexWkTitle[0].vtx = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y, 0.0f);
	g_vertexWkTitle[1].vtx = D3DXVECTOR3(TITLE_POS_X + SCREEN_WIDTH, TITLE_POS_Y, 0.0f);
	g_vertexWkTitle[2].vtx = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y + SCREEN_HEIGHT, 0.0f);
	g_vertexWkTitle[3].vtx = D3DXVECTOR3(TITLE_POS_X + SCREEN_WIDTH, TITLE_POS_Y + SCREEN_HEIGHT, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkTitle[0].rhw =
		g_vertexWkTitle[1].rhw =
		g_vertexWkTitle[2].rhw =
		g_vertexWkTitle[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkTitle[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkTitle[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitle[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitle[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitle[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

