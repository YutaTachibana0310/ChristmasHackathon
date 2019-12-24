/**********************************************************************
[[メインプログラム(GameItemAi.cpp)]]
	作者：奥田　真規

	アイテムのAIに関するプログラム
***********************************************************************/
#include "GameItem_Ai.h"

//---------------------------------------------------------------------
//	マクロ定義(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	構造体、列挙体、共用体宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	プロトタイプ宣言(同cpp内限定)
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------

/*=====================================================================
	NO_Ai関数
	毎フレームzを減らすだけ
	戻り値:bool:プレイヤーに衝突；true
							してない:false
引数 Entity_ITEM *pData
=====================================================================*/
bool UpdateItemAI_NO(ENTITY_ITEM*pData)
{
	pData->pos.z -= ITEM_SPD;

	return false;
}