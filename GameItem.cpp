/**********************************************************************
[[���C���v���O����(GameItem.cpp)]]
	��ҁF���c�@�^�K

	�Q�[���A�C�e���Ɋւ���v���O����
***********************************************************************/
#include "GameItem.h"
#include "GameItem_Ai.h"
#include "CSV_Reader.h"

//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------
// �Q�b�g�f�o�C�X�̊ȗ���
#define D3DDEVICE(p)	LPDIRECT3DDEVICE9 (p)=GetDevice();

// �e�[�u���A�h���X
#define ITEM_TBL	_T("data/GIMMICK/GameItemTbl.csv")

// �������(���j
#define POP_Z		(100.0f)

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------


typedef struct GAME_ITEM_DATA {
	LPDIRECT3DTEXTURE9		pTex[MAX_TEXTYPE];	// �A�C�e���Ŏg�p���鑍�e�N�X�`��
	LPDIRECT3DVERTEXBUFFER9	pVtx;				// �A�C�e���Ŏg�p�����ł��낤���_�l
	ENTITY_ITEM			*top_pt;			// �J�n�|�C���^	
	ENTITY_ITEM			*bot_pt;			// �I�[�|�C���^			
	CSV_FILE			*ItemPopTbl;		// CSV�̃e�[�u��
	int					TblIdx;				// �Ō�ɏo���������e�[�u���̍s
	DWORD				tItv;				// �O��o�������Ƃ�����̂�����
	DWORD				tLast;				// �O�t���[�����̎���
	void				(*Update)(void);	// �X�V�֐��|�C���^
}GAME_ITEM_DATA;

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------
void UpdateGameItemBeforeGameStart(void);
void UpdateGameItemAfterGameStart(void);
void SetItem(float X, ITEM_AI_TYPE Ai, ITEM_TEX_TYPE Tex, bool bPlus);
void *ReferenceItemAI(ITEM_AI_TYPE Ai);
//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------
static GAME_ITEM_DATA *g_pItemData = NULL;


/*=====================================================================
�Q�[���A�C�e���������֐�
�߂�l�FHRESULT
����:void
=====================================================================*/
HRESULT InitGameItem(void)
{
	D3DDEVICE(pDevice);	// �f�o�C�X
	HRESULT hr = S_OK;	// ���ʕ\��
	// �e�N�X�`���̃A�h���X
	const char *const filename[MAX_TEXTYPE] =
	{
		_T("Data/TEXTURE/Item/woman.png"),
		_T("Data/TEXTURE/Item/woman.png"),
	};

	// �k���`�F�b�N
	if (g_pItemData != NULL)
	{
		MessageBox(NULL, _T("�������k���`�F�b�N"), _T("GAMEITEM"), NULL);
		return E_FAIL;
	}

	// �A�C�e���̃�����������
	g_pItemData = (GAME_ITEM_DATA*)calloc(1, sizeof(GAME_ITEM_DATA));

	// �e�N�X�`���̊m��
	for (int i = 0; i < MAX_TEXTYPE; i++)
	{
		// �ǂ݂���
		hr = D3DXCreateTextureFromFile(pDevice, filename[i], &g_pItemData->pTex[i]);

		// �G���[
		if (FAILED(hr))
		{
			MessageBox(NULL, _T("�������e�N�X�`���`�F�b�N"), _T("GAMEITEM"), NULL);
			return E_FAIL;
		}
	}

	// ���_�̍쐬
	{
		// ���_�̍쐬
		hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_pItemData->pVtx,
			NULL);
		// �G���[
		if (FAILED(hr))
		{
			MessageBox(NULL, _T("���������_�쐬�`�F�b�N"), _T("GAMEITEM"), NULL);
			return E_FAIL;
		}

		// �ύX�̂Ȃ����_�͂��炩���ߐݒ���s��
		{
			VERTEX_3D	*pvtx;

			hr = g_pItemData->pVtx->Lock(0, 0, (void**)&pvtx, 0);		// ���b�N

			for (int i = 0; i < NUM_VERTEX; i++)
			{
				pvtx[i].diffuse = D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff);
				pvtx[i].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
				pvtx[i].tex = D3DXVECTOR2(1.0f*(i % 2), 1.0f*(i / 2));
			}
			pvtx[0].vtx = D3DXVECTOR3(-ITEM_SIZE_X, ITEM_SIZE_Y*2, 0.0f);
			pvtx[1].vtx = D3DXVECTOR3(ITEM_SIZE_X, ITEM_SIZE_Y*2, 0.0f);
			pvtx[2].vtx = D3DXVECTOR3(-ITEM_SIZE_X, 0.0f, 0.0f);
			pvtx[3].vtx = D3DXVECTOR3(ITEM_SIZE_X, 0.0f, 0.0f);

			hr = g_pItemData->pVtx->Unlock();							// ���b�N����
		}
	}


	// �|�b�v�e�[�u���̓ǂݍ���
	if (FAILED(CreateCSVFromFile(ITEM_TBL, (char *)"dfddd",
		CSV_CELRANGE{ 0, 1, 0, 0 },
		&g_pItemData->ItemPopTbl, CSV_OPTION_SHOWALL)))
	// �G���[
	{
		MessageBox(NULL, _T("������csv�`�F�b�N"), _T("GAMEITEM"), NULL);
		return E_FAIL;
	}

	// �������̃X�e�[�^�X�w��
	g_pItemData->Update = UpdateGameItemBeforeGameStart;	// �X�V�֐��|�C���^
	g_pItemData->TblIdx = -1;								// �C���f�b�N�X��0xff..��
	return S_OK;
}

/*=====================================================================
�Q�[���A�C�e���X�V(�Q�[���J�n�O)�֐�
�߂�l�Fvoid
����:void
=====================================================================*/
void UpdateGameItemBeforeGameStart(void)
{
	// �������Ȃ�(�\��)
	return;
}

/*=====================================================================
�Q�[���A�C�e���X�V(�Q�[���J�n��)�֐�
�����ɍX�V������
�߂�l�Fvoid
����:void
=====================================================================*/
void UpdateGameItemAfterGameStart(void)
{
	DWORD tNow;
	ENTITY_ITEM *work_pt;
	// �����̃J�E���g
	tNow = timeGetTime();							// �����̎擾
	g_pItemData->tItv += tNow - g_pItemData->tLast;	// �Ԋu�̎Z�o�y�щ��Z
	g_pItemData->tLast = tNow;						// ��ŋ��߂������̑��

	// �Ԋu�������|�b�v�Ԋu�����𒴂��Ă����ꍇ
	while (true)
	{
		if (_ARRAY(g_pItemData->ItemPopTbl, g_pItemData->TblIdx + 1, 0)._int < g_pItemData->tItv)
		{
			// INTERVAL���Ԃ������
			g_pItemData->tItv -= _ARRAY(g_pItemData->ItemPopTbl, ++g_pItemData->TblIdx, 0)._int;

			// �A�C�e���̐ݒu
			SetItem(_ARRAY(g_pItemData->ItemPopTbl, g_pItemData->TblIdx, 1)._float,
				(ITEM_AI_TYPE)_ARRAY(g_pItemData->ItemPopTbl, g_pItemData->TblIdx, 2)._int,
				(ITEM_TEX_TYPE)_ARRAY(g_pItemData->ItemPopTbl, g_pItemData->TblIdx, 3)._int,
				_ARRAY(g_pItemData->ItemPopTbl, g_pItemData->TblIdx, 4)._int);
		}
		else break;
	}

	work_pt = g_pItemData->top_pt;
	while (work_pt != NULL)
	{
		work_pt->UpdateEachGameItem(work_pt);
		work_pt = work_pt->next_pt;
	}
	return;
}
/*=====================================================================
�Q�[���A�C�e���X�V�֐�
�߂�l�Fvoid
����:void
=====================================================================*/
void UpdateGameItem(void)
{
	// �֐��|�C���^�Ŏw�肷��
	g_pItemData->Update();
}

/*=====================================================================
�Q�[���A�C�e���`��֐�
�߂�l�Fvoid
����:void
=====================================================================*/
void DrawGameItem(void)
{
	D3DDEVICE(pDevice);
	ENTITY_ITEM *work_pt = g_pItemData->top_pt;
	D3DXMATRIX	mtxWorld;
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���_�f�[�^�̐ݒ�(������g��Ȃ�)
	pDevice->SetStreamSource(0, g_pItemData->pVtx, 0, sizeof(VERTEX_3D));

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	// �k���܂ł��
	while (work_pt != NULL)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorld);

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScl, work_pt->scl.x, work_pt->scl.y, work_pt->scl.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, work_pt->rot.y, work_pt->rot.x, work_pt->rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, work_pt->pos.x, work_pt->pos.y, work_pt->pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);
		
		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		// �e�N�X�`���̔ɉh
		pDevice->SetTexture(0, work_pt->pTex);

		// �`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		
		// ���|�C���^
		work_pt = work_pt->next_pt;
	}

	// ���C���e�B���O��yuukou �ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, true);

}

/*=====================================================================
�Q�[���A�C�e���I���֐�
�߂�l�Fvoid
����:void
=====================================================================*/
void UninitGameItem(void)
{
	// �k���`�F�b�N
	if (g_pItemData == NULL)return;

	// csv�̊J��
	SAFE_RELEASE(g_pItemData->ItemPopTbl);

	// �e�N�X�`���̊J��
	for (int i = 0; i < MAX_TEXTYPE; i++)
	{
		SAFE_RELEASE(g_pItemData->pTex[i]);
	}

	// ����������
	free(g_pItemData);
}


/*=====================================================================
�Q�[���A�C�e���̎��ԃX�^�[�g�֐�
������g���Ǝ��Ԃ��i�݂܂���
�߂�l�Fvoid
����:void
=====================================================================*/
void StartGameItemTime(void)
{
	if (g_pItemData == NULL)return;
	g_pItemData->tItv	= 0;
	g_pItemData->tLast	= timeGetTime();	// �����̎擾
	g_pItemData->Update = UpdateGameItemAfterGameStart;
	g_pItemData->TblIdx = -1;								// �C���f�b�N�X��0xff..��

}

/*=====================================================================
�Q�[���A�C�e��	�Z�b�g�֐�(��cpp��p)
�߂�l�Fvoid
����:void
=====================================================================*/
void SetItem(float X, ITEM_AI_TYPE Ai, ITEM_TEX_TYPE Tex, bool bPlus)
{
	ENTITY_ITEM *work_pt =(ENTITY_ITEM*) calloc(1, sizeof ENTITY_ITEM);

	if (g_pItemData->top_pt == NULL)
	{
		g_pItemData->top_pt = work_pt;
		g_pItemData->bot_pt = work_pt;
	}
	else
	{
		g_pItemData->bot_pt->next_pt = work_pt;
	}

	// ���l�̑��
	work_pt->next_pt = NULL;
	work_pt->pos = D3DXVECTOR3(X, ITEM_SIZE_Y, POP_Z);
	work_pt->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	work_pt->bPlus = bPlus;
	work_pt->pTex = g_pItemData->pTex[Tex];
	work_pt->UpdateEachGameItem = (bool(*)(ENTITY_ITEM*))ReferenceItemAI(Ai);
}

/*=====================================================================
�Q�[���A�C�e��	�Q�Ɗ֐�(��cpp��p)
�߂�l�Fvoid
����:ITEM_AI_TYPE Ai		�Q�Ƃ������ԍ����w��
=====================================================================*/
void *ReferenceItemAI(ITEM_AI_TYPE Ai)
{
	switch (Ai)
	{
	case NO_AI:
		return (void*)UpdateItemAI_NO;
		break;
	case MAX_AITYPE:
	default:
		return NULL;
		break;
	}
}