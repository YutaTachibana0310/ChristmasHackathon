//=====================================
//
//CreamRoad.h
//�@�\:
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#ifndef _CREAMROAD_H_
#define _CREAMROAD_H_

#include "main.h"

/**************************************
�O���錾
***************************************/
class MeshContainer;
class BoxCollider3D;

void InitCreamRoad();
void UninitCreamRoad();
void UpdateCreamRoad();
void DrawCreamRoad();

/**************************************
�N���X��`
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
	std::shared_ptr<BoxCollider3D> collider;
};
#endif