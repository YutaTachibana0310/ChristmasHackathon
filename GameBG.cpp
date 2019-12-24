//=====================================
//
//GameBG.cpp
//機能:
//Author:GP12B332 21 立花雄太
//
//=====================================
#include "GameBG.h"
#include "Framework\Renderer2D\Polygon2D.h"

/**************************************
グローバル変数
***************************************/
Polygon2D *polygon = nullptr;

/**************************************
初期化
***************************************/
void InitGameBG()
{
	polygon = new Polygon2D();
	
	polygon->SetSize({ (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT });
	polygon->SetPosition({ SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f });
	polygon->LoadTexture("data/TEXTURE/gameBG.jpg");
}

/**************************************
終了処理
***************************************/
void UninitGameBG()
{
	SAFE_DELETE(polygon);
}

/**************************************
描画
***************************************/
void DrawGameBG()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	polygon->Draw();

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
