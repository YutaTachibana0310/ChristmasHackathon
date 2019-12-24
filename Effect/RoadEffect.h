//=====================================
//
//RoadEffect.h
//機能:RoadActor描画シェーダ
//Author:GP12B332 21 立花雄太
//
//=====================================
#ifndef _ROADEFFECT_H_
#define _ROADEFFECT_H_

#include "../main.h"
#include "../Framework/Effect/RendererEffect.h"

/**************************************
クラス定義
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