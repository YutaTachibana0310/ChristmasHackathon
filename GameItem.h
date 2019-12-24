/**********************************************************************
[[ゲームアイテムヘッダー(GameItem.h)]]
	作者：奥田　真規

	GameItem.cppに関するヘッダー
***********************************************************************/
#pragma once
//#include <Windows.h>
//#include <d3dx9.h>
#include "main.h"

#define ITEM_SIZE_X		(20.0f/2)	// アイテムのサイズ指定(x)
#define ITEM_SIZE_Y		(20.0f/2)	// (y)

// アイテムスピード
#define ITEM_SPD	(0.4f)

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言 (他cppでも使用する)
//---------------------------------------------------------------------

// テクスチャれきょ型
enum ITEM_TEX_TYPE {
	NO_TEX,
	NANKA_TEX,
	MAX_TEXTYPE
};

// Ai列挙型
enum ITEM_AI_TYPE {
	NO_AI,

	MAX_AITYPE
};

// アイテムデータ構造体
typedef struct ENTITY_ITEM {
	bool				isUse;								// 使用可能かどうか
	LPDIRECT3DTEXTURE9	pTex;								// アイテムのテクスチャ
	D3DXVECTOR3			pos;								// 位置
	D3DXVECTOR3			rot;								// 回転
	D3DXVECTOR3			scl;								// スケール（多分1.0f）
	bool				(*UpdateEachGameItem)(ENTITY_ITEM*);// アイテムごとの更新関数ポインタ
	bool				bPlus;								// そのアイテムを拾うとﾌﾟﾚｲﾔｰにプラス(true)マイナス(false)
}ENTITY_ITEM;
//---------------------------------------------------------------------
//	プロトタイプ宣言 (他cppでも使用する)
//---------------------------------------------------------------------
void StartGameItemTime(void);
HRESULT InitGameItem(void);
void UpdateGameItem(void);
void DrawGameItem(void);
void UninitGameItem(void);