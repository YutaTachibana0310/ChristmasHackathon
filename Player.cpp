//=====================================
//
//Player.cpp
//機能:
//Author:GP12B332 21 立花雄太
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
グローバル変数
***************************************/
static Player* player = nullptr;
static const D3DXVECTOR3 InitPos = { 0.0f, 0.0f, -20.0f };
/**************************************
初期化
***************************************/
void InitPlayer()
{
	ResourceManager::Instance()->LoadMesh("Player", "data/MODEL/Cake.x");
	ResourceManager::Instance()->LoadMesh("PlayerCream", "data/MODEL/Cream.x");

	player = new Player();
}

/**************************************
終了
***************************************/
void UninitPlayer()
{
	SAFE_DELETE(player);
}

/**************************************
更新
***************************************/
void UpdatePlayer()
{
	player->Update();
}

/**************************************
描画
***************************************/
void DrawPlayer()
{
	player->Draw();
}

/**************************************
サイズ取得
***************************************/
float GetPlayerSize()
{
	return player->GetSize();
}

/**************************************
サイズ取得
***************************************/
D3DXVECTOR3 GetPlayerPosition()
{
	return player->GetPosition();
}

/**************************************
コンストラクタ
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
デストラクタ
***************************************/
Player::~Player()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(cream);
	collider.reset();
}

/**************************************
更新処理
***************************************/
void Player::Update()
{
	transform->Rotate(5.0f, Vector3::Right);

	//クリームがヒットしていたら大きくなる
	//していなければ小さく
	//フラグを毎フレーム下ろす
	float deltaSize = isHitCream ? 0.05f : -0.02f;
	scaleCream = Math::Clamp(0.95f, 25.0f, scaleCream + deltaSize);
	cream->SetScale({ 1.0f, scaleCream, scaleCream });
	isHitCream = false;

	//移動
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
描画処理
***************************************/
void Player::Draw()
{
	transform->SetWorld();
	mesh->Draw();

	cream->Draw(*transform);

	collider->Draw();
}

/**************************************
当たり判定
***************************************/
void Player::OnColliderHit(ColliderObserver *other)
{
	Debug::Log("Hit Player");
	isHitCream = true;
}

/**************************************
コンストラクタ
***************************************/
PlayerCream::PlayerCream()
{
	mesh = new MeshContainer();
	ResourceManager::Instance()->GetMesh("PlayerCream", mesh);
}

/**************************************
デストラクタ
***************************************/
PlayerCream::~PlayerCream()
{
	SAFE_DELETE(mesh);
}

/**************************************
描画処理
***************************************/
void PlayerCream::Draw(const Transform& parent)
{
	D3DXMATRIX mtxParent = parent.GetMatrix();
	transform->SetWorld(&mtxParent);
	mesh->Draw();
}
