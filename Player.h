//=====================================
//
//Player.h
//�@�\:
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "Framework\Collider\ColliderObserver.h"

/**************************************
�v���g�^�C�v�錾
***************************************/
void InitPlayer();
void UninitPlayer();
void UpdatePlayer();
void DrawPlayer();

/**************************************
�O���錾
***************************************/
class MeshContainer;
class PlayerCream;
class BoxCollider3D;

/**************************************
�N���X��`
***************************************/
class Player : public GameObject, public ColliderObserver
{
public:
	Player();
	~Player();

	void Update();
	void Draw();

	void OnColliderHit(ColliderObserver *other) override;

private:
	MeshContainer * mesh;

	PlayerCream* cream;
	float scaleCream;

	std::shared_ptr<BoxCollider3D> collider;

	bool isHitCream;
};

class PlayerCream : public GameObject
{
public:
	PlayerCream();
	~PlayerCream();

	void Draw(const Transform& parent);

private:
	MeshContainer * mesh;
};

#endif