//=====================================
//
//CreamRoad.cpp
//�@�\:
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "CreamRoad.h"
#include "Framework\Resource\ResourceManager.h"
#include "Framework\Renderer3D\MeshContainer.h"
#include "Framework\Core\ObjectPool.h"

/**************************************
�O���[�o���ϐ�
***************************************/
std::vector<CreamRoad*> container;
static int cntGenerateCream;

/**************************************
����������
***************************************/
void InitCreamRoad()
{
	ResourceManager::Instance()->LoadMesh("CreamRoad", "data/MODEL/CreamRoad.x");

	cntGenerateCream = 0;
}

/**************************************
�I������
***************************************/
void UninitCreamRoad()
{
	Utility::DeleteContainer(container);
}

/**************************************
�X�V����
***************************************/
void UpdateCreamRoad()
{
	for (auto&& cream : container)
	{
		cream->Update();
	}
}

/**************************************
�`�揈��
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
�R���X�g���N�^
***************************************/
CreamRoad::CreamRoad()
{
	mesh = new MeshContainer();
	ResourceManager::Instance()->GetMesh("CreamRoad", mesh);

	transform->Move(Vector3::Up * Math::RandomRange(0.01f, 0.05f));
}

/**************************************
�f�X�g���N�^
***************************************/
CreamRoad::~CreamRoad()
{
	SAFE_DELETE(mesh);
}

/**************************************
����������
***************************************/
void CreamRoad::Init()
{
}

/**************************************
�I������
***************************************/
void CreamRoad::Uninit()
{
}

/**************************************
�X�V����
***************************************/
void CreamRoad::Update()
{
	transform->Move(Vector3::Back * 0.5f);
}

/**************************************
�`�揈��
***************************************/
void CreamRoad::Draw()
{
	transform->SetWorld();
	mesh->Draw();
}
