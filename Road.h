//=====================================
//
//Road.h
//�@�\:
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#ifndef _ROAD_H_
#define _ROAD_H_

#include "main.h"

/**************************************
�O���錾
***************************************/
void InitRoad();
void UninitRoad();
void UpdateRoad();
void DrawRoad();

class MeshContainer;
class RoadEffect;
/**************************************
�N���X��`
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