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
コンストラクタ
***************************************/
Player::Player() :
	isHitCream(false)
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
	float deltaSize = isHitCream ? 0.05f : -0.05f;
	scaleCream = Math::Clamp(0.0f, 100.0f, scaleCream + deltaSize);
	cream->SetScale({ 1.0f, scaleCream, scaleCream });
	isHitCream = false;

	//移動

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

	Debug::Begin("Player");
	static D3DXVECTOR3 pos = InitPos;
	Debug::Slider("Pos", pos, Vector3::One * -50.0f, Vector3::One * 50.0f);
	transform->SetPosition(pos);
	Debug::End();
#endif
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
