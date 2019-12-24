//=====================================
//
//コライダーマネージャヘッダ[ColliderManager.h]
//Author:GP12B332 21 立花雄太
//
//=====================================
#ifndef _COLLIDERMANAGER_H_
#define _COLLIDERMANAGER_H_

#include "BoxCollider3D.h"
#include "../Pattern/BaseSingleton.h"

#include <unordered_map>
#include <list>
#include <string>
#include <memory>
#include <string>

/**************************************
前方宣言
***************************************/

/**************************************
クラス定義
***************************************/
class ColliderManager : public BaseSingleton<ColliderManager>
{
public:
	//更新処理
	void Update();

	//クリア処理
	void Clear();

	//衝突判定
	void CheckRoundRobin(const std::string& tag1, const std::string& tag2);

	//当たり判定追加処理
	void AddBoxCollider3D(const std::string& tag, std::shared_ptr<BoxCollider3D>& entity);

private:
	//ボックスコライダーコンテナ
	std::unordered_map <std::string, std::list<std::weak_ptr<BoxCollider3D>>> boxColliderMap;
	
};

#endif