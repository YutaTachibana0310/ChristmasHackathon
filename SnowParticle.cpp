//=====================================
//
//SnowParticle.cpp
//�@�\:
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "SnowParticle.h"
#include "Framework\Math\Easing.h"

/**************************************
Controller�R���X�g���N�^
***************************************/
SnowParticleController::SnowParticleController() :
	BaseParticleController(Particle_3D)
{
	MakeUnitBuffer({ 10.0f, 10.0f });

	LoadTexture("data/TEXTURE/Particle/snow.png");

	particleContainer.resize(2048, nullptr);
	for (auto&& particle : particleContainer)
	{
		particle = new SnowParticle();
	}
	
	emitterContainer.resize(2, nullptr);
	for (auto&& emitter : emitterContainer)
	{
		emitter = new BaseEmitter(5, 0);
	}
}


/**************************************
�R���X�g���N�^
***************************************/
SnowParticle::SnowParticle() :
	Particle3D(120, 180)
{
}

/**************************************
����������
***************************************/
void SnowParticle::Init()
{
	cntFrame = 0;
	active = true;

	D3DXVECTOR3 pos = Vector3::Zero;
	pos.x = Math::RandomRange(-1000.0f, 1000.0f);
	pos.y = Math::RandomRange(5.0f, 100.0f);
	pos.z = Math::RandomRange(-50.0f, 1000.0f);
	transform->SetPosition(pos);

	speed = Math::RandomRange(0.2f, 0.1f);
}

/**************************************
�X�V����
***************************************/
void SnowParticle::Update()
{
	if (!IsActive())
		return;

	cntFrame++;

	transform->Move(Vector3::Down * speed);

	float t = (float)cntFrame / lifeFrame;
	float scale = Easing::EaseValue(t, 1.0f, 0.0f, EaseType::InExpo);
	transform->SetScale(Vector3::One * scale);
}
