//=====================================
//
//トランスフォームオブジェクト処理[TransformObject.cpp]
//Author:GP12A332 21 立花雄太
//
//=====================================
#include "TransformObject.h"
#include "../Resource/ResourceManager.h"

/**************************************
マクロ定義
***************************************/

/**************************************
コンストラクタ
***************************************/
TransformObject::TransformObject()
{
	ResourceManager::Instance()->LoadMesh("Transform", "data/MODEL/transform.x");
	
	mesh = new MeshContainer();
	ResourceManager::Instance()->GetMesh("Transform", mesh);
}

/**************************************
デストラクタ
***************************************/
TransformObject::~TransformObject()
{
	SAFE_DELETE(mesh);
}

/**************************************
描画処理
***************************************/
void TransformObject::Draw()
{
	transform->SetWorld();
	mesh->Draw();
}

/**************************************
指定の座標を向かせる
***************************************/
void TransformObject::LookAt(const D3DXVECTOR3 & target)
{
	transform->LookAt(target);
}
