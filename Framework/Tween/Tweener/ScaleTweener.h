//=====================================
//
// ScaleTweener.h
// 機能:スケールトゥイーナー
// Author:GP12B332 21 立花雄太
//
//=====================================
#ifndef _SCALETWEENER_H_
#define _SCALETWEENER_H_

#include "../../../main.h"
#include "Tweener.h"

/**************************************
前方宣言
***************************************/

/**************************************
クラス定義
***************************************/
class ScaleTweener : public Tweener<Transform>
{
public:
	/**************************************
	コンストラクタ
	***************************************/
	ScaleTweener(std::shared_ptr<Transform>& ref, const D3DXVECTOR3& start, const D3DXVECTOR3& end, int duration, EaseType type, Callback callback) :
		Tweener<Transform>(ref, duration, type, callback),
		start(start),
		end(end)
	{

	}

	/**************************************
	更新処理
	***************************************/
	void Update()
	{
		cntFrame++;

		std::shared_ptr<Transform> transform = reference.lock();
		if (transform)
		{
			float t = (float)cntFrame / duration;
			transform->SetScale(Easing::EaseValue(t, start, end, easeType));
			CheckCallback();
		}
	}

private:
	D3DXVECTOR3 start, end;
};

#endif