//=============================================================================
//
// リザルト画面処理 [title.cpp]
// Author : 後地　賢人
//
//=============================================================================
#include "main.h"
#include "title.h"
#include <stdio.h>


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTitle(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTitle = NULL;		// テクスチャへのポインタ

VERTEX_2D				g_vertexWkTitle[NUM_VERTEX];			// 頂点情報格納ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_TITLE,				// ファイルの名前
			&g_pD3DTextureTitle);		// 読み込むメモリー

	}

	// 頂点情報の作成
	MakeVertexTitle();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	if (g_pD3DTextureTitle != NULL)
	{// テクスチャの開放
		g_pD3DTextureTitle->Release();
		g_pD3DTextureTitle = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
	//if (GetKeyboardTrigger(DIK_RETURN))
	//{// Enter押したら、ステージを切り替える
	//	InitGame();				// ゲームの再初期化処理
	//	SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_sample000);

	//}
	//else if (IsButtonTriggered(0, BUTTON_B))
	//{
	//	InitGame();				// ゲームの再初期化処理
	//	SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_sample000);
	//}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureTitle);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitle, sizeof(VERTEX_2D));

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTitle(void)
{
	// 頂点座標の設定
	g_vertexWkTitle[0].vtx = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y, 0.0f);
	g_vertexWkTitle[1].vtx = D3DXVECTOR3(TITLE_POS_X + SCREEN_WIDTH, TITLE_POS_Y, 0.0f);
	g_vertexWkTitle[2].vtx = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y + SCREEN_HEIGHT, 0.0f);
	g_vertexWkTitle[3].vtx = D3DXVECTOR3(TITLE_POS_X + SCREEN_WIDTH, TITLE_POS_Y + SCREEN_HEIGHT, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkTitle[0].rhw =
		g_vertexWkTitle[1].rhw =
		g_vertexWkTitle[2].rhw =
		g_vertexWkTitle[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkTitle[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkTitle[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitle[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitle[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitle[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

