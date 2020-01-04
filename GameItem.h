/**********************************************************************
[[ゲームアイテムヘッダー(GameItem.h)]]
	作者：奥田　真規

	GameItem.cppに関するヘッダー
***********************************************************************/
#pragma once
#include "main.h"
#include "CSV_Reader.h"

#include <list>
#include <string>

// デバッグ？
#include "Framework\Collider\ColliderObserver.h"

#define ITEM_SIZE_X		(10.0f/2)	// アイテムのサイズ指定(x)
#define ITEM_SIZE_Y		(10.0f/2)	// (y)

// アイテムスピード
#define ITEM_SPD	(15.0f)

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言 (他cppでも使用する)
//---------------------------------------------------------------------
class BoxCollider3D;
class BoardPolygon;
class GameItem;
class string;
// いたポリ列挙型
enum ITEM_TEXTURE {
	MINUS_DEFA_TEX,
	PLUS_DEFA_TEX,
	MAX_ITEMTEXTURE
};

// アイテム(クラス)列挙型
enum ITEM_CLASS {
	DEFAULT_ITEMCLASS,

	MAX_AITYPE
};


// アイテム処理全体で使用されるであろうクラス
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

	LPDIRECT3DVERTEXBUFFER9	pVtx;		// 頂点バッファ
	std::list<GameItem *>	li_Item;	// アイテムクラスベースのリスト

	CSV_FILE				*CSV_Data;	// CSVファイル構造体
	int						CSV_idx;	// 添え字
	
	int						tItv;		// 前回出現したときからのじかん
	DWORD					tLast;		// 前フレーム時の時刻

	int						minus;		// マイナスアイテムの破壊数
	int						plus;		// プラスアイテムの破壊数

};


// アイテムの基底クラス
class GameItem :public GameObject, public ColliderObserver 
{
public:
	GameItem();
	virtual ~GameItem();
	void Init();
	virtual void EachItemUpdate();	// アイテム種別「ごと」の更新関数()
	virtual void EachItemDraw();	// 上記描画
	virtual void LastWord(Item * item);						// 消滅時に効果が発動
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
	std::shared_ptr<BoxCollider3D>	collider;		// 当たり判定

protected:
	LPDIRECT3DTEXTURE9				pD3DTexture;	// テクスチャ
	bool							isPlus;			// プラス属性(当たっていいやつ？)
	bool							isShow;			// 表示の可否
	bool							isDelete;		// 削除のコントロール
	bool							isHit;			// プレイヤーにあたったか
};

//---------------------------------------------------------------------
//	プロトタイプ宣言 (他cppでも使用する)
//---------------------------------------------------------------------
void StartGameItemTime(void);
HRESULT InitGameItem(void);
void UpdateGameItem(void);
void DrawGameItem(void);
void UninitGameItem(void);
