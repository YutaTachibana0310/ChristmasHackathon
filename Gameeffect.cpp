//=============================================================================
//
// バレット処理 [Gameeffect.cpp]
// Author : 後地　賢人
//
//=============================================================================
#include "main.h"
#include "Gameeffect.h"
#include <stdio.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEffect(int no);
void SetTextureEffect(int no, int cntPattern);
void SetVertexEffect(int no);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureEffect_UP = NULL;		// テクスチャへのポリゴン
static LPDIRECT3DTEXTURE9		g_pD3DTextureEffect_DOWN = NULL;		// テクスチャへのポリゴン

static EFFECT					GameeffectWk[EFFECT_MAX];			// バレット構造体

int test;	//テスト用カウント
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEffect(int type)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();

	// UP用テクスチャの読み込み
	if (type == 0)
	{
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			EFFECT_UP_TEXTURE,						// ファイルの名前
			&g_pD3DTextureEffect_UP);							// 読み込むメモリのポインタ
	}

	// DOWN用テクスチャの読み込み
	if (type == 0)
	{
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			EFFECT_DOWN_TEXTURE,						// ファイルの名前
			&g_pD3DTextureEffect_DOWN);							// 読み込むメモリのポインタ
	}

	// バレットの初期化処理
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		GameeffectWk[i].use = false;								// 未使用（発射されていない弾）
		GameeffectWk[i].pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);		// 座標データを初期化
		GameeffectWk[i].PatternAnim = 0;							// アニメパターン番号をランダムで初期化
		GameeffectWk[i].CountAnim = 0;								// アニメカウントを初期化

		MakeVertexEffect(i);									// 頂点情報の作成
	}


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(void)
{
	if (g_pD3DTextureEffect_UP != NULL)
	{	// テクスチャの開放
		g_pD3DTextureEffect_UP->Release();
		g_pD3DTextureEffect_UP = NULL;
	}

	if (g_pD3DTextureEffect_DOWN != NULL)
	{	// テクスチャの開放
		g_pD3DTextureEffect_DOWN->Release();
		g_pD3DTextureEffect_DOWN = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEffect(void)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (GameeffectWk[i].use == true)		// 使用している状態なら更新する
		{
			// アニメーション
			GameeffectWk[i].CountAnim++;

			switch (GameeffectWk[i].type)
			{
			case UP:
				if ((GameeffectWk[i].CountAnim % EFFECT_UP_TIME_ANIMATION) == 0)
				{
					// パターンの切り替え
					GameeffectWk[i].PatternAnim = (GameeffectWk[i].PatternAnim + 1) % EFFECT_UP_ANIM_PATTERN_NUM;

					// テクスチャ座標を設定
					SetTextureEffect(i, GameeffectWk[i].PatternAnim);
				}
				SetVertexEffect(i);				// 移動後の座標で頂点を設定

				if (GameeffectWk[i].CountAnim >= EFFECT_UP_TIME_ANIMATION * EFFECT_UP_ANIM_PATTERN_NUM)
				{
					GameeffectWk[i].use = false;
					GameeffectWk[i].CountAnim = 0;
				}
				break;

			case DOWN:
				if ((GameeffectWk[i].CountAnim % EFFECT_DOWN_TIME_ANIMATION) == 0)
				{
					// パターンの切り替え
					GameeffectWk[i].PatternAnim = (GameeffectWk[i].PatternAnim + 1) % EFFECT_DOWN_ANIM_PATTERN_NUM;

					// テクスチャ座標を設定
					SetTextureEffect(i, GameeffectWk[i].PatternAnim);
				}
				SetVertexEffect(i);				// 移動後の座標で頂点を設定

				if (GameeffectWk[i].CountAnim >= EFFECT_DOWN_TIME_ANIMATION * EFFECT_DOWN_ANIM_PATTERN_NUM)
				{
					GameeffectWk[i].use = false;
					GameeffectWk[i].CountAnim = 0;
				}
				break;

			default:
				break;
			}
		}
	}

	test++;

	if (test < 121)
	{
		if (test % 120 == 0)
		{
			SetEffect(D3DXVECTOR3(500.0f, 500.0f, 500.0f), UP);
		}
		if (test % 60 == 0)
		{
			SetEffect(D3DXVECTOR3(800.0f, 800.0f, 800.0f), DOWN);
		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (GameeffectWk[i].use == true)			// 使用している状態なら更新する
		{
			switch (GameeffectWk[i].type)	// エフェクトのtypeに応じて表示するテクスチャを変える
			{
			case UP:
				// テクスチャの設定
				pDevice->SetTexture(0, g_pD3DTextureEffect_UP);

				// ポリゴンの描画
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, GameeffectWk[i].vertexWk, sizeof(VERTEX_2D));
				break;

			case DOWN:
				// テクスチャの設定
				pDevice->SetTexture(0, g_pD3DTextureEffect_DOWN);

				// ポリゴンの描画
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, GameeffectWk[i].vertexWk, sizeof(VERTEX_2D));
				break;


			default:
				break;
			}
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexEffect(int i)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertexEffect(i);

	// rhwの設定
	GameeffectWk[i].vertexWk[0].rhw =
		GameeffectWk[i].vertexWk[1].rhw =
		GameeffectWk[i].vertexWk[2].rhw =
		GameeffectWk[i].vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	GameeffectWk[i].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	GameeffectWk[i].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	GameeffectWk[i].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	GameeffectWk[i].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	GameeffectWk[i].vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	GameeffectWk[i].vertexWk[1].tex = D3DXVECTOR2(1.0f / EFFECT_UP_TEXTURE_PATTERN_DIVIDE_X, 0.0f);
	GameeffectWk[i].vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / EFFECT_UP_TEXTURE_PATTERN_DIVIDE_Y);
	GameeffectWk[i].vertexWk[3].tex = D3DXVECTOR2(1.0f / EFFECT_UP_TEXTURE_PATTERN_DIVIDE_X, 1.0f / EFFECT_UP_TEXTURE_PATTERN_DIVIDE_Y);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureEffect(int i, int cntPattern)
{
	int x;
	int y;
	float sizeX;
	float sizeY;

	switch (GameeffectWk[i].type)	// エフェクトのtypeに応じて表示するテクスチャを変える
	{
	case UP:
		// テクスチャ座標の設定
		 x = cntPattern % EFFECT_UP_TEXTURE_PATTERN_DIVIDE_X;
		 y = cntPattern / EFFECT_UP_TEXTURE_PATTERN_DIVIDE_X;
		 sizeX = 1.0f / EFFECT_UP_TEXTURE_PATTERN_DIVIDE_X;
		 sizeY = 1.0f / EFFECT_UP_TEXTURE_PATTERN_DIVIDE_Y;

		GameeffectWk[i].vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		GameeffectWk[i].vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		GameeffectWk[i].vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		GameeffectWk[i].vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		break;

	case DOWN:
		// テクスチャ座標の設定
		x = cntPattern % EFFECT_DOWN_TEXTURE_PATTERN_DIVIDE_X;
		y = cntPattern / EFFECT_DOWN_TEXTURE_PATTERN_DIVIDE_X;
		 sizeX = 1.0f / EFFECT_DOWN_TEXTURE_PATTERN_DIVIDE_X;
		 sizeY = 1.0f / EFFECT_DOWN_TEXTURE_PATTERN_DIVIDE_Y;

		GameeffectWk[i].vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		GameeffectWk[i].vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		GameeffectWk[i].vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		GameeffectWk[i].vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		break;

	}
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexEffect(int i)
{
	//頂点座標の設定
	GameeffectWk[i].vertexWk[0].vtx.x = GameeffectWk[i].pos.x - EFFECT_TEXTURE_SIZE_X;
	GameeffectWk[i].vertexWk[0].vtx.y = GameeffectWk[i].pos.y - EFFECT_TEXTURE_SIZE_Y;
	GameeffectWk[i].vertexWk[0].vtx.z = 0.0f;

	GameeffectWk[i].vertexWk[1].vtx.x = GameeffectWk[i].pos.x + EFFECT_TEXTURE_SIZE_X;
	GameeffectWk[i].vertexWk[1].vtx.y = GameeffectWk[i].pos.y - EFFECT_TEXTURE_SIZE_Y;
	GameeffectWk[i].vertexWk[1].vtx.z = 0.0f;

	GameeffectWk[i].vertexWk[2].vtx.x = GameeffectWk[i].pos.x - EFFECT_TEXTURE_SIZE_X;
	GameeffectWk[i].vertexWk[2].vtx.y = GameeffectWk[i].pos.y + EFFECT_TEXTURE_SIZE_Y;
	GameeffectWk[i].vertexWk[2].vtx.z = 0.0f;

	GameeffectWk[i].vertexWk[3].vtx.x = GameeffectWk[i].pos.x + EFFECT_TEXTURE_SIZE_X;
	GameeffectWk[i].vertexWk[3].vtx.y = GameeffectWk[i].pos.y + EFFECT_TEXTURE_SIZE_Y;
	GameeffectWk[i].vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// バレットの発射設定　typeにUPで得点エフェクト、DOWNで減点エフェクト
//=============================================================================
void SetEffect(D3DXVECTOR3 pos, int type)
{
	// もし未使用の弾が無かったら発射しない( =これ以上撃てないって事 )
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (GameeffectWk[i].use == false)		// 未使用状態のバレットを見つける
		{
			GameeffectWk[i].use = true;			// 使用状態へ変更する
			GameeffectWk[i].pos = pos;			// 座標をセット
			GameeffectWk[i].type = type;		// 弾の種類をセット


			//// SE再生
			//PlaySound(SOUND_LABEL_SE_shot000);
			return;							// 1発セットしたので終了する
		}
	}
}

//=============================================================================
// エネミー取得関数
//=============================================================================
EFFECT *GetEffect(void)
{
	return(&GameeffectWk[0]);
}

