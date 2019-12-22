//=====================================
//
//GameSkybox.cpp
//�@�\:�X�J�C�{�b�N�X
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "GameSkybox.h"
#include "Framework\Renderer3D\SkyBox.h"

/**************************************
�O���[�o���ϐ�
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
����������
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
�I������
***************************************/
void UninitSkybox()
{
	SAFE_DELETE(skybox);
}

/**************************************
�X�V����
***************************************/
void UpdateSkybox()
{
	D3DXVECTOR3 rotation = skybox->GetRotation();
	rotation.y += 0.1f;
	skybox->SetRotatition(rotation);
}

/**************************************
�`�揈��
***************************************/
void DrawSkybox()
{
	skybox->Draw();
}
