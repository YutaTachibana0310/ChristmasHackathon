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

	transform->SetPosition({ 0.0f, 0.0f, -20.0f });
}

/**************************************
デストラクタ
***************************************/
Player::~Player()
{
	SAFE_DELETE(mesh);
}

/**************************************
更新処理
***************************************/
void Player::Update()
{
	transform->Rotate(5.0f, Vector3::Right);
}

/**************************************
描画処理
***************************************/
void Player::Draw()
{
	transform->SetWorld();
	mesh->Draw();
}
