//=====================================
//
//Player.cpp
//�@�\:
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "Player.h"
#include "Framework\Resource\ResourceManager.h"
#include "Framework\Renderer3D\MeshContainer.h"

/**************************************
�O���[�o���ϐ�
***************************************/
static Player* player = nullptr;

/**************************************
������
***************************************/
void InitPlayer()
{
	ResourceManager::Instance()->LoadMesh("Player", "data/MODEL/Cake.x");

	player = new Player();
}

/**************************************
�I��
***************************************/
void UninitPlayer()
{
	SAFE_DELETE(player);
}

/**************************************
�X�V
***************************************/
void UpdatePlayer()
{
	player->Update();
}

/**************************************
�`��
***************************************/
void DrawPlayer()
{
	player->Draw();
}

/**************************************
�R���X�g���N�^
***************************************/
Player::Player()
{
	mesh = new MeshContainer();
	ResourceManager::Instance()->GetMesh("Player", mesh);

	transform->SetPosition({ 0.0f, 0.0f, -20.0f });
}

/**************************************
�f�X�g���N�^
***************************************/
Player::~Player()
{
	SAFE_DELETE(mesh);
}

/**************************************
�X�V����
***************************************/
void Player::Update()
{
	transform->Rotate(5.0f, Vector3::Right);
}

/**************************************
�`�揈��
***************************************/
void Player::Draw()
{
	transform->SetWorld();
	mesh->Draw();
}
