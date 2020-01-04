/**********************************************************************
[[メインプログラム(GameItem.cpp)]]
	作者：奥田　真規

	ゲームアイテムに関するプログラム
***********************************************************************/
#include "GameItem.h"
#include "GameItem_Ai.h"
#include "Gameeffect.h"
#include <string>
#include <stdio.h>
// 当たり判定(多分）
#include "Framework\Collider\BoxCollider3D.h"

// データをまとめて参照とかできるかんじ(多分）
#include "Framework\Resource\ResourceManager.h"
#include "Framework\Tool\DebugWindow.h"
#include "Framework\Core\ObjectPool.h"

//メモリリークの検出フレームワーク
#include "Framework/Tool/MemoryDetect.h"

//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------
// ゲットデバイスの簡略化
#define D3DDEVICE(p)	LPDIRECT3DDEVICE9 (p)=GetDevice();

// テーブルアドレス
#define ITEM_TBL	_T("data/GIMMICK/GameItemTbl.csv")

// しゅつげんｚ(仮）
#define POP_Z		(2500.0f)

// アイテムのエフェクト位置をスクリーンにする
#define IS_ITEM_EFFECT_POS_SCREEN	(true)

#if IS_ITEM_EFFECT_POS_SCREEN
#define EFFECT_POS_SCREEN	D3DXVECTOR3(SCREEN_CENTER_X,SCREEN_CENTER_Y+100.0f,0.0f)
#endif

#define IS_ITEMDEBUG	(false)
#define IS_ITEMCHECKNUM_DEBUG	(false)
//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------
static Item *g_item = NULL;

/*=====================================================================
ゲームアイテムの時間スタート関数
これを使用するとゲームが開始されます
戻り値：void
引数:void
=====================================================================*/
void StartGameItemTime(void)
{
	if (g_item == NULL)return;
	g_item->ResetGame();
}

/*=====================================================================
ゲームアイテム初期化関数
戻り値：HRESULT
引数:void
=====================================================================*/
HRESULT InitGameItem(void)
{
	if (g_item != NULL)
	{
		MessageBox(NULL, _T("Itemクラスのインスタンスが存在しています"), _T("GAMEITEM--ERROR--"), NULL);
		return E_FAIL;
	}

	// クラスのコンストラクタ
	g_item = new Item();

	return S_OK;

}

/*=====================================================================
ゲームアイテム終了関数
戻り値：void
引数:void
=====================================================================*/
void UninitGameItem(void)
{
	// クラスのデストラクタ
	SAFE_DELETE(g_item);
}

/*=====================================================================
ゲームアイテム更新関数
戻り値：void
引数:void
=====================================================================*/
void UpdateGameItem(void)
{
	g_item->Update();
}

/*=====================================================================
ゲームアイテム描画関数
戻り値：void
引数:void
=====================================================================*/
void DrawGameItem(void)
{
	g_item->Draw();
}

//////////////////////////////////////////////////////////////////////////////////
// Itemクラス
/////////////////////////////////////////////////////////////////////////////////

/*=====================================================================
+Item::コンストラクタ関数
引数: void
=====================================================================*/
Item::Item() :pVtx(0), li_Item(0), CSV_Data(0), CSV_idx(-1), tItv(0), minus(0), plus(0)
{
	// テクスチャの読み込み
	for (int i = 0; i < ITEM_TEXTURE::MAX_ITEMTEXTURE; i++)
	{
		ResourceManager::Instance()->LoadTexture(GetTexFileAddress((ITEM_TEXTURE)i).c_str());		
	}

	// 頂点の作成
	{
		D3DDEVICE(pDevice);
		// 頂点の作成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&pVtx,
			NULL);

		// 変更のない頂点はあらかじめ設定を行う
		{
			VERTEX_3D	*pvtx = NULL;

			pVtx->Lock(0, 0, (void**)&pvtx, 0);		// ロック

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

			pVtx->Unlock();							// ロック解除
		}
	}

	
	// ポップテーブルの読み込み(エラーの表示はあり)
	CreateCSVFromFile(ITEM_TBL, (char *)"dfddd",
		CSV_CELRANGE{ 0, 1, 0, 0 },
		&CSV_Data, CSV_OPTION_SHOWERR);

#if IS_ITEMDEBUG
	string str;
	for (int i = 0; i < CSV_Data->Line_Size; i++)
	{
		string line = to_string(i) + "行目:";
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

	// 仮で現在の時刻を挿入
	tLast = timeGetTime();

}

/*=====================================================================
+Item::デストラクタ関数
引数: void
=====================================================================*/
Item::~Item()
{
	SAFE_RELEASE(CSV_Data);

	// テクスチャの開放
	for (int i = 0; i < ITEM_TEXTURE::MAX_ITEMTEXTURE; i++)
	{
		ResourceManager::Instance()->ReleaseTexture(GetTexFileAddress((ITEM_TEXTURE)i).c_str());
	}

	/*
	頂点バッファの解放が抜けていた
	*/
	SAFE_RELEASE(pVtx);

	/*
	リストに残っているGameItemの削除が必要
	*/
	for (auto&& item : li_Item)
	{
		SAFE_DELETE(item);
	}
}

/*=====================================================================
+Item::更新関数
戻り値：void
引数: void
=====================================================================*/
void Item:: Update()
{
	DWORD tNow;
	// 時刻のカウント
	tNow = timeGetTime();				// 時刻の取得
	tItv += tNow - tLast;				// 間隔の算出及び加算
	tLast = tNow;						// 上で求めだ時刻の代入

#if IS_ITEMDEBUG
	string str;
	for (int i = 0; i < CSV_Data->Line_Size; i++)
	{
		string line = to_string(i) + "行目:";
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


	// ポップ処理
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
			// INTERVAL時間から引く
			tItv -= _ARRAY(CSV_Data, ++CSV_idx, 0)._int;

			// アイテムの設置
			SetGameItem(_ARRAY(CSV_Data, CSV_idx, 1)._float,
				(ITEM_TEXTURE)_ARRAY(CSV_Data, CSV_idx, 2)._int,
				(ITEM_CLASS)_ARRAY(CSV_Data, CSV_idx, 3)._int,
				_ARRAY(CSV_Data, CSV_idx, 4)._int);
		}
		else break;
	}

	// 全リスト操作
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
			// 削除処理
			//SAFE_DELETE(*itr);
			ObjectPool::Instance()->Destroy(*itr);
			(*itr) = NULL;
			itr = li_Item.erase(itr);
		}	
		else ++itr;
		
	}

}

/*=====================================================================
+Item::描画関数
戻り値：void
引数: void
=====================================================================*/
void Item::Draw()
{
	D3DDEVICE(pDevice);
	list <GameItem *>::iterator itr;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// 頂点データの設定(一つしか使わない)
	pDevice->SetStreamSource(0, pVtx, 0, sizeof(VERTEX_3D));

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	// 全リスト操作
	for (list <GameItem *>::iterator itr = li_Item.begin(); itr != li_Item.end(); itr++)
	{
		if (!(*itr)->GetisShow())continue;

		(*itr)->EachItemDraw();	
	}

	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}

/*=====================================================================
+Item::ゲームリセット関数
戻り値：void
引数: void
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
-Item::テクスチャファイルアドレス取得関数
戻り値：
const char * const	: 帰ってくるファイル
引数:Tex			: 調べたいテクスチャの種類
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
-Item::ゲームアイテム関数
戻り値：void
引数:
float X,			:X値
ITEM_TEXTURE Tex,	:テクスチャ
ITEM_CLASS Class,	:使用クラス
bool bPlus,			:プラスアイテムかどうか
...					:クラスによって使用するステータス
=====================================================================*/
void Item::SetGameItem(float X, ITEM_TEXTURE Tex, ITEM_CLASS Class, bool bPlus,...)
{
	GameItem *item;

	// 継承クラスの設定
	switch (Class)
	{
	case DEFAULT_ITEMCLASS:
	case MAX_AITYPE:
	default:
		item = ObjectPool::Instance()->Create<GameItem>();
		break;
	}

	// 初期化
	item->Init();

	// テクスチャの設定
	LPDIRECT3DTEXTURE9 tex;
	ResourceManager::Instance()-> GetTexture(GetTexFileAddress(Tex).c_str(), tex);
	item->SetTexture(tex);

	// 位置
	item->SetPosition(D3DXVECTOR3(X, 0.0f, POP_Z));
	item->SetItemPlus(bPlus);

	li_Item.push_back(item);	// リストの追加
}


//////////////////////////////////////////////////////////////////////////////////
// GameItemクラス
/////////////////////////////////////////////////////////////////////////////////

/*=====================================================================
+GameItem::コンストラクタ関数
引数: void
=====================================================================*/
GameItem::GameItem() :isPlus(0), isShow(true), isDelete(false), isHit(false)
{
	collider = BoxCollider3D::Create("Item", transform);
	collider->SetSize({ ITEM_SIZE_X * 2, ITEM_SIZE_Y * 2, 0.0f });
	collider->AddObserver(this);
//	SetEffect(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0), DOWN);	// デバッグ用

}

/*=====================================================================
+GameItem::デストラクタ関数
引数: void
=====================================================================*/
GameItem::~GameItem()
{
	collider.reset();

	/*
	自身が参照してるテクスチャの解放が必要
	*/
	SAFE_RELEASE(pD3DTexture);
}

/*=====================================================================
+GameItem::初期化関数
引数: void
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
	初期化時に当たり判定を有効化する
	*/
	collider->SetActive(true);

	/*
	ヒットフラグを立てる
	*/
	isHit = true;
}

/*=====================================================================
+GameItem::更新関数

引数: void
=====================================================================*/
void GameItem::EachItemUpdate()
{
}

/*=====================================================================
+GameItem::消滅時効果発動関数
戻り値:void
引数: Item * item:アイテムクラス
=====================================================================*/
void GameItem::LastWord(Item * item)
{
	/*
	プレイヤーとあたっていたときだけエフェクト発生
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
+GameItem::描画関数
	描画を行う
引数: void
=====================================================================*/
void GameItem::EachItemDraw()
{
	D3DDEVICE(pDevice);
	transform->SetWorld();											// ワールド行列を設定
	pDevice->SetTexture(0, pD3DTexture);							// テクスチャの繁栄
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);	// 描画

	collider->Draw();
}

/*=====================================================================
+GameItem::エリア外削除関数
	ポジション判定による削除フラグを立てる
引数: void
=====================================================================*/
void GameItem::CheckDeleteByPos()
{
	D3DXVECTOR3 pos = transform->GetPosition();
	pos.z -= ITEM_SPD;

	transform->SetPosition(pos);
	if (pos.z <= -400.0f)
	{
		//MessageBox(NULL, "エリア外", NULL, NULL);
		isShow = false;
		isDelete = true;
		SetActive(false);
	}
}

/*=====================================================================
+GameItem::当たり判定関数
戻り値:void
引数: ColliderObserver *other　: 衝突した相方
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
	判定が残り続けるのでオフにする
	*/
	collider->SetActive(false);


	//MessageBox(NULL, "当たり判定", NULL, NULL);
//	LastWord();	
}

/*=====================================================================
+GameItem::テクスチャ設置関数
戻り値:void
引数: LPDIRECT3DTEXTURE9 pTex :テクスチャ
=====================================================================*/
void GameItem::SetTexture(LPDIRECT3DTEXTURE9 pTex)
{
	pD3DTexture = pTex;
}

/*=====================================================================
-GameItem::初期化（特別）
戻り値:void
引数: void
=====================================================================*/
void GameItem::Init_sp()
{
}



/////////////////////////////////////////////////////////////////////////////////
