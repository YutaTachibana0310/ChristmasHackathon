//=====================================
//
//Player.h
//機能:
//Author:GP12B332 21 立花雄太
//
//=====================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "Framework\Collider\ColliderObserver.h"

/**************************************
プロトタイプ宣言
***************************************/
void InitPlayer();
void UninitPlayer();
void UpdatePlayer();
void DrawPlayer();

/**************************************
前方宣言
***************************************/
class MeshContainer;
class PlayerCream;
class BoxCollider3D;

/**************************************
クラス定義
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