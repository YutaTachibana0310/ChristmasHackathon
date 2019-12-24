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

#include "Framework\Input\input.h"

/**************************************
グローバル変数
***************************************/
static Player* player = nullptr;

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
デストラクタ
***************************************/
Player::~Player()
{
	SAFE_DELETE(mesh);
	SAFE_DELETE(cream);
}

/**************************************
更新処理
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
描画処理
***************************************/
void Player::Draw()
{
	transform->SetWorld();
	mesh->Draw();

	cream->Draw();
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
void PlayerCream::Draw()
{
	transform->SetWorld();
	mesh->Draw();
}

/**************************************
回転処理
***************************************/
void PlayerCream::Rotate(float degree)
{
	transform->Rotate(degree, Vector3::Right);
}
