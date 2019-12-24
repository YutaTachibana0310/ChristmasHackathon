//=====================================
//
//Road.cpp
//�@�\:
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "Road.h"
#include "Framework\Resource\ResourceManager.h"
#include "Framework\Renderer3D\MeshContainer.h"

/**************************************
�O���[�o���ϐ�
***************************************/
static RoadBG *road = nullptr;

/**************************************
������
***************************************/
void InitRoad()
{
	ResourceManager::Instance()->LoadMesh("Road", "data/MODEL/Road.x");

	road = new RoadBG();
}

/**************************************

***************************************/
void UninitRoad()
{
	SAFE_DELETE(road);
}

/**************************************

***************************************/
void UpdateRoad()
{
	road->Update();
}

/**************************************

***************************************/
void DrawRoad()
{
	road->Draw();
}

/**************************************

***************************************/
RoadBG::RoadBG()
{
	mesh = new MeshContainer();
	ResourceManager::Instance()->GetMesh("Road", mesh);


}

/**************************************

***************************************/
RoadBG::~RoadBG()
{
	SAFE_DELETE(mesh);
}

/**************************************

***************************************/
void RoadBG::Update()
{
}

/**************************************

***************************************/
void RoadBG::Draw()
{
	transform->SetWorld();
	mesh->Draw();
}
