//=====================================
//
// FinishTelop.h
// 機能:フィニッシュテロップ
// Author:GP12B332 21 立花雄太
//
//=====================================
#ifndef _FINISHTELOP_H_
#define _FINISHTELOP_H_

#include "main.h"
#include "Framework/Renderer2D/Polygon2D.h"

#include <functional>

/**************************************
前方宣言
***************************************/

/**************************************
クラス定義
***************************************/
class FinishTelop : public Polygon2D
{
public:
	FinishTelop();
	~FinishTelop();

	void Set(std::function<void()> callback);

	void OnFinishTelopIn();

private:
	std::function<void()> onFinishTelopOut;
};

#endif