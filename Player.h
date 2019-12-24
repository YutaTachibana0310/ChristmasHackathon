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

/**************************************
�N���X��`
***************************************/
class Player : public GameObject
{
public:
	Player();
	~Player();

	void Update();
	void Draw();

private:
	MeshContainer * mesh;
};

#endif