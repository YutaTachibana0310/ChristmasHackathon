//=============================================================================
//
// スコア処理 [GameUI_size.h]
// Author : 後地　賢人
//
//=============================================================================
#pragma once

// マクロ定義
#define UI_SIZE_NUM_GAME_FILE		_T("data/TEXTURE/UI/num1.png")	// 大きさ用の数字用
#define UI_SIZE_TEXT_GAME_FILE		_T("data/TEXTURE/UI/UI_size.png")	// 「大きさ」の文字表示用

#define UI_SIZE_SIZE_W		(100)		// テクスチャサイズ
#define UI_SIZE_SIZE_H		(UI_SIZE_SIZE_W*2)		// 同上
#define UI_SIZE_POS_X			(900)		// ポリゴンの初期位置X
#define UI_SIZE_POS_Y			(20)		// 同上
#define UI_SIZE_MAX			(999)		// スコアの最大値
#define UI_SIZE_DIGIT			(3)			// 桁数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitUI_size(int type);
void UninitUI_size(void);
void UpdateUI_size(void);
void DrawUI_size(void);

void AddUI_size(int add);

