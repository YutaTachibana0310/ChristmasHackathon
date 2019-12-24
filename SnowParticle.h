//=====================================
//
//SnowParticle.h
//�@�\:
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#ifndef _SNOWPARTICLE_H_
#define _SNOWPARTICLE_H_

#include "main.h"
#include "Framework\Particle\3D\Particle3D.h"
#include "Framework\Particle\BaseParticleController.h"

/**************************************
�O���錾
***************************************/

/**************************************
�N���X��`
***************************************/
class SnowParticleController : public BaseParticleController
{
public:
	SnowParticleController();
};

class SnowParticle : public Particle3D
{
public:
	SnowParticle();

	void Init();
	void Update();

private:
	float speed;
};
#endif