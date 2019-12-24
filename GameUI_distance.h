//=============================================================================
//
// スコア処理 [GameUI_distance.h]
// Author : 後地　賢人
//
//=============================================================================
#pragma once

// マクロ定義
#define UI_DISTANCE_NUM_GAME_FILE		_T("data/TEXTURE/UI/num1.png")	// サンプル用画像
#define UI_DISTANCE_TEXT_GAME_FILE		_T("data/TEXTURE/UI/UI_meter.png")	// サンプル用画像

#define UI_DISTANCE_SIZE_W		(100)		// テクスチャサイズ
#define UI_DISTANCE_SIZE_H		(UI_DISTANCE_SIZE_W*2)		// 同上
#define UI_DISTANCE_POS_X			(1500)		// ポリゴンの初期位置X
#define UI_DISTANCE_POS_Y			(800)		// 同上
#define UI_DISTANCE_MAX			(99999)		// スコアの最大値
#define UI_DISTANCE_DIGIT			(4)			// 桁数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitUI_distance(int type);
void UninitUI_distance(void);
void UpdateUI_distance(void);
void DrawUI_distance(void);

void AddUI_distance(int add);

