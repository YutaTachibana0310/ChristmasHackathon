//=====================================
//
// GameParticleManager.h
// 機能:ゲームシーンのパーティクルマネージャ
// Author:GP12B332 21 立花雄太
//
//=====================================
#ifndef _GAMEPARTICLEMANAGER_H_
#define _GAMEPARTICLEMANAGER_H_

#include "main.h"
#include "Framework/Particle/SceneParticleManager.h"
#include "Framework/Pattern/BaseSingleton.h"

/**************************************
ゲームシーンのパーティクル通し番号
***************************************/
namespace GameParticle
{
	enum ID
	{
		Snow,		//白い煙
		Max
	};
}

/**************************************
クラス定義
***************************************/
class GameParticleManager : public SceneParticleManager, public BaseSingleton<GameParticleManager>
{
	using SceneParticleManager::SceneParticleManager;
public:
	//初期化処理
	void Init() override;

	//描画処理
	void Draw() override;

private:
	static const float BloomPower[3];
	static const float BloomThrethold[3];
};

#endif