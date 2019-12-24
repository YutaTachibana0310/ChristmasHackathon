//=====================================
//
//RoadEffect.cpp
//機能:RoadActor描画エフェクト
//Author:GP12B332 21 立花雄太
//
//=====================================
#include "RoadEffect.h"
#include "../Framework/Resource/ResourceManager.h"

	/**************************************
	コンストラクタ
	***************************************/
RoadEffect::RoadEffect()
{
	//エフェクト
	ResourceManager::Instance()->GetEffect("data/EFFECT/RiverRenderer.cfx", effect);

	//ハンドル取得
	hWorld = effect->GetParameterByName(0, "mtxWorld");
	hView = effect->GetParameterByName(0, "mtxView");
	hProjection = effect->GetParameterByName(0, "mtxProjection");

	hMatDiffuse = effect->GetParameterByName(0, "materialDiffuse");
	hMatAmbient = effect->GetParameterByName(0, "materialAmbient");
	hMatSpecular = effect->GetParameterByName(0, "materialSpecular");

	hLightDirection = effect->GetParameterByName(0, "lightDirection");
	hLightDiffuse = effect->GetParameterByName(0, "lightDiffuse");
	hLightAmbient = effect->GetParameterByName(0, "lightAmbient");
	hLightSpecular = effect->GetParameterByName(0, "lightSpecular");

	hTexUV = effect->GetParameterByName(0, "texUV");

	effect->SetTechnique("tech");
}

/**************************************
デストラクタ
***************************************/
RoadEffect::~RoadEffect()
{
	SAFE_RELEASE(effect);
}

/**************************************
ワールド情報設定処理
***************************************/
void RoadEffect::SetWorld(const Transform & transform)
{
	D3DXMATRIX mtx = transform.GetMatrix();
	effect->SetMatrix(hWorld, &mtx);
}

/**************************************
テクスチャUV設定処理
***************************************/
void RoadEffect::SetUV(const D3DXVECTOR2 & uv)
{
	effect->SetFloatArray(hTexUV, (float*)&uv, 2);
}