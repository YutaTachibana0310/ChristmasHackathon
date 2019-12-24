//=====================================
//
//Road.h
//機能:
//Author:GP12B332 21 立花雄太
//
//=====================================
#ifndef _ROAD_H_
#define _ROAD_H_

#include "main.h"

/**************************************
前方宣言
***************************************/
void InitRoad();
void UninitRoad();
void UpdateRoad();
void DrawRoad();

class MeshContainer;
class RoadEffect;
/**************************************
クラス定義
***************************************/
class RoadBG : public GameObject
{
public:
	RoadBG();
	~RoadBG();

	void Update();
	void Draw();

private:
	float offset;

	MeshContainer *mesh;
	RoadEffect *effect;
};
#endif