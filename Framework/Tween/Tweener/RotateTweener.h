//=====================================
//
// RotateTweener.h
// 機能:回転トゥイーナー
// Author:GP12B332 21 立花雄太
//
//=====================================
#ifndef _ROTATETWEENER_H_
#define _ROTATETWEENER_H_

#include "../../../main.h"
#include "Tweener.h"
#include "../../Math/Quaternion.h"

/**************************************
前方宣言
***************************************/

/**************************************
クラス定義
***************************************/
class RotateTweener : public Tweener<Transform>
{
public:
	/**************************************
	コンストラクタ
	***************************************/
	RotateTweener(std::shared_ptr<Transform>& ref, const D3DXVECTOR3& start, const D3DXVECTOR3& end, int duration, EaseType type, Callback callback) :
		Tweener<Transform>(ref, duration, type, callback),
		start(Quaternion::ToQuaternion(start)),
		end(Quaternion::ToQuaternion(end))
	{
	}

	/**************************************
	コンストラクタ
	***************************************/
	RotateTweener(std::shared_ptr<Transform>& ref, const D3DXQUATERNION& start, const D3DXQUATERNION& end, int duration, EaseType type, Callback callback) :
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
			float t = Easing::EaseValue((float)cntFrame / duration, 0.0f, 1.0f, easeType);
			D3DXQUATERNION quaternion;
			D3DXQuaternionSlerp(&quaternion, &start, &end, t);
			transform->SetRotation(quaternion);
			CheckCallback();
		}
	}

private:
	D3DXQUATERNION start, end;
};

#endif