//=============================================================================
//
// バレット処理 [Gameeffect.h]
// Author : 後地　賢人
//
//=============================================================================
#pragma once


// マクロ定義
#define EFFECT_UP_TEXTURE			_T("data/TEXTURE/Particle/pipo-sweetseffect001_480.png")	// 画像
#define EFFECT_DOWN_TEXTURE			_T("data/TEXTURE/Particle/pipo-btleffect102h.png")	// 画像

#define EFFECT_TEXTURE_SIZE_X	(600/2)		// テクスチャサイズ
#define EFFECT_TEXTURE_SIZE_Y	(600/2)		// 同上

#define EFFECT_UP_TEXTURE_PATTERN_DIVIDE_X	(5)	// アニメパターンのテクスチャ内分割数（X)
#define EFFECT_UP_TEXTURE_PATTERN_DIVIDE_Y	(3)	// アニメパターンのテクスチャ内分割数（Y)
#define EFFECT_UP_ANIM_PATTERN_NUM			(EFFECT_UP_TEXTURE_PATTERN_DIVIDE_X*EFFECT_UP_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define EFFECT_UP_TIME_ANIMATION			(4)	// アニメーションの切り替わるカウント

#define EFFECT_DOWN_TEXTURE_PATTERN_DIVIDE_X	(10)	// アニメパターンのテクスチャ内分割数（X)
#define EFFECT_DOWN_TEXTURE_PATTERN_DIVIDE_Y	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define EFFECT_DOWN_ANIM_PATTERN_NUM			(EFFECT_DOWN_TEXTURE_PATTERN_DIVIDE_X*EFFECT_DOWN_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define EFFECT_DOWN_TIME_ANIMATION			(4)	// アニメーションの切り替わるカウント


#define EFFECT_MAX						(10)	// バレットの最大数

//*****************************************************************************
// 構造体宣言
//*****************************************************************************

typedef struct	// バレット構造体
{
	bool			use;						// true:使用  false:未使用
	int				type;						// エフェクトの種類
	D3DXVECTOR3		pos;						// ポリゴンの移動量

	int				PatternAnim;				// アニメーションパターンナンバー
	int				CountAnim;					// アニメーションカウント

	//LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D		vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

} EFFECT;

enum EFFCT_GAME{
	UP,
	DOWN,
	EFFCT_GAME_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEffect(int type);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, int type);
EFFECT *GetEffect(void);


