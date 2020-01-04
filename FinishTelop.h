//=====================================
//
// FinishTelop.h
// �@�\:�t�B�j�b�V���e���b�v
// Author:GP12B332 21 ���ԗY��
//
//=====================================
#ifndef _FINISHTELOP_H_
#define _FINISHTELOP_H_

#include "main.h"
#include "Framework/Renderer2D/Polygon2D.h"

#include <functional>

/**************************************
�O���錾
***************************************/

/**************************************
�N���X��`
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