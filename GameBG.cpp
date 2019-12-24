//=====================================
//
//GameBG.cpp
//�@�\:
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "GameBG.h"
#include "Framework\Renderer2D\Polygon2D.h"

/**************************************
�O���[�o���ϐ�
***************************************/
Polygon2D *polygon = nullptr;

/**************************************
������
***************************************/
void InitGameBG()
{
	polygon = new Polygon2D();
	
	polygon->SetSize({ (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT });
	polygon->SetPosition({ SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f });
	polygon->LoadTexture("data/TEXTURE/gameBG.jpg");
}

/**************************************
�I������
***************************************/
void UninitGameBG()
{
	SAFE_DELETE(polygon);
}

/**************************************
�`��
***************************************/
void DrawGameBG()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	polygon->Draw();

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
