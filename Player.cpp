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

#include "Framework\Input\input.h"

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
	ResourceManager::Instance()->LoadMesh("PlayerCream", "data/MODEL/Cream.x");

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

	cream = new PlayerCream();

	const D3DXVECTOR3 InitPos = { 0.0f, 0.0f, -20.0f };
	transform->SetPosition(InitPos);
	cream->SetPosition(InitPos);

	scaleCream = 0.0f;
}

/**************************************
�f�X�g���N�^
***************************************/
Player::~Player()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(cream);
}

/**************************************
�X�V����
***************************************/
void Player::Update()
{
	transform->Rotate(5.0f, Vector3::Right);

#ifdef _DEBUG
	float deltaValue = 0.0f;
	if (Keyboard::GetPress(DIK_1))
	{
		deltaValue = 0.05f;
	}
	else if (Keyboard::GetPress(DIK_2))
	{
		deltaValue = -0.05f;
	}
	scaleCream = Math::Clamp(0.0f, 100.0f, scaleCream + deltaValue);

	cream->SetScale({ 1.0f, scaleCream, scaleCream });
#endif
}

/**************************************
�`�揈��
***************************************/
void Player::Draw()
{
	transform->SetWorld();
	mesh->Draw();

	cream->Draw();
}

/**************************************
�R���X�g���N�^
***************************************/
PlayerCream::PlayerCream()
{
	mesh = new MeshContainer();
	ResourceManager::Instance()->GetMesh("PlayerCream", mesh);
}

/**************************************
�f�X�g���N�^
***************************************/
PlayerCream::~PlayerCream()
{
	SAFE_DELETE(mesh);
}

/**************************************
�`�揈��
***************************************/
void PlayerCream::Draw()
{
	transform->SetWorld();
	mesh->Draw();
}

/**************************************
��]����
***************************************/
void PlayerCream::Rotate(float degree)
{
	transform->Rotate(degree, Vector3::Right);
}
