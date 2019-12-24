//=====================================
//
//ボックスコライダー3Dヘッダ[BoxCollider3D.h]
//Author:GP12B332 21 立花雄太
//
//=====================================
#ifndef _BOXCOLLIDER3D_H_
#define _BOXCOLLIDER3D_H_

#define BOXCOLLIDER3D_USE_DEBUG

#include "../../main.h"
#include "BaseCollider.h"
#include "ColliderObserver.h"

#include <memory>
/**************************************
BoxCollider3Dクラス
***************************************/
class BoxCollider3D : public BaseCollider, public std::enable_shared_from_this<BoxCollider3D>
{
public:
	//インスタンス作成処理
	static std::shared_ptr<BoxCollider3D> Create(std::string tag, const std::shared_ptr<Transform>& transform);

	//デストラクタ
	~BoxCollider3D();

	//衝突判定、サイズセット、座標オフセットセット
	bool CheckCollision(BoxCollider3D& other);
	void SetSize(const D3DXVECTOR3 size);
	void SetOffset(const D3DXVECTOR3 offset);

	//ID取得
	unsigned GetUniqueID() const;

	//デバッグ表示
	void Draw();

private:
	std::string tag;		//タグ
	D3DXVECTOR3 size;		//サイズ
	D3DXVECTOR3 offset;		//オフセット
	
	unsigned uniqueID;				//ユニークID
	static unsigned incrementID;	//インクリメントID

	BoxCollider3D(const std::string& tag, const std::shared_ptr<Transform>& transform);

#ifdef BOXCOLLIDER3D_USE_DEBUG
	static UINT instanceCount;		//インスタンスカウント
	static void CreateRenderTool();
	static D3DMATERIAL9 material;	//描画用マテリアル
	static LPD3DXMESH mesh;			//描画用メッシュ
#endif
};

using PtrBoxCollider3D = std::shared_ptr<BoxCollider3D>;

#endif