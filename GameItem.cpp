/**********************************************************************
[[���C���v���O����(GameItem.cpp)]]
	��ҁF���c�@�^�K

	�Q�[���A�C�e���Ɋւ���v���O����
***********************************************************************/
#include "GameItem.h"
#include "GameItem_Ai.h"
#include "Gameeffect.h"
#include <string>
#include <stdio.h>
// �����蔻��(�����j
#include "Framework\Collider\BoxCollider3D.h"

// �f�[�^���܂Ƃ߂ĎQ�ƂƂ��ł��邩��(�����j
#include "Framework\Resource\ResourceManager.h"
#include "Framework\Tool\DebugWindow.h"
#include "Framework\Core\ObjectPool.h"

//���������[�N�̌��o�t���[�����[�N
#include "Framework/Tool/MemoryDetect.h"

//---------------------------------------------------------------------
//	�}�N����`(��cpp������)
//---------------------------------------------------------------------
// �Q�b�g�f�o�C�X�̊ȗ���
#define D3DDEVICE(p)	LPDIRECT3DDEVICE9 (p)=GetDevice();

// �e�[�u���A�h���X
#define ITEM_TBL	_T("data/GIMMICK/GameItemTbl.csv")

// �������(���j
#define POP_Z		(2500.0f)

// �A�C�e���̃G�t�F�N�g�ʒu���X�N���[���ɂ���
#define IS_ITEM_EFFECT_POS_SCREEN	(true)

#if IS_ITEM_EFFECT_POS_SCREEN
#define EFFECT_POS_SCREEN	D3DXVECTOR3(SCREEN_CENTER_X,SCREEN_CENTER_Y+100.0f,0.0f)
#endif

#define IS_ITEMDEBUG	(false)
#define IS_ITEMCHECKNUM_DEBUG	(false)
//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾(��cpp������)
//---------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------
//	�v���g�^�C�v�錾(��cpp������)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------
static Item *g_item = NULL;

/*=====================================================================
�Q�[���A�C�e���̎��ԃX�^�[�g�֐�
������g�p����ƃQ�[�����J�n����܂�
�߂�l�Fvoid
����:void
=====================================================================*/
void StartGameItemTime(void)
{
	if (g_item == NULL)return;
	g_item->ResetGame();
}

/*=====================================================================
�Q�[���A�C�e���������֐�
�߂�l�FHRESULT
����:void
=====================================================================*/
HRESULT InitGameItem(void)
{
	if (g_item != NULL)
	{
		MessageBox(NULL, _T("Item�N���X�̃C���X�^���X�����݂��Ă��܂�"), _T("GAMEITEM--ERROR--"), NULL);
		return E_FAIL;
	}

	// �N���X�̃R���X�g���N�^
	g_item = new Item();

	return S_OK;

}

/*=====================================================================
�Q�[���A�C�e���I���֐�
�߂�l�Fvoid
����:void
=====================================================================*/
void UninitGameItem(void)
{
	// �N���X�̃f�X�g���N�^
	SAFE_DELETE(g_item);
}

/*=====================================================================
�Q�[���A�C�e���X�V�֐�
�߂�l�Fvoid
����:void
=====================================================================*/
void UpdateGameItem(void)
{
	g_item->Update();
}

/*=====================================================================
�Q�[���A�C�e���`��֐�
�߂�l�Fvoid
����:void
=====================================================================*/
void DrawGameItem(void)
{
	g_item->Draw();
}

//////////////////////////////////////////////////////////////////////////////////
// Item�N���X
/////////////////////////////////////////////////////////////////////////////////

/*=====================================================================
+Item::�R���X�g���N�^�֐�
����: void
=====================================================================*/
Item::Item() :pVtx(0), li_Item(0), CSV_Data(0), CSV_idx(-1), tItv(0), minus(0), plus(0)
{
	// �e�N�X�`���̓ǂݍ���
	for (int i = 0; i < ITEM_TEXTURE::MAX_ITEMTEXTURE; i++)
	{
		ResourceManager::Instance()->LoadTexture(GetTexFileAddress((ITEM_TEXTURE)i).c_str());		
	}

	// ���_�̍쐬
	{
		D3DDEVICE(pDevice);
		// ���_�̍쐬
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&pVtx,
			NULL);

		// �ύX�̂Ȃ����_�͂��炩���ߐݒ���s��
		{
			VERTEX_3D	*pvtx = NULL;

			pVtx->Lock(0, 0, (void**)&pvtx, 0);		// ���b�N

			for (int i = 0; i < NUM_VERTEX; i++)
			{
				pvtx[i].diffuse = D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff);
				pvtx[i].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
				pvtx[i].tex = D3DXVECTOR2(1.0f*(i % 2), 1.0f*(i / 2));
			}
			pvtx[0].vtx = D3DXVECTOR3(-ITEM_SIZE_X, ITEM_SIZE_Y * 2, 0.0f);
			pvtx[1].vtx = D3DXVECTOR3(ITEM_SIZE_X, ITEM_SIZE_Y * 2, 0.0f);
			pvtx[2].vtx = D3DXVECTOR3(-ITEM_SIZE_X, 0.0f, 0.0f);
			pvtx[3].vtx = D3DXVECTOR3(ITEM_SIZE_X, 0.0f, 0.0f);

			pVtx->Unlock();							// ���b�N����
		}
	}

	
	// �|�b�v�e�[�u���̓ǂݍ���(�G���[�̕\���͂���)
	CreateCSVFromFile(ITEM_TBL, (char *)"dfddd",
		CSV_CELRANGE{ 0, 1, 0, 0 },
		&CSV_Data, CSV_OPTION_SHOWERR);

#if IS_ITEMDEBUG
	string str;
	for (int i = 0; i < CSV_Data->Line_Size; i++)
	{
		string line = to_string(i) + "�s��:";
		str += line;
		for (int j = 0; j < CSV_Data->Column_Size; j++)
		{
			string column;
			if (*(CSV_Data->TypeArray + j) == CSV_INT)
			{
				column = "[" + to_string(_ARRAY(CSV_Data, i, j)._int) + "]";
			}
			else
			{
				column = "[" + to_string(_ARRAY(CSV_Data, i, j)._float) + "]";
			}
			str += column;
		}
		str += "\n";
	}
	MessageBox(NULL, str.c_str(), NULL, NULL);
#endif

	// ���Ō��݂̎�����}��
	tLast = timeGetTime();

}

/*=====================================================================
+Item::�f�X�g���N�^�֐�
����: void
=====================================================================*/
Item::~Item()
{
	SAFE_RELEASE(CSV_Data);

	// �e�N�X�`���̊J��
	for (int i = 0; i < ITEM_TEXTURE::MAX_ITEMTEXTURE; i++)
	{
		ResourceManager::Instance()->ReleaseTexture(GetTexFileAddress((ITEM_TEXTURE)i).c_str());
	}

	/*
	���_�o�b�t�@�̉���������Ă���
	*/
	SAFE_RELEASE(pVtx);

	/*
	���X�g�Ɏc���Ă���GameItem�̍폜���K�v
	*/
	for (auto&& item : li_Item)
	{
		SAFE_DELETE(item);
	}
}

/*=====================================================================
+Item::�X�V�֐�
�߂�l�Fvoid
����: void
=====================================================================*/
void Item:: Update()
{
	DWORD tNow;
	// �����̃J�E���g
	tNow = timeGetTime();				// �����̎擾
	tItv += tNow - tLast;				// �Ԋu�̎Z�o�y�щ��Z
	tLast = tNow;						// ��ŋ��߂������̑��

#if IS_ITEMDEBUG
	string str;
	for (int i = 0; i < CSV_Data->Line_Size; i++)
	{
		string line = to_string(i) + "�s��:";
		str += line;
		for (int j = 0; j < CSV_Data->Column_Size; j++)
		{
			string column;
			if (*(CSV_Data->TypeArray + j) == CSV_INT)
			{
				column = "[" + to_string(_ARRAY(CSV_Data, i, j)._int) + "]";
			}//_ARRAY(CSV_Data, CSV_idx + 1, 0)._int
			else
			{
				column = "[" + to_string(_ARRAY(CSV_Data, i, j)._float) + "]";
			}
			str += column;
		}
		str += "\n";
	}
	MessageBox(NULL, str.c_str(), NULL, NULL);
#endif


	// �|�b�v����
	while (true)
	{
		if (CSV_idx + 1 >= CSV_Data->Line_Size)
		{
			CSV_idx = -1;
		}
#if IS_ITEMCHECKNUM_DEBUG
		string line_str = to_string(_ARRAY(CSV_Data, (CSV_idx + 1), 0)._int) + ":" + to_string(CSV_idx)
			+":"+to_string(CSV_Data->Column_Size);
		MessageBox(NULL, line_str.c_str(), NULL, NULL);
#endif
		
		if (_ARRAY(CSV_Data, (CSV_idx + 1), 0)._int <= tItv)
		{
			// INTERVAL���Ԃ������
			tItv -= _ARRAY(CSV_Data, ++CSV_idx, 0)._int;

			// �A�C�e���̐ݒu
			SetGameItem(_ARRAY(CSV_Data, CSV_idx, 1)._float,
				(ITEM_TEXTURE)_ARRAY(CSV_Data, CSV_idx, 2)._int,
				(ITEM_CLASS)_ARRAY(CSV_Data, CSV_idx, 3)._int,
				_ARRAY(CSV_Data, CSV_idx, 4)._int);
		}
		else break;
	}

	// �S���X�g����
	for (list <GameItem *>::iterator itr = li_Item.begin(); itr != li_Item.end();)
	{
		(*itr)->CheckDeleteByPos();

		if (!(*itr)->GetisShow())
		{
			(*itr)->LastWord(this);
		}

		(*itr)->EachItemUpdate();
		if ((*itr)->GetisDelete())
		{
			// �폜����
			//SAFE_DELETE(*itr);
			ObjectPool::Instance()->Destroy(*itr);
			(*itr) = NULL;
			itr = li_Item.erase(itr);
		}	
		else ++itr;
		
	}

}

/*=====================================================================
+Item::�`��֐�
�߂�l�Fvoid
����: void
=====================================================================*/
void Item::Draw()
{
	D3DDEVICE(pDevice);
	list <GameItem *>::iterator itr;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���_�f�[�^�̐ݒ�(������g��Ȃ�)
	pDevice->SetStreamSource(0, pVtx, 0, sizeof(VERTEX_3D));

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	// �S���X�g����
	for (list <GameItem *>::iterator itr = li_Item.begin(); itr != li_Item.end(); itr++)
	{
		if (!(*itr)->GetisShow())continue;

		(*itr)->EachItemDraw();	
	}

	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}

/*=====================================================================
+Item::�Q�[�����Z�b�g�֐�
�߂�l�Fvoid
����: void
=====================================================================*/
void Item::ResetGame()
{
	CSV_idx = -1;
	tItv = minus = plus = 0;
	tLast = timeGetTime();

	for (list <GameItem *>::iterator itr = li_Item.begin(); itr != li_Item.end();itr++)
	{
		ObjectPool::Instance()->Destroy(*itr);
		itr=li_Item.erase(itr);
		
	}

}

/*=====================================================================
-Item::�e�N�X�`���t�@�C���A�h���X�擾�֐�
�߂�l�F
const char * const	: �A���Ă���t�@�C��
����:Tex			: ���ׂ����e�N�X�`���̎��
=====================================================================*/
std::string Item::GetTexFileAddress(ITEM_TEXTURE Tex)
{
	const char* const filename[ITEM_TEXTURE::MAX_ITEMTEXTURE] = {
		{"data/TEXTURE/Item/dust.png"},
		{"data/TEXTURE/Item/strawberry.png"}
	};

	return std::string(filename[Tex]);
}


/*=====================================================================
-Item::�Q�[���A�C�e���֐�
�߂�l�Fvoid
����:
float X,			:X�l
ITEM_TEXTURE Tex,	:�e�N�X�`��
ITEM_CLASS Class,	:�g�p�N���X
bool bPlus,			:�v���X�A�C�e�����ǂ���
...					:�N���X�ɂ���Ďg�p����X�e�[�^�X
=====================================================================*/
void Item::SetGameItem(float X, ITEM_TEXTURE Tex, ITEM_CLASS Class, bool bPlus,...)
{
	GameItem *item;

	// �p���N���X�̐ݒ�
	switch (Class)
	{
	case DEFAULT_ITEMCLASS:
	case MAX_AITYPE:
	default:
		item = ObjectPool::Instance()->Create<GameItem>();
		break;
	}

	// ������
	item->Init();

	// �e�N�X�`���̐ݒ�
	LPDIRECT3DTEXTURE9 tex;
	ResourceManager::Instance()-> GetTexture(GetTexFileAddress(Tex).c_str(), tex);
	item->SetTexture(tex);

	// �ʒu
	item->SetPosition(D3DXVECTOR3(X, 0.0f, POP_Z));
	item->SetItemPlus(bPlus);

	li_Item.push_back(item);	// ���X�g�̒ǉ�
}


//////////////////////////////////////////////////////////////////////////////////
// GameItem�N���X
/////////////////////////////////////////////////////////////////////////////////

/*=====================================================================
+GameItem::�R���X�g���N�^�֐�
����: void
=====================================================================*/
GameItem::GameItem() :isPlus(0), isShow(true), isDelete(false), isHit(false)
{
	collider = BoxCollider3D::Create("Item", transform);
	collider->SetSize({ ITEM_SIZE_X * 2, ITEM_SIZE_Y * 2, 0.0f });
	collider->AddObserver(this);
//	SetEffect(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0), DOWN);	// �f�o�b�O�p

}

/*=====================================================================
+GameItem::�f�X�g���N�^�֐�
����: void
=====================================================================*/
GameItem::~GameItem()
{
	collider.reset();

	/*
	���g���Q�Ƃ��Ă�e�N�X�`���̉�����K�v
	*/
	SAFE_RELEASE(pD3DTexture);
}

/*=====================================================================
+GameItem::�������֐�
����: void
=====================================================================*/
void GameItem::Init()
{
	Init_sp();
	isPlus = false;
	isShow = true;
	isDelete = false;
	isHit = false;

	SetActive(true);

	/*
	���������ɓ����蔻���L��������
	*/
	collider->SetActive(true);

	/*
	�q�b�g�t���O�𗧂Ă�
	*/
	isHit = true;
}

/*=====================================================================
+GameItem::�X�V�֐�

����: void
=====================================================================*/
void GameItem::EachItemUpdate()
{
}

/*=====================================================================
+GameItem::���Ŏ����ʔ����֐�
�߂�l:void
����: Item * item:�A�C�e���N���X
=====================================================================*/
void GameItem::LastWord(Item * item)
{
	/*
	�v���C���[�Ƃ������Ă����Ƃ������G�t�F�N�g����
	*/
	if (isHit)
	{
		if (isPlus)
		{
#if IS_ITEM_EFFECT_POS_SCREEN
			SetEffect(EFFECT_POS_SCREEN, UP);
#else
			SetEffect(transform->GetPosition(), UP);
#endif
			item->CountPlus();
		}
		else
		{
#if IS_ITEM_EFFECT_POS_SCREEN
			SetEffect(EFFECT_POS_SCREEN, DOWN);
#else
			SetEffect(transform->GetPosition(), DOWN);
#endif
			item->CountMinus();
		}
	}

	SetActive(false);
	isDelete = true;
	isShow = true;

}

/*=====================================================================
+GameItem::�`��֐�
	�`����s��
����: void
=====================================================================*/
void GameItem::EachItemDraw()
{
	D3DDEVICE(pDevice);
	transform->SetWorld();											// ���[���h�s���ݒ�
	pDevice->SetTexture(0, pD3DTexture);							// �e�N�X�`���̔ɉh
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);	// �`��

	collider->Draw();
}

/*=====================================================================
+GameItem::�G���A�O�폜�֐�
	�|�W�V��������ɂ��폜�t���O�𗧂Ă�
����: void
=====================================================================*/
void GameItem::CheckDeleteByPos()
{
	D3DXVECTOR3 pos = transform->GetPosition();
	pos.z -= ITEM_SPD;

	transform->SetPosition(pos);
	if (pos.z <= -400.0f)
	{
		//MessageBox(NULL, "�G���A�O", NULL, NULL);
		isShow = false;
		isDelete = true;
		SetActive(false);
	}
}

/*=====================================================================
+GameItem::�����蔻��֐�
�߂�l:void
����: ColliderObserver *other�@: �Փ˂�������
=====================================================================*/
void GameItem::OnColliderHit(ColliderObserver *other)
{
	if (!isShow)return;
	else isShow = false;

	{
		D3DXVECTOR3 pos = transform->GetPosition();
		Debug::Log("Hit ITEM =Pos(%f,%f,%f)", pos.x, pos.y, pos.z);
	}

	/*
	���肪�c�葱����̂ŃI�t�ɂ���
	*/
	collider->SetActive(false);


	//MessageBox(NULL, "�����蔻��", NULL, NULL);
//	LastWord();	
}

/*=====================================================================
+GameItem::�e�N�X�`���ݒu�֐�
�߂�l:void
����: LPDIRECT3DTEXTURE9 pTex :�e�N�X�`��
=====================================================================*/
void GameItem::SetTexture(LPDIRECT3DTEXTURE9 pTex)
{
	pD3DTexture = pTex;
}

/*=====================================================================
-GameItem::�������i���ʁj
�߂�l:void
����: void
=====================================================================*/
void GameItem::Init_sp()
{
}



/////////////////////////////////////////////////////////////////////////////////
