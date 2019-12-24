//=====================================
//
//RoadEffect.h
//�@�\:RoadActor�`��V�F�[�_
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#ifndef _ROADEFFECT_H_
#define _ROADEFFECT_H_

#include "../main.h"
#include "../Framework/Effect/RendererEffect.h"

/**************************************
�N���X��`
***************************************/
class RoadEffect : public RendererEffect
{
public:
	RoadEffect();
	~RoadEffect();

	void SetWorld(const Transform& transform);
	void SetUV(const D3DXVECTOR2& uv);

private:
	D3DXHANDLE hTexUV;
};

#endif