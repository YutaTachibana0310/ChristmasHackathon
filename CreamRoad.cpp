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
#include "Framework\Collider\BoxCollider3D.h"

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
	cntGenerateCream++;
	if (cntGenerateCream == 60)
	{
		CreamRoad *ptr = ObjectPool::Instance()->Create<CreamRoad>();
		ptr->Init();

		container.push_back(ptr);
		cntGenerateCream = 0;
	}

	for (auto&& cream : container)
	{
		cream->Update();
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
�`�揈��
***************************************/
void DrawCreamRoad()
{
	for (auto&& cream : container)
	{
		cream->Draw();
	}
}

/**************************************
�R���X�g���N�^
***************************************/
CreamRoad::CreamRoad()
{
	mesh = new MeshContainer();
	ResourceManager::Instance()->GetMesh("CreamRoad", mesh);

	collider = BoxCollider3D::Create("CreamRoad", transform);
	collider->SetSize({ 7.5f, 10.0f, 80.0f });

	transform->Move(Vector3::Up * Math::RandomRange(0.01f, 0.05f));
	transform->SetScale({ 1.0f, 1.0f, 5.0f });
}

/**************************************
�f�X�g���N�^
***************************************/
CreamRoad::~CreamRoad()
{
	SAFE_DELETE(mesh);
	collider.reset();
}

/**************************************
����������
***************************************/
void CreamRoad::Init()
{
	D3DXVECTOR3 position = transform->GetPosition();
	position.z = 2500.0f;
	position.x = Math::RandomRange(-1, 2) * 15.0f;
	transform->SetPosition(position);
	SetActive(true);
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
	D3DXVECTOR3 position = transform->GetPosition();

	if (position.z < -500.0f)
	{
		SetActive(false);
	}
	else
	{
		position.z -= 15.0f;
		transform->SetPosition(position);
	}
}

/**************************************
�`�揈��
***************************************/
void CreamRoad::Draw()
{
	transform->SetWorld();
	mesh->Draw();

	collider->Draw();
}
