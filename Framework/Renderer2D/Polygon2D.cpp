//=====================================
//
//ポリゴン2D処理[Polygon2D.cpp]
//Author:GP12A332 21 立花雄太
//
//=====================================
#include "Polygon2D.h"
#include "../Resource/ResourceManager.h"

/**************************************
マクロ定義
***************************************/
#define POLYGON2D_DEFAULT_SIZE		(20.0f)

/**************************************
コンストラクタ
***************************************/
Polygon2D::Polygon2D() :
	texture(NULL)
{
	SetSize({ POLYGON2D_DEFAULT_SIZE, POLYGON2D_DEFAULT_SIZE });
	SetUV(0.0f, 0.0f, 1.0f, 1.0f);

	vtxWk[0].diffuse =
		vtxWk[1].diffuse =
		vtxWk[2].diffuse =
		vtxWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	vtxWk[0].rhw =
		vtxWk[1].rhw =
		vtxWk[2].rhw =
		vtxWk[3].rhw = 1.0f;
}

/**************************************
デストラクタ
***************************************/
Polygon2D::~Polygon2D()
{
	SAFE_RELEASE(texture);
}

/**************************************
描画処理
***************************************/
void Polygon2D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture);

	pDevice->SetFVF(FVF_VERTEX_2D);

	SetVertex();

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vtxWk, sizeof(VERTEX_2D));
}

/**************************************
サイズセット処理
***************************************/
void Polygon2D::SetSize(const D3DXVECTOR2& size)
{
	vtxPos[0] = D3DXVECTOR3(-size.x * 1.0f, -size.y * 1.0f, 0.0f);
	vtxPos[1] = D3DXVECTOR3( size.x * 1.0f, -size.y * 1.0f, 0.0f);
	vtxPos[2] = D3DXVECTOR3(-size.x * 1.0f,  size.y * 1.0f, 0.0f);
	vtxPos[3] = D3DXVECTOR3( size.x * 1.0f,  size.y * 1.0f, 0.0f);

	vtxSize = size;
}

/**************************************
カラーセット処理
***************************************/
void Polygon2D::SetColor(D3DXCOLOR color)
{
	vtxWk[0].diffuse =
		vtxWk[1].diffuse =
		vtxWk[2].diffuse =
		vtxWk[3].diffuse = color;
}

/**************************************
UV座標設定処理
***************************************/
void Polygon2D::SetUV(float left, float top, float width, float height)
{
	vtxUV[0] = D3DXVECTOR2(left, top);
	vtxUV[1] = D3DXVECTOR2(left + width, top);
	vtxUV[2] = D3DXVECTOR2(left, top + height);
	vtxUV[3] = D3DXVECTOR2(left + width, top + height);

	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vtxWk[i].tex = vtxUV[i];
	}
}

/**************************************
テクスチャ読み込み処理
***************************************/
void Polygon2D::LoadTexture(const char* path)
{
	SAFE_RELEASE(texture);

	ResourceManager::Instance()->GetTexture(path, texture);
}

/**************************************
頂点座標設定処理
***************************************/
void Polygon2D::SetVertex()
{
	D3DXMATRIX mtxTransform = transform->GetMatrix();

	for (int i = 0; i < NUM_VERTEX; i++)
	{
		D3DXVec3TransformCoord(&vtxWk[i].vtx, &vtxPos[i], &mtxTransform);
	}
}