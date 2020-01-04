/**********************************************************************
[[�Q�[���A�C�e���w�b�_�[(GameItem.h)]]
	��ҁF���c�@�^�K

	GameItem.cpp�Ɋւ���w�b�_�[
***********************************************************************/
#pragma once
#include "main.h"
#include "CSV_Reader.h"

#include <list>
#include <string>

// �f�o�b�O�H
#include "Framework\Collider\ColliderObserver.h"

#define ITEM_SIZE_X		(10.0f/2)	// �A�C�e���̃T�C�Y�w��(x)
#define ITEM_SIZE_Y		(10.0f/2)	// (y)

// �A�C�e���X�s�[�h
#define ITEM_SPD	(15.0f)

//---------------------------------------------------------------------
//	�\���́A�񋓑́A���p�̐錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------
class BoxCollider3D;
class BoardPolygon;
class GameItem;
class string;
// �����|���񋓌^
enum ITEM_TEXTURE {
	MINUS_DEFA_TEX,
	PLUS_DEFA_TEX,
	MAX_ITEMTEXTURE
};

// �A�C�e��(�N���X)�񋓌^
enum ITEM_CLASS {
	DEFAULT_ITEMCLASS,

	MAX_AITYPE
};


// �A�C�e�������S�̂Ŏg�p�����ł��낤�N���X
class Item
{
public:
	Item();
	~Item();

	void Update();
	void Draw();
	void ResetGame();
	void CountMinus()
	{
		minus++;
	}

	void CountPlus()
	{
		plus++;
	}

private:
	std::string GetTexFileAddress(ITEM_TEXTURE Tex);
	void SetGameItem(float X, ITEM_TEXTURE Tex, ITEM_CLASS Class, bool bPlus,...);

	LPDIRECT3DVERTEXBUFFER9	pVtx;		// ���_�o�b�t�@
	std::list<GameItem *>	li_Item;	// �A�C�e���N���X�x�[�X�̃��X�g

	CSV_FILE				*CSV_Data;	// CSV�t�@�C���\����
	int						CSV_idx;	// �Y����
	
	int						tItv;		// �O��o�������Ƃ�����̂�����
	DWORD					tLast;		// �O�t���[�����̎���

	int						minus;		// �}�C�i�X�A�C�e���̔j��
	int						plus;		// �v���X�A�C�e���̔j��

};


// �A�C�e���̊��N���X
class GameItem :public GameObject, public ColliderObserver 
{
public:
	GameItem();
	virtual ~GameItem();
	void Init();
	virtual void EachItemUpdate();	// �A�C�e����ʁu���Ɓv�̍X�V�֐�()
	virtual void EachItemDraw();	// ��L�`��
	virtual void LastWord(Item * item);						// ���Ŏ��Ɍ��ʂ�����
	void CheckDeleteByPos();

	bool GetisItemPlus()
	{
		return isPlus;
	}

	void SetItemPlus(bool is)
	{
		isPlus = is;
	}

	bool GetisShow()
	{
		return isShow;
	}

	bool GetisDelete()
	{
		return isDelete;
	}

	bool IsHit()
	{
		return isHit;
	}

	void Uninit(){}
	void OnColliderHit(ColliderObserver *other) final;
	void SetTexture(LPDIRECT3DTEXTURE9 pTex);

private:
	virtual void Init_sp();
	std::shared_ptr<BoxCollider3D>	collider;		// �����蔻��

protected:
	LPDIRECT3DTEXTURE9				pD3DTexture;	// �e�N�X�`��
	bool							isPlus;			// �v���X����(�������Ă�����H)
	bool							isShow;			// �\���̉�
	bool							isDelete;		// �폜�̃R���g���[��
	bool							isHit;			// �v���C���[�ɂ���������
};

//---------------------------------------------------------------------
//	�v���g�^�C�v�錾 (��cpp�ł��g�p����)
//---------------------------------------------------------------------
void StartGameItemTime(void);
HRESULT InitGameItem(void);
void UpdateGameItem(void);
void DrawGameItem(void);
void UninitGameItem(void);
