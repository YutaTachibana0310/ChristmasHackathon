//=====================================
//
//CreamRoad.cpp
//機能:
//Author:GP12B332 21 立花雄太
//
//=====================================
#include "CreamRoad.h"
#include "Framework\Resource\ResourceManager.h"
#include "Framework\Renderer3D\MeshContainer.h"
#include "Framework\Core\ObjectPool.h"

/**************************************
グローバル変数
***************************************/
std::vector<CreamRoad*> container;
static int cntGenerateCream;

/**************************************
初期化処理
***************************************/
void InitCreamRoad()
{
	ResourceManager::Instance()->LoadMesh("CreamRoad", "data/MODEL/CreamRoad.x");

	cntGenerateCream = 0;
}

/**************************************
終了処理
***************************************/
void UninitCreamRoad()
{
	Utility::DeleteContainer(container);
}

/**************************************
更新処理
***************************************/
void UpdateCreamRoad()
{
	for (auto&& cream : container)
	{
		cream->Update();
	}
}

/**************************************
描画処理
***************************************/
void DrawCreamRoad()
{
	cntGenerateCream++;

	if (cntGenerateCream == 120)
	{
		CreamRoad *ptr = ObjectPool::Instance()->Create<CreamRoad>();
		container.push_back(ptr);
		cntGenerateCream = 0;
	}

	for (auto&& cream : container)
	{
		cream->Draw();
	}

	for (auto&& cream : container)
	{
		if (cream->IsActive())
			continue;

		ObjectPool::Instance()->Destroy(cream);
		cream = nullptr;
	}

	auto itr = std::remove(container.begin(), container.end(), nullptr);
	container.erase(itr, container.end());
}

/**************************************
コンストラクタ
***************************************/
CreamRoad::CreamRoad()
{
	mesh = new MeshContainer();
	ResourceManager::Instance()->GetMesh("CreamRoad", mesh);

	transform->Move(Vector3::Up * Math::RandomRange(0.01f, 0.05f));
}

/**************************************
デストラクタ
***************************************/
CreamRoad::~CreamRoad()
{
	SAFE_DELETE(mesh);
}

/**************************************
初期化処理
***************************************/
void CreamRoad::Init()
{
}

/**************************************
終了処理
***************************************/
void CreamRoad::Uninit()
{
}

/**************************************
更新処理
***************************************/
void CreamRoad::Update()
{
	transform->Move(Vector3::Back * 0.5f);
}

/**************************************
描画処理
***************************************/
void CreamRoad::Draw()
{
	transform->SetWorld();
	mesh->Draw();
}
