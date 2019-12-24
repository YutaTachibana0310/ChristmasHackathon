//=============================================================================
//
// スコア処理 [GameUI_distance.cpp]
// Author : 後地　賢人
//
//=============================================================================
#include "main.h"
#include "GameUI_distance.h"
#include <stdio.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexUI_distance(void);
void SetTextureUI_distance(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTexture_num = NULL;			// テクスチャへのポリゴン
static VERTEX_2D				g_vertexWk_num[UI_DISTANCE_DIGIT][NUM_VERTEX];	// 頂点情報格納ワーク

static LPDIRECT3DTEXTURE9		g_pD3DTexture_text = NULL;			// テクスチャへのポリゴン
static VERTEX_2D				g_vertexWk_text[NUM_VERTEX];	// 頂点情報格納ワーク


static D3DXVECTOR3				g_posUI_distance;						// ポリゴンの移動量

static int						g_UI_distance;						// スコア

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitUI_distance(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャーの初期化を行う？

	//テキストのほう
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			UI_DISTANCE_TEXT_GAME_FILE,					// ファイルの名前
			&g_pD3DTexture_text);					// 読み込むメモリのポインタ
	}

	//numのほう
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			UI_DISTANCE_NUM_GAME_FILE,					// ファイルの名前
			&g_pD3DTexture_num);					// 読み込むメモリのポインタ
	}

	g_posUI_distance = D3DXVECTOR3((float)UI_DISTANCE_POS_X, (float)UI_DISTANCE_POS_Y, 0.0f);
	g_UI_distance = 0;

	// 頂点情報の作成
	MakeVertexUI_distance();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitUI_distance(void)
{
	if (g_pD3DTexture_text != NULL)
	{	// テクスチャの開放
		g_pD3DTexture_text->Release();
		g_pD3DTexture_text = NULL;
	}

	if (g_pD3DTexture_num != NULL)
	{	// テクスチャの開放
		g_pD3DTexture_num->Release();
		g_pD3DTexture_num = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateUI_distance(void)
{
	// 毎フレーム実行される処理を記述する


	SetTextureUI_distance();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawUI_distance(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i;

	// 文字部
	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTexture_text);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &g_vertexWk_text[0], sizeof(VERTEX_2D));


	// 数字部
	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTexture_num);

	// スコア
	for (i = 0; i < UI_DISTANCE_DIGIT; i++)
	{
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &g_vertexWk_num[i][0], sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexUI_distance(void)
{
	int i;
	float habaX = UI_DISTANCE_SIZE_W;	// 数字の横幅

	// テキスト部
	// 頂点座標の設定
	g_vertexWk_text[0].vtx = D3DXVECTOR3(-habaX * -1 + g_posUI_distance.x, g_posUI_distance.y, 0.0f);
	g_vertexWk_text[1].vtx = D3DXVECTOR3(-habaX * -1 + g_posUI_distance.x + UI_DISTANCE_SIZE_W, g_posUI_distance.y, 0.0f);
	g_vertexWk_text[2].vtx = D3DXVECTOR3(-habaX * -1 + g_posUI_distance.x, g_posUI_distance.y + UI_DISTANCE_SIZE_H, 0.0f);
	g_vertexWk_text[3].vtx = D3DXVECTOR3(-habaX * -1 + g_posUI_distance.x + UI_DISTANCE_SIZE_W, g_posUI_distance.y + UI_DISTANCE_SIZE_H, 0.0f);

	// rhwの設定
	g_vertexWk_text[0].rhw =
		g_vertexWk_text[1].rhw =
		g_vertexWk_text[2].rhw =
		g_vertexWk_text[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWk_text[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk_text[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk_text[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk_text[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWk_text[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWk_text[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWk_text[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWk_text[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// 桁数分処理する
	for (i = 0; i < UI_DISTANCE_DIGIT; i++)
	{
		// 頂点座標の設定
		g_vertexWk_num[i][0].vtx.x = -habaX * i + g_posUI_distance.x;
		g_vertexWk_num[i][0].vtx.y = g_posUI_distance.y;
		g_vertexWk_num[i][0].vtx.z = 0.0f;
		g_vertexWk_num[i][1].vtx.x = -habaX * i + g_posUI_distance.x + UI_DISTANCE_SIZE_W;
		g_vertexWk_num[i][1].vtx.y = g_posUI_distance.y;
		g_vertexWk_num[i][1].vtx.z = 0.0f;
		g_vertexWk_num[i][2].vtx.x = -habaX * i + g_posUI_distance.x;
		g_vertexWk_num[i][2].vtx.y = g_posUI_distance.y + UI_DISTANCE_SIZE_H;
		g_vertexWk_num[i][2].vtx.z = 0.0f;
		g_vertexWk_num[i][3].vtx.x = -habaX * i + g_posUI_distance.x + UI_DISTANCE_SIZE_W;
		g_vertexWk_num[i][3].vtx.y = g_posUI_distance.y + UI_DISTANCE_SIZE_H;
		g_vertexWk_num[i][3].vtx.z = 0.0f;

		// rhwの設定
		g_vertexWk_num[i][0].rhw =
			g_vertexWk_num[i][1].rhw =
			g_vertexWk_num[i][2].rhw =
			g_vertexWk_num[i][3].rhw = 1.0f;

		// 反射光の設定
		g_vertexWk_num[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWk_num[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWk_num[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWk_num[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		g_vertexWk_num[i][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		g_vertexWk_num[i][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		g_vertexWk_num[i][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		g_vertexWk_num[i][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}
//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetTextureUI_distance(void)
{
	int i;
	int number = g_UI_distance;

	for (i = 0; i < UI_DISTANCE_DIGIT; i++)
	{
		// テクスチャ座標の設定
		float x = (float)(number % 10);
		g_vertexWk_num[i][0].tex = D3DXVECTOR2(0.1f * x, 0.0f);
		g_vertexWk_num[i][1].tex = D3DXVECTOR2(0.1f * (x + 1), 0.0f);
		g_vertexWk_num[i][2].tex = D3DXVECTOR2(0.1f * x, 1.0f);
		g_vertexWk_num[i][3].tex = D3DXVECTOR2(0.1f * (x + 1), 1.0f);
		number /= 10;
	}

}
//=============================================================================
// 距離データをセットする
// 引数:add :追加する点数。マイナスも可能、初期化などに。
//=============================================================================
void AddUI_distance(int add)
{
	g_UI_distance += add;
	if (g_UI_distance > UI_DISTANCE_MAX)
	{
		g_UI_distance = UI_DISTANCE_MAX;
	}

}
