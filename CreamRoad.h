//=====================================
//
//CreamRoad.h
//機能:
//Author:GP12B332 21 立花雄太
//
//=====================================
#ifndef _CREAMROAD_H_
#define _CREAMROAD_H_

#include "main.h"

/**************************************
前方宣言
***************************************/
class MeshContainer;

void InitCreamRoad();
void UninitCreamRoad();
void UpdateCreamRoad();
void DrawCreamRoad();

/**************************************
クラス定義
***************************************/
class CreamRoad : public GameObject
{
public:
	CreamRoad();
	~CreamRoad();

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	MeshContainer * mesh;
};
#endif