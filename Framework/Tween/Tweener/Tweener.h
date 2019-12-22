//=====================================
//
// Tweener.h
// 機能:トゥイーナー基底クラス
// Author:GP12B332 21 立花雄太
//
//=====================================
#ifndef _TWEENER_H_
#define _TWEENER_H_

#include "../../../main.h"
#include "../../Math/Easing.h"
#include <functional>

/**************************************
前方宣言
***************************************/
class Polygon2D;

/**************************************
BaseTweenerクラス
***************************************/
class BaseTweener
{
public:
	using Callback = std::function<void()>;

	/**************************************
	コンストラクタ
	***************************************/
	BaseTweener(int duration, EaseType type, Callback callback) :
		cntFrame(0),
		duration(duration),
		easeType(type),
		callback(callback)
	{
	}

	/**************************************
	デストラクタ
	***************************************/
	virtual ~BaseTweener() {}

	/**************************************
	終了判定
	***************************************/
	virtual bool IsFinished()
	{
		return cntFrame >= duration;
	}

	/**************************************
	更新処理　※純粋仮想関数
	***************************************/
	virtual void Update() = 0;

	/**************************************
	コールバック実行確認
	***************************************/
	void CheckCallback()
	{
		if (cntFrame < duration)
			return;

		if (callback == nullptr)
			return;

		callback();
	}

protected:
	int cntFrame;
	int duration;
	EaseType easeType;
	Callback callback;
};

/**************************************
Tweenerクラス
BaseTweenerを継承してテンプレート化
***************************************/
template<class T>
class Tweener : public BaseTweener
{
public:
	/**************************************
	コンストラクタ
	***************************************/
	Tweener(std::shared_ptr<T>& ref, int duration, EaseType type, Callback callback) :
		BaseTweener(duration, type, callback),
		reference(ref)
	{

	}

	/**************************************
	デストラクタ
	***************************************/
	virtual ~Tweener()
	{
		reference.reset();
	}

	/**************************************
	終了判定
	***************************************/
	bool IsFinished() override
	{
		if (reference.expired())
			return true;

		return BaseTweener::IsFinished();
	}

protected:
	std::weak_ptr<T> reference;
};

#endif