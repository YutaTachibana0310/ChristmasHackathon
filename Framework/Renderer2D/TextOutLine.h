//=====================================
//
// TextOutLine.h
// 機能:テキストアウトライン
// Author:GP12B332 21 立花雄太
//
//=====================================
#ifndef _TEXTOUTLINE_H_
#define _TEXTOUTLINE_H_

#include "../../main.h"
#include "TextViewer.h"

/**************************************
前方宣言
***************************************/

/**************************************
クラス定義
***************************************/
class TextViewer::OutLine
{
public:
	OutLine(const TextViewer* entity);
	~OutLine();

	void Draw();

	void SetColor(const D3DXCOLOR& color);

	void SetWidth(int width);

private:
	const TextViewer* entity;

	D3DXCOLOR color;
	int width;
};

#endif