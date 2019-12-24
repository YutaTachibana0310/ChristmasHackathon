//=====================================
//
//SnowParticle.h
//機能:
//Author:GP12B332 21 立花雄太
//
//=====================================
#ifndef _SNOWPARTICLE_H_
#define _SNOWPARTICLE_H_

#include "main.h"
#include "Framework\Particle\3D\Particle3D.h"
#include "Framework\Particle\BaseParticleController.h"

/**************************************
前方宣言
***************************************/

/**************************************
クラス定義
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