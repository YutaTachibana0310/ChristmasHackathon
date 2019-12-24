//=====================================
//
//ゲームパーティクルマネージャ処理[GameParticleManager.cpp]
//Author:GP12A332 21 立花雄太
//
//=====================================
#include "GameParticleManager.h"
#include "Framework/PostEffect/CrossFilterController.h"
#include "Framework/Tool/DebugWindow.h"
#include "Framework/Particle/BaseParticleController.h"

#include "SnowParticle.h"

/**************************************
staticメンバ
***************************************/
const float GameParticleManager::BloomPower[3] = { 1.0f, 1.0f, 1.0f };
const float GameParticleManager::BloomThrethold[3] = { 0.46f, 0.35f, 0.18f };

/**************************************
初期化処理
***************************************/
void GameParticleManager::Init()
{
	SceneParticleManager::Init();

	controllers.resize(GameParticle::Max, NULL);
	controllers[GameParticle::Snow] = new SnowParticleController();

	Generate(GameParticle::Snow, Vector3::Zero);

	crossFilter->SetPower(BloomPower[0], BloomPower[1], BloomPower[2]);
	crossFilter->SetThrethold(BloomThrethold[0], BloomThrethold[1], BloomThrethold[2]);
}

/**************************************
描画処理
***************************************/
void GameParticleManager::Draw()
{
	SceneParticleManager::Draw();

	//Debug::Begin("GameParticle");
	//Debug::Text("Cloud : %d", controllers[GameParticle::Cloud]->GetParticleCount());
	//Debug::End();
}
