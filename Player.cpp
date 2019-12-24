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
#include "Framework\Tool\DebugWindow.h"
#include "Framework\Tween\Tween.h"

#include "Framework\Input\input.h"
#include "Framework\Collider\BoxCollider3D.h"

/**************************************
�O���[�o���ϐ�
***************************************/
static Player* player = nullptr;
static const D3DXVECTOR3 InitPos = { 0.0f, 0.0f, -20.0f };
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
�T�C�Y�擾
***************************************/
float GetPlayerSize()
{
	return player->GetSize();
}

/**************************************
�T�C�Y�擾
***************************************/
D3DXVECTOR3 GetPlayerPosition()
{
	return player->GetPosition();
}

/**************************************
�R���X�g���N�^
***************************************/
Player::Player() :
	isHitCream(false),
	currentLane(0)
{
	mesh = new MeshContainer();
	ResourceManager::Instance()->GetMesh("Player", mesh);

	cream = new PlayerCream();

	transform->SetPosition(InitPos);

	scaleCream = 0.0f;

	collider = BoxCollider3D::Create("Player", transform);
	collider->SetSize({ 6.0f, 5.0f, 1.0f });
	collider->AddObserver(this);
}

/**************************************
�f�X�g���N�^
***************************************/
Player::~Player()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(cream);
	collider.reset();
}

/**************************************
�X�V����
***************************************/
void Player::Update()
{
	transform->Rotate(5.0f, Vector3::Right);

	//�N���[�����q�b�g���Ă�����傫���Ȃ�
	//���Ă��Ȃ���Ώ�����
	//�t���O�𖈃t���[�����낷
	float deltaSize = isHitCream ? 0.05f : -0.02f;
	scaleCream = Math::Clamp(0.95f, 25.0f, scaleCream + deltaSize);
	cream->SetScale({ 1.0f, scaleCream, scaleCream });
	isHitCream = false;

	//�ړ�
	if (!inMoving)
	{
		float inputX = Input::GetTriggerHorizontal(0);
		
		if (currentLane < 1 && inputX > 0.0f)
		{
			inMoving = true;
			currentLane++;

			Tween::Move(*this, { currentLane * 15.0f, 0.0f, -20.0f }, 20, EaseType::OutCubic, [this]()
			{
				inMoving = false;
			});
		}

		if (currentLane > -1 && inputX < 0.0f)
		{
			inMoving = true;
			currentLane--;

			Tween::Move(*this, { currentLane * 15.0f, 0.0f, -20.0f }, 20, EaseType::OutCubic, [this]()
			{
				inMoving = false;
			});
		}
	}
}

/**************************************
�`�揈��
***************************************/
void Player::Draw()
{
	transform->SetWorld();
	mesh->Draw();

	cream->Draw(*transform);

	collider->Draw();
}

/**************************************
�����蔻��
***************************************/
void Player::OnColliderHit(ColliderObserver *other)
{
	Debug::Log("Hit Player");
	isHitCream = true;
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
void PlayerCream::Draw(const Transform& parent)
{
	D3DXMATRIX mtxParent = parent.GetMatrix();
	transform->SetWorld(&mtxParent);
	mesh->Draw();
}
