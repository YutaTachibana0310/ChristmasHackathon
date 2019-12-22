//=====================================
//
// ViewerTweener.h
// 機能:ビューアトゥイーナー
// Author:GP12B332 21 立花雄太
//
//=====================================
#ifndef _VIEWERTWEENER_H_
#define _VIEWERTWEENER_H_

#include "../../../main.h"
#include "Tweener.h"
#include "../../Renderer2D/Polygon2D.h"

/**************************************
前方宣言
***************************************/

/**************************************
クラス定義
***************************************/
class ViewerTweener : public Tweener<Polygon2D>
{
public:
	/**************************************
	展開タイプ列挙子
	***************************************/
	enum class ExpandType : int
	{
		None,
		LeftToRight,
		RightToLeft,
		UpToDown,
		DownToUp,
		ToUpDown,
		ToLeftRight
	};

	/**************************************
	圧縮タイプ列挙子
	***************************************/
	enum class CloseType : int
	{
		None,
		LeftToRight,
		RightToLeft,
		UpToDown,
		DownToUp,
		FromUpDown,
		FromLeftRight
	};

	/**************************************
	コンストラクタ（展開タイプ）
	***************************************/
	ViewerTweener(std::shared_ptr<Polygon2D>& ref, ExpandType type, int duration, EaseType easeType, Callback callback) :
		Tweener(ref, duration, easeType, callback),
		expandType(type),
		closeType(CloseType::None),
		isFade(false),
		startAlpha(1.0f),
		endAlpha(1.0f)
	{

	}

	/**************************************
	コンストラクタ（圧縮タイプ）
	***************************************/
	ViewerTweener(std::shared_ptr<Polygon2D>& ref, CloseType type, int duration, EaseType easeType, Callback callback) :
		Tweener(ref, duration, easeType, callback),
		expandType(ExpandType::None),
		closeType(type),
		isFade(false),
		startAlpha(1.0f),
		endAlpha(1.0f)
	{

	}

	/**************************************
	コンストラクタ（フェード）
	***************************************/
	ViewerTweener(std::shared_ptr<Polygon2D>& ref, float startAlpha, float endAlpha, int duration, EaseType easeType, Callback callback) :
		Tweener(ref, duration, easeType, callback),
		expandType(ExpandType::None),
		closeType(CloseType::None),
		isFade(true),
		startAlpha(startAlpha),
		endAlpha(endAlpha)
	{

	}

	/**************************************
	更新処理
	***************************************/
	void Update()
	{
		cntFrame++;

		std::shared_ptr<Polygon2D> polygon = reference.lock();
		if (polygon)
		{
			if (expandType != ExpandType::None)
				_Expand(polygon);

			if (closeType != CloseType::None)
				_Close(polygon);

			if (isFade)
				_Fade(polygon);

			CheckCallback();
		}
	}


private:
	ExpandType expandType;
	CloseType closeType;

	bool isFade;
	float startAlpha, endAlpha;

	/**************************************
	展開のタイプ分岐処理
	***************************************/
	void _Expand(std::shared_ptr<Polygon2D>& polygon)
	{
		float t = (float)cntFrame / duration;

		if (expandType == ExpandType::DownToUp)
			_ExpandDownToUp(t, polygon);

		else if (expandType == ExpandType::LeftToRight)
			_ExpandLeftToRight(t, polygon);

		else if (expandType == ExpandType::RightToLeft)
			_ExpandRightToLeft(t, polygon);

		else if (expandType == ExpandType::ToLeftRight)
			_ExpandToLeftRight(t, polygon);

		else if (expandType == ExpandType::ToUpDown)
			_ExpandToUpDown(t, polygon);

		else if (expandType == ExpandType::UpToDown)
			_ExpandUpToDown(t, polygon);
	}

	/**************************************
	圧縮のタイプ分岐処理
	***************************************/
	void _Close(std::shared_ptr<Polygon2D>& polygon)
	{
		float t = (float)cntFrame / duration;

		if (closeType == CloseType::DownToUp)
			_CloseDownToUp(t, polygon);

		else if (closeType == CloseType::FromLeftRight)
			_CloseFromLeftRight(t, polygon);

		else if (closeType == CloseType::FromUpDown)
			_CloseFromUpDown(t, polygon);

		else if (closeType == CloseType::LeftToRight)
			_CloseLeftToRight(t, polygon);

		else if (closeType == CloseType::RightToLeft)
			_CloseRightToLeft(t, polygon);

		else if (closeType == CloseType::UpToDown)
			_CloseUpToDown(t, polygon);
	}

	/**************************************
	フェード処理
	***************************************/
	void _Fade(std::shared_ptr<Polygon2D>& polygon)
	{
		float t = (float)cntFrame / duration;
		float alpha = Easing::EaseValue(t, startAlpha, endAlpha, easeType);
		for (int i = 0; i < NUM_VERTEX; i++)
		{
			D3DXCOLOR color = polygon->vtxWk[i].diffuse;
			color.a = alpha;
			polygon->vtxWk[i].diffuse = color;
		}
	}

	/**************************************
	展開処理
	***************************************/
	void _ExpandLeftToRight(float t, std::shared_ptr<Polygon2D>& polygon)
	{
		float percentVtx = Easing::EaseValue(t, -1.0f, 1.0f, easeType);
		float widthU = polygon->vtxUV[1].x - polygon->vtxUV[0].x;
		float percentUV = Easing::EaseValue(t, -widthU, 0.0f, easeType);

		polygon->vtxPos[0] = D3DXVECTOR3(-polygon->vtxSize.x, -polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[1] = D3DXVECTOR3(polygon->vtxSize.x * percentVtx, -polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[2] = D3DXVECTOR3(-polygon->vtxSize.x, polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[3] = D3DXVECTOR3(polygon->vtxSize.x * percentVtx, polygon->vtxSize.y, 0.0f);

		polygon->vtxWk[0].tex = polygon->vtxUV[0];
		polygon->vtxWk[1].tex = D3DXVECTOR2(polygon->vtxUV[1].x + percentUV, polygon->vtxUV[1].y);
		polygon->vtxWk[2].tex = polygon->vtxUV[2];
		polygon->vtxWk[3].tex = D3DXVECTOR2(polygon->vtxUV[3].x + percentUV, polygon->vtxUV[3].y);
	}

	/**************************************
	展開処理
	***************************************/
	void _ExpandRightToLeft(float t, std::shared_ptr<Polygon2D>& polygon)
	{
		float percentVtx = Easing::EaseValue(t, -1.0f, 1.0f, easeType);
		float widthU = polygon->vtxUV[1].x - polygon->vtxUV[0].x;
		float percentUV = Easing::EaseValue(t, widthU, 0.0f, easeType);

		polygon->vtxPos[0] = D3DXVECTOR3(-polygon->vtxSize.x * percentVtx, -polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[1] = D3DXVECTOR3(polygon->vtxSize.x, -polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[2] = D3DXVECTOR3(-polygon->vtxSize.x * percentVtx, polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[3] = D3DXVECTOR3(polygon->vtxSize.x, polygon->vtxSize.y, 0.0f);

		polygon->vtxWk[0].tex = D3DXVECTOR2(polygon->vtxUV[0].x + percentUV, polygon->vtxUV[0].y);
		polygon->vtxWk[1].tex = polygon->vtxUV[1];
		polygon->vtxWk[2].tex = D3DXVECTOR2(polygon->vtxUV[2].x + percentUV, polygon->vtxUV[2].y);
		polygon->vtxWk[3].tex = polygon->vtxUV[3];
	}

	/**************************************
	展開処理
	***************************************/
	void _ExpandUpToDown(float t, std::shared_ptr<Polygon2D>& polygon)
	{
		float percentVtx = Easing::EaseValue(t, -1.0f, 1.0f, easeType);
		float widthV = polygon->vtxUV[3].y - polygon->vtxUV[0].y;
		float percentUV = Easing::EaseValue(t, -widthV, 0.0f, easeType);

		polygon->vtxPos[0] = D3DXVECTOR3(-polygon->vtxSize.x, -polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[1] = D3DXVECTOR3(polygon->vtxSize.x, -polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[2] = D3DXVECTOR3(-polygon->vtxSize.x, polygon->vtxSize.y * percentVtx, 0.0f);
		polygon->vtxPos[3] = D3DXVECTOR3(polygon->vtxSize.x, polygon->vtxSize.y  * percentVtx, 0.0f);

		polygon->vtxWk[0].tex = polygon->vtxUV[0];
		polygon->vtxWk[1].tex = polygon->vtxUV[1];
		polygon->vtxWk[2].tex = D3DXVECTOR2(polygon->vtxUV[2].x, polygon->vtxUV[2].y + percentUV);
		polygon->vtxWk[3].tex = D3DXVECTOR2(polygon->vtxUV[3].x, polygon->vtxUV[3].y + percentUV);
	}

	/**************************************
	展開処理
	***************************************/
	void _ExpandDownToUp(float t, std::shared_ptr<Polygon2D>& polygon)
	{
		float percentVtx = Easing::EaseValue(t, -1.0f, 1.0f, easeType);
		float widthV = polygon->vtxUV[3].y - polygon->vtxUV[0].y;
		float percentUV = Easing::EaseValue(t, widthV, 0.0f, easeType);

		polygon->vtxPos[0] = D3DXVECTOR3(-polygon->vtxSize.x, -polygon->vtxSize.y * percentVtx, 0.0f);
		polygon->vtxPos[1] = D3DXVECTOR3(polygon->vtxSize.x, -polygon->vtxSize.y * percentVtx, 0.0f);
		polygon->vtxPos[2] = D3DXVECTOR3(-polygon->vtxSize.x, polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[3] = D3DXVECTOR3(polygon->vtxSize.x, polygon->vtxSize.y, 0.0f);

		polygon->vtxWk[0].tex = D3DXVECTOR2(polygon->vtxUV[0].x, polygon->vtxUV[0].y + percentUV);
		polygon->vtxWk[1].tex = D3DXVECTOR2(polygon->vtxUV[1].x, polygon->vtxUV[1].y + percentUV);
		polygon->vtxWk[2].tex = polygon->vtxUV[2];
		polygon->vtxWk[3].tex = polygon->vtxUV[3];
	}

	/**************************************
	展開処理
	***************************************/
	void _ExpandToUpDown(float t, std::shared_ptr<Polygon2D>& polygon)
	{
		float percentVtx = Easing::EaseValue(t, 0.0f, 1.0f, easeType);
		float widthV = polygon->vtxUV[3].y - polygon->vtxUV[0].y;
		float percentUV = Easing::EaseValue(t, 0.5f * widthV, 0.0f, easeType);

		polygon->vtxPos[0] = D3DXVECTOR3(-polygon->vtxSize.x, -polygon->vtxSize.y * percentVtx, 0.0f);
		polygon->vtxPos[1] = D3DXVECTOR3( polygon->vtxSize.x, -polygon->vtxSize.y * percentVtx, 0.0f);
		polygon->vtxPos[2] = D3DXVECTOR3(-polygon->vtxSize.x,  polygon->vtxSize.y * percentVtx, 0.0f);
		polygon->vtxPos[3] = D3DXVECTOR3( polygon->vtxSize.x,  polygon->vtxSize.y * percentVtx, 0.0f);

		polygon->vtxWk[0].tex = D3DXVECTOR2(polygon->vtxUV[0].x, polygon->vtxUV[0].y + percentUV);
		polygon->vtxWk[1].tex = D3DXVECTOR2(polygon->vtxUV[1].x, polygon->vtxUV[1].y + percentUV);
		polygon->vtxWk[2].tex = D3DXVECTOR2(polygon->vtxUV[2].x, polygon->vtxUV[2].y - percentUV);
		polygon->vtxWk[3].tex = D3DXVECTOR2(polygon->vtxUV[3].x, polygon->vtxUV[3].y - percentUV);
	}

	/**************************************
	展開処理
	***************************************/
	void _ExpandToLeftRight(float t, std::shared_ptr<Polygon2D>& polygon)
	{
		float percentVtx = Easing::EaseValue(t, 0.0f, 1.0f, easeType);
		float widthU = polygon->vtxUV[1].x - polygon->vtxUV[0].x;
		float percentUV = Easing::EaseValue(t, 0.5f * widthU, 0.0f, easeType);

		polygon->vtxPos[0] = D3DXVECTOR3(-polygon->vtxSize.x * percentVtx, -polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[1] = D3DXVECTOR3( polygon->vtxSize.x * percentVtx, -polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[2] = D3DXVECTOR3(-polygon->vtxSize.x * percentVtx,  polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[3] = D3DXVECTOR3( polygon->vtxSize.x * percentVtx,  polygon->vtxSize.y, 0.0f);

		polygon->vtxWk[0].tex = D3DXVECTOR2(polygon->vtxUV[0].x + percentUV, polygon->vtxUV[0].y);
		polygon->vtxWk[1].tex = D3DXVECTOR2(polygon->vtxUV[1].x - percentUV, polygon->vtxUV[1].y);
		polygon->vtxWk[2].tex = D3DXVECTOR2(polygon->vtxUV[2].x + percentUV, polygon->vtxUV[2].y);
		polygon->vtxWk[3].tex = D3DXVECTOR2(polygon->vtxUV[3].x - percentUV, polygon->vtxUV[3].y);
	}

	/**************************************
	圧縮処理
	***************************************/
	void _CloseLeftToRight(float t, std::shared_ptr<Polygon2D>& polygon)
	{
		float percentVtx = Easing::EaseValue(t, 1.0f, -1.0f, easeType);
		float widthU = polygon->vtxUV[1].x - polygon->vtxUV[0].x;
		float percentUV = Easing::EaseValue(t, 0.0f, -widthU, easeType);

		polygon->vtxPos[0] = D3DXVECTOR3(-polygon->vtxSize.x, -polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[1] = D3DXVECTOR3(polygon->vtxSize.x * percentVtx, -polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[2] = D3DXVECTOR3(-polygon->vtxSize.x, polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[3] = D3DXVECTOR3(polygon->vtxSize.x * percentVtx, polygon->vtxSize.y, 0.0f);

		polygon->vtxWk[0].tex = polygon->vtxUV[0];
		polygon->vtxWk[1].tex = D3DXVECTOR2(polygon->vtxUV[1].x + percentUV, polygon->vtxUV[1].y);
		polygon->vtxWk[2].tex = polygon->vtxUV[2];
		polygon->vtxWk[3].tex = D3DXVECTOR2(polygon->vtxUV[3].x + percentUV, polygon->vtxUV[3].y);
	}

	/**************************************
	圧縮処理
	***************************************/
	void _CloseRightToLeft(float t, std::shared_ptr<Polygon2D>& polygon)
	{
		float percentVtx = Easing::EaseValue(t, 1.0f, -1.0f, easeType);
		float widthU = polygon->vtxUV[1].x - polygon->vtxUV[0].x;
		float percentUV = Easing::EaseValue(t, 0.0f, widthU, easeType);

		polygon->vtxPos[0] = D3DXVECTOR3(-polygon->vtxSize.x * percentVtx, -polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[1] = D3DXVECTOR3(polygon->vtxSize.x, -polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[2] = D3DXVECTOR3(-polygon->vtxSize.x * percentVtx, polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[3] = D3DXVECTOR3(polygon->vtxSize.x, polygon->vtxSize.y, 0.0f);

		polygon->vtxWk[0].tex = D3DXVECTOR2(polygon->vtxUV[0].x + percentUV, polygon->vtxUV[0].y);
		polygon->vtxWk[1].tex = polygon->vtxUV[1];
		polygon->vtxWk[2].tex = D3DXVECTOR2(polygon->vtxUV[2].x + percentUV, polygon->vtxUV[2].y);
		polygon->vtxWk[3].tex = polygon->vtxUV[3];
	}

	/**************************************
	圧縮処理
	***************************************/
	void _CloseUpToDown(float t, std::shared_ptr<Polygon2D>& polygon)
	{
		float percentVtx = Easing::EaseValue(t, 1.0f, -1.0f, easeType);
		float widthV = polygon->vtxUV[3].y - polygon->vtxUV[0].y;
		float percentUV = Easing::EaseValue(t, 0.0f, -widthV, easeType);

		polygon->vtxPos[0] = D3DXVECTOR3(-polygon->vtxSize.x, -polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[1] = D3DXVECTOR3(polygon->vtxSize.x, -polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[2] = D3DXVECTOR3(-polygon->vtxSize.x, polygon->vtxSize.y * percentVtx, 0.0f);
		polygon->vtxPos[3] = D3DXVECTOR3(polygon->vtxSize.x, polygon->vtxSize.y  * percentVtx, 0.0f);

		polygon->vtxWk[0].tex = polygon->vtxUV[0];
		polygon->vtxWk[1].tex = polygon->vtxUV[1];
		polygon->vtxWk[2].tex = D3DXVECTOR2(polygon->vtxUV[2].x, polygon->vtxUV[2].y + percentUV);
		polygon->vtxWk[3].tex = D3DXVECTOR2(polygon->vtxUV[3].x, polygon->vtxUV[3].y + percentUV);
	}

	/**************************************
	圧縮処理
	***************************************/
	void _CloseDownToUp(float t, std::shared_ptr<Polygon2D>& polygon)
	{
		float percentVtx = Easing::EaseValue(t, 1.0f, -1.0f, easeType);
		float widthV = polygon->vtxUV[3].y - polygon->vtxUV[0].y;
		float percentUV = Easing::EaseValue(t, 0.0f, widthV, easeType);

		polygon->vtxPos[0] = D3DXVECTOR3(-polygon->vtxSize.x, -polygon->vtxSize.y * percentVtx, 0.0f);
		polygon->vtxPos[1] = D3DXVECTOR3(polygon->vtxSize.x, -polygon->vtxSize.y * percentVtx, 0.0f);
		polygon->vtxPos[2] = D3DXVECTOR3(-polygon->vtxSize.x, polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[3] = D3DXVECTOR3(polygon->vtxSize.x, polygon->vtxSize.y, 0.0f);

		polygon->vtxWk[0].tex = D3DXVECTOR2(polygon->vtxUV[0].x, polygon->vtxUV[0].y + percentUV);
		polygon->vtxWk[1].tex = D3DXVECTOR2(polygon->vtxUV[1].x, polygon->vtxUV[1].y + percentUV);
		polygon->vtxWk[2].tex = polygon->vtxUV[2];
		polygon->vtxWk[3].tex = polygon->vtxUV[3];
	}

	/**************************************
	圧縮処理
	***************************************/
	void _CloseFromUpDown(float t, std::shared_ptr<Polygon2D>& polygon)
	{
		float percentVtx = Easing::EaseValue(t, 1.0f, 0.0f, easeType);
		float widthV = polygon->vtxUV[3].y - polygon->vtxUV[0].y;
		float percentUV = Easing::EaseValue(t, 0.0f, 0.5f * widthV, easeType);

		polygon->vtxPos[0] = D3DXVECTOR3(-polygon->vtxSize.x, -polygon->vtxSize.y * percentVtx, 0.0f);
		polygon->vtxPos[1] = D3DXVECTOR3(polygon->vtxSize.x, -polygon->vtxSize.y * percentVtx, 0.0f);
		polygon->vtxPos[2] = D3DXVECTOR3(-polygon->vtxSize.x, polygon->vtxSize.y * percentVtx, 0.0f);
		polygon->vtxPos[3] = D3DXVECTOR3(polygon->vtxSize.x, polygon->vtxSize.y * percentVtx, 0.0f);

		polygon->vtxWk[0].tex = D3DXVECTOR2(polygon->vtxUV[0].x, polygon->vtxUV[0].y + percentUV);
		polygon->vtxWk[1].tex = D3DXVECTOR2(polygon->vtxUV[1].x, polygon->vtxUV[1].y + percentUV);
		polygon->vtxWk[2].tex = D3DXVECTOR2(polygon->vtxUV[2].x, polygon->vtxUV[2].y - percentUV);
		polygon->vtxWk[3].tex = D3DXVECTOR2(polygon->vtxUV[3].x, polygon->vtxUV[3].y - percentUV);
	}

	/**************************************
	圧縮処理
	***************************************/
	void _CloseFromLeftRight(float t, std::shared_ptr<Polygon2D>& polygon)
	{
		float percentVtx = Easing::EaseValue(t, 1.0f, 0.0f, easeType);
		float widthU = polygon->vtxUV[1].x - polygon->vtxUV[0].x;
		float percentUV = Easing::EaseValue(t, 0.0f, 0.5f * widthU, easeType);

		polygon->vtxPos[0] = D3DXVECTOR3(-polygon->vtxSize.x * percentVtx, -polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[1] = D3DXVECTOR3(polygon->vtxSize.x * percentVtx, -polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[2] = D3DXVECTOR3(-polygon->vtxSize.x * percentVtx, polygon->vtxSize.y, 0.0f);
		polygon->vtxPos[3] = D3DXVECTOR3(polygon->vtxSize.x * percentVtx, polygon->vtxSize.y, 0.0f);

		polygon->vtxWk[0].tex = D3DXVECTOR2(polygon->vtxUV[0].x + percentUV, polygon->vtxUV[0].y);
		polygon->vtxWk[1].tex = D3DXVECTOR2(polygon->vtxUV[1].x - percentUV, polygon->vtxUV[1].y);
		polygon->vtxWk[2].tex = D3DXVECTOR2(polygon->vtxUV[2].x + percentUV, polygon->vtxUV[2].y);
		polygon->vtxWk[3].tex = D3DXVECTOR2(polygon->vtxUV[3].x - percentUV, polygon->vtxUV[3].y);
	}
};

#endif