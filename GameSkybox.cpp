//=====================================
//
//GameSkybox.cpp
//機能:スカイボックス
//Author:GP12B332 21 立花雄太
//
//=====================================
#include "GameSkybox.h"
#include "Framework\Renderer3D\SkyBox.h"

/**************************************
グローバル変数
***************************************/
static SkyBox* skybox = nullptr;
static const char* TexName[] = {
	"data/TEXTURE/skybox/AnotherPlanet_Cam_Up.jpg",
	"data/TEXTURE/skybox/AnotherPlanet_Cam_Back.jpg",
	"data/TEXTURE/skybox/AnotherPlanet_Cam_Down.jpg",
	"data/TEXTURE/skybox/AnotherPlanet_Cam_Front.jpg",
	"data/TEXTURE/skybox/AnotherPlanet_Cam_Right.jpg",
	"data/TEXTURE/skybox/AnotherPlanet_Cam_Left.jpg",
};

/**************************************
初期化処理
***************************************/
void InitSkybox()
{
	skybox = new SkyBox({ 2000.0f, 2000.0f, 2000.0f });
	for (int i = 0; i < SkyBox::Surface::Max; i++)
	{
		skybox->LoadTexture(TexName[i], SkyBox::Surface(i));
	}
}

/**************************************
終了処理
***************************************/
void UninitSkybox()
{
	SAFE_DELETE(skybox);
}

/**************************************
更新処理
***************************************/
void UpdateSkybox()
{
	D3DXVECTOR3 rotation = skybox->GetRotation();
	rotation.y += 0.1f;
	skybox->SetRotatition(rotation);
}

/**************************************
描画処理
***************************************/
void DrawSkybox()
{
	skybox->Draw();
}
