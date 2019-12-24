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

#include "Effect\RoadEffect.h"

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

	effect = new RoadEffect();

	transform->SetScale({ 1.0f, 1.0f, 10.0f });
}

/**************************************

***************************************/
RoadBG::~RoadBG()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(effect);
}

/**************************************

***************************************/
void RoadBG::Update()
{
	offset = Math::WrapAround(0.0f, 1.0f, offset += 0.05f);
	effect->SetUV(	{ 0.0f, offset });
}

/**************************************

***************************************/
void RoadBG::Draw()
{
	effect->SetWorld(*transform);

	effect->Begin();
	effect->BeginPass(0);

	mesh->Draw(*effect);

	effect->EndPass();
	effect->End();
}
