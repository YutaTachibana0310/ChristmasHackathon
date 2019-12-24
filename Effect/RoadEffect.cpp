//=====================================
//
//RoadEffect.cpp
//�@�\:RoadActor�`��G�t�F�N�g
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "RoadEffect.h"
#include "../Framework/Resource/ResourceManager.h"

	/**************************************
	�R���X�g���N�^
	***************************************/
RoadEffect::RoadEffect()
{
	//�G�t�F�N�g
	ResourceManager::Instance()->GetEffect("data/EFFECT/RiverRenderer.cfx", effect);

	//�n���h���擾
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
�f�X�g���N�^
***************************************/
RoadEffect::~RoadEffect()
{
	SAFE_RELEASE(effect);
}

/**************************************
���[���h���ݒ菈��
***************************************/
void RoadEffect::SetWorld(const Transform & transform)
{
	D3DXMATRIX mtx = transform.GetMatrix();
	effect->SetMatrix(hWorld, &mtx);
}

/**************************************
�e�N�X�`��UV�ݒ菈��
***************************************/
void RoadEffect::SetUV(const D3DXVECTOR2 & uv)
{
	effect->SetFloatArray(hTexUV, (float*)&uv, 2);
}