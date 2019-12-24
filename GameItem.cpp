/**********************************************************************
[[メインプログラム(GameItem.cpp)]]
	作者：奥田　真規

	ゲームアイテムに関するプログラム
***********************************************************************/
#include "GameItem.h"
#include "GameItem_Ai.h"
#include "CSV_Reader.h"

//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------
// ゲットデバイスの簡略化
#define D3DDEVICE(p)	LPDIRECT3DDEVICE9 (p)=GetDevice();

// テーブルアドレス
#define ITEM_TBL	_T("data/GIMMICK/GameItemTbl.csv")

// しゅつげんｚ(仮）
#define POP_Z		(100.0f)

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------


typedef struct GAME_ITEM_DATA {
	LPDIRECT3DTEXTURE9		pTex[MAX_TEXTYPE];	// アイテムで使用する総テクスチャ
	LPDIRECT3DVERTEXBUFFER9	pVtx;				// アイテムで使用されるであろう頂点様
	ENTITY_ITEM			*top_pt;			// 開始ポインタ	
	ENTITY_ITEM			*bot_pt;			// 終端ポインタ			
	CSV_FILE			*ItemPopTbl;		// CSVのテーブル
	int					TblIdx;				// 最後に出現させたテーブルの行
	DWORD				tItv;				// 前回出現したときからのじかん
	DWORD				tLast;				// 前フレーム時の時刻
	void				(*Update)(void);	// 更新関数ポインタ
}GAME_ITEM_DATA;

//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------
void UpdateGameItemBeforeGameStart(void);
void UpdateGameItemAfterGameStart(void);
void SetItem(float X, ITEM_AI_TYPE Ai, ITEM_TEX_TYPE Tex, bool bPlus);
void *ReferenceItemAI(ITEM_AI_TYPE Ai);
//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------
static GAME_ITEM_DATA *g_pItemData = NULL;


/*=====================================================================
ゲームアイテム初期化関数
戻り値：HRESULT
引数:void
=====================================================================*/
HRESULT InitGameItem(void)
{
	D3DDEVICE(pDevice);	// デバイス
	HRESULT hr = S_OK;	// 結果表示
	// テクスチャのアドレス
	const char *const filename[MAX_TEXTYPE] =
	{
		_T("Data/TEXTURE/Item/woman.png"),
		_T("Data/TEXTURE/Item/woman.png"),
	};

	// ヌルチェック
	if (g_pItemData != NULL)
	{
		MessageBox(NULL, _T("初期化ヌルチェック"), _T("GAMEITEM"), NULL);
		return E_FAIL;
	}

	// アイテムのメモリかくほ
	g_pItemData = (GAME_ITEM_DATA*)calloc(1, sizeof(GAME_ITEM_DATA));

	// テクスチャの確保
	for (int i = 0; i < MAX_TEXTYPE; i++)
	{
		// 読みこみ
		hr = D3DXCreateTextureFromFile(pDevice, filename[i], &g_pItemData->pTex[i]);

		// エラー
		if (FAILED(hr))
		{
			MessageBox(NULL, _T("初期化テクスチャチェック"), _T("GAMEITEM"), NULL);
			return E_FAIL;
		}
	}

	// 頂点の作成
	{
		// 頂点の作成
		hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_pItemData->pVtx,
			NULL);
		// エラー
		if (FAILED(hr))
		{
			MessageBox(NULL, _T("初期化頂点作成チェック"), _T("GAMEITEM"), NULL);
			return E_FAIL;
		}

		// 変更のない頂点はあらかじめ設定を行う
		{
			VERTEX_3D	*pvtx;

			hr = g_pItemData->pVtx->Lock(0, 0, (void**)&pvtx, 0);		// ロック

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

			hr = g_pItemData->pVtx->Unlock();							// ロック解除
		}
	}


	// ポップテーブルの読み込み
	if (FAILED(CreateCSVFromFile(ITEM_TBL, (char *)"dfddd",
		CSV_CELRANGE{ 0, 1, 0, 0 },
		&g_pItemData->ItemPopTbl, CSV_OPTION_SHOWALL)))
	// エラー
	{
		MessageBox(NULL, _T("初期化csvチェック"), _T("GAMEITEM"), NULL);
		return E_FAIL;
	}

	// もろもろのステータス指定
	g_pItemData->Update = UpdateGameItemBeforeGameStart;	// 更新関数ポインタ
	g_pItemData->TblIdx = -1;								// インデックスを0xff..に
	return S_OK;
}

/*=====================================================================
ゲームアイテム更新(ゲーム開始前)関数
戻り値：void
引数:void
=====================================================================*/
void UpdateGameItemBeforeGameStart(void)
{
	// 何もしない(予定)
	return;
}

/*=====================================================================
ゲームアイテム更新(ゲーム開始後)関数
ここに更新を書く
戻り値：void
引数:void
=====================================================================*/
void UpdateGameItemAfterGameStart(void)
{
	DWORD tNow;
	ENTITY_ITEM *work_pt;
	// 時刻のカウント
	tNow = timeGetTime();							// 時刻の取得
	g_pItemData->tItv += tNow - g_pItemData->tLast;	// 間隔の算出及び加算
	g_pItemData->tLast = tNow;						// 上で求めだ時刻の代入

	// 間隔時刻がポップ間隔時刻を超えていた場合
	while (true)
	{
		if (_ARRAY(g_pItemData->ItemPopTbl, g_pItemData->TblIdx + 1, 0)._int < g_pItemData->tItv)
		{
			// INTERVAL時間から引く
			g_pItemData->tItv -= _ARRAY(g_pItemData->ItemPopTbl, ++g_pItemData->TblIdx, 0)._int;

			// アイテムの設置
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
ゲームアイテム更新関数
戻り値：void
引数:void
=====================================================================*/
void UpdateGameItem(void)
{
	// 関数ポインタで指定する
	g_pItemData->Update();
}

/*=====================================================================
ゲームアイテム描画関数
戻り値：void
引数:void
=====================================================================*/
void DrawGameItem(void)
{
	D3DDEVICE(pDevice);
	ENTITY_ITEM *work_pt = g_pItemData->top_pt;
	D3DXMATRIX	mtxWorld;
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// 頂点データの設定(一つしか使わない)
	pDevice->SetStreamSource(0, g_pItemData->pVtx, 0, sizeof(VERTEX_3D));

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	// ヌルまでやる
	while (work_pt != NULL)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxWorld);

		// スケールを反映
		D3DXMatrixScaling(&mtxScl, work_pt->scl.x, work_pt->scl.y, work_pt->scl.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, work_pt->rot.y, work_pt->rot.x, work_pt->rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, work_pt->pos.x, work_pt->pos.y, work_pt->pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);
		
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		// テクスチャの繁栄
		pDevice->SetTexture(0, work_pt->pTex);

		// 描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		
		// 次ポインタ
		work_pt = work_pt->next_pt;
	}

	// ラインティングをyuukou にする
	pDevice->SetRenderState(D3DRS_LIGHTING, true);

}

/*=====================================================================
ゲームアイテム終了関数
戻り値：void
引数:void
=====================================================================*/
void UninitGameItem(void)
{
	// ヌルチェック
	if (g_pItemData == NULL)return;

	// csvの開放
	SAFE_RELEASE(g_pItemData->ItemPopTbl);

	// テクスチャの開放
	for (int i = 0; i < MAX_TEXTYPE; i++)
	{
		SAFE_RELEASE(g_pItemData->pTex[i]);
	}

	// 自分を消す
	free(g_pItemData);
}


/*=====================================================================
ゲームアイテムの時間スタート関数
これを使うと時間が進みます☆
戻り値：void
引数:void
=====================================================================*/
void StartGameItemTime(void)
{
	if (g_pItemData == NULL)return;
	g_pItemData->tItv	= 0;
	g_pItemData->tLast	= timeGetTime();	// 時刻の取得
	g_pItemData->Update = UpdateGameItemAfterGameStart;
	g_pItemData->TblIdx = -1;								// インデックスを0xff..に

}

/*=====================================================================
ゲームアイテム	セット関数(同cpp専用)
戻り値：void
引数:void
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

	// 数値の代入
	work_pt->next_pt = NULL;
	work_pt->pos = D3DXVECTOR3(X, ITEM_SIZE_Y, POP_Z);
	work_pt->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	work_pt->bPlus = bPlus;
	work_pt->pTex = g_pItemData->pTex[Tex];
	work_pt->UpdateEachGameItem = (bool(*)(ENTITY_ITEM*))ReferenceItemAI(Ai);
}

/*=====================================================================
ゲームアイテム	参照関数(同cpp専用)
戻り値：void
引数:ITEM_AI_TYPE Ai		参照したい番号を指定
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