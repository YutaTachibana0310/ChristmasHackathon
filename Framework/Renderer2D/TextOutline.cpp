//=====================================
//
// TextOutLine.cpp
// 機能:テキストアウトライン
// Author:GP12B332 21 立花雄太
//
//=====================================
#include "TextOutLine.h"

/**************************************
グローバル変数
***************************************/

/**************************************
コンストラクタ
***************************************/
TextViewer::OutLine::OutLine(const TextViewer * entity) :
	entity(entity),
	color(D3DXCOLOR(0.05f, 0.05f, 0.05f, 1.0f)),
	width(2)
{
}

/**************************************
デストラクタ
***************************************/
TextViewer::OutLine::~OutLine()
{
}

/**************************************
描画処理
***************************************/
void TextViewer::OutLine::Draw()
{
	RECT rect = entity->GetRect();

	rect.left -= width;
	entity->font->DrawTextA(NULL, entity->text.c_str(), -1, &rect, (WORD)entity->horizontal | (WORD)entity->vertical | DT_NOCLIP, color);

	rect.left += width * 2;
	entity->font->DrawTextA(NULL, entity->text.c_str(), -1, &rect, (WORD)entity->horizontal | (WORD)entity->vertical | DT_NOCLIP, color);

	rect.left -= width;
	rect.top -= width;
	entity->font->DrawTextA(NULL, entity->text.c_str(), -1, &rect, (WORD)entity->horizontal | (WORD)entity->vertical | DT_NOCLIP, color);

	rect.top += width * 2;
	entity->font->DrawTextA(NULL, entity->text.c_str(), -1, &rect, (WORD)entity->horizontal | (WORD)entity->vertical | DT_NOCLIP, color);
}

/**************************************
色設定処理
***************************************/
void TextViewer::OutLine::SetColor(const D3DXCOLOR & color)
{
	this->color = color;
}

/**************************************
太さ設定処理
***************************************/
void TextViewer::OutLine::SetWidth(int width)
{
	this->width = width;
}
