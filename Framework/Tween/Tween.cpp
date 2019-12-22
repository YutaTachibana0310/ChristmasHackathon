//=====================================
//
//トゥイーン処理[Tween.cpp]
//Author:GP12A332 21 立花雄太
//
//=====================================
#include "Tween.h"

#include "Tweener/MoveTweener.h"
#include "Tweener/RotateTweener.h"
#include "Tweener/ScaleTweener.h"
#include "Tweener/ViewerTweener.h"

#include <algorithm>

using namespace std;

/**************************************
展開タイプ変換
***************************************/
ViewerTweener::ExpandType ConvertExpandType(ExpandType type)
{
	if (type == ExpandType::DownToUp)
		return ViewerTweener::ExpandType::DownToUp;

	if (type == ExpandType::LeftToRight)
		return ViewerTweener::ExpandType::LeftToRight;

	if (type == ExpandType::RightToLeft)
		return ViewerTweener::ExpandType::RightToLeft;

	if (type == ExpandType::ToLeftRight)
		return ViewerTweener::ExpandType::ToLeftRight;

	if (type == ExpandType::ToUpDown)
		return ViewerTweener::ExpandType::ToUpDown;

	if (type == ExpandType::UpToDown)
		return ViewerTweener::ExpandType::UpToDown;

	return ViewerTweener::ExpandType::None;
}

/**************************************
圧縮タイプ変換
***************************************/
ViewerTweener::CloseType ConvertCloseType(CloseType type)
{
	if (type == CloseType::DownToUp)
		return ViewerTweener::CloseType::DownToUp;

	if (type == CloseType::LeftToRight)
		return ViewerTweener::CloseType::LeftToRight;

	if (type == CloseType::RightToLeft)
		return ViewerTweener::CloseType::RightToLeft;

	if (type == CloseType::FromLeftRight)
		return ViewerTweener::CloseType::FromLeftRight;

	if (type == CloseType::FromUpDown)
		return ViewerTweener::CloseType::FromUpDown;

	if (type == CloseType::UpToDown)
		return ViewerTweener::CloseType::UpToDown;

	return ViewerTweener::CloseType::None;
}

/**************************************
コンストラクタ
***************************************/
Tween::Tween()
{
	if (mInstance == NULL)
	{
		mInstance = this;
	}
}

/**************************************
デストラクタ
***************************************/
Tween::~Tween()
{
	for (auto&& tweener : tweenerContainer)
	{
		SAFE_DELETE(tweener);
	}
	tweenerContainer.clear();

	if (mInstance == this)
	{
		mInstance = NULL;
	}
}

/**************************************
更新処理
***************************************/
void Tween::Update()
{
	for (auto&& tweener : tweenerContainer)
	{
		tweener->Update();
	}

	ClearContainer();
}

/**************************************
コンテナクリア処理
***************************************/
void Tween::ClearContainer()
{
	for (auto&& tweener : tweenerContainer)
	{
		if (!tweener->IsFinished())
			continue;

		SAFE_DELETE(tweener);
	}

	tweenerContainer.remove_if([](auto&& tweener)
	{
		return tweener == NULL;
	});
}

/**************************************
クリア処理
***************************************/
void Tween::ClearAll()
{
	for (auto&& tweener : tweenerContainer)
	{
		SAFE_DELETE(tweener);
	}

	tweenerContainer.clear();
}

/**************************************
移動処理
***************************************/
void Tween::Move(GameObject& ref, const D3DXVECTOR3& start, const D3DXVECTOR3& end, int duration, EaseType type, std::function<void(void)> callback)
{
	MoveTweener *tweener = new MoveTweener(ref.transform, start, end, duration, type, callback);
	mInstance->tweenerContainer.push_back(tweener);
}

/**************************************
移動処理
***************************************/
void Tween::Move(GameObject& ref, const D3DXVECTOR3& end, int duration, EaseType type, std::function<void(void)> callback)
{
	D3DXVECTOR3 start = ref.transform->GetPosition();
	Move(ref, start, end, duration, type, callback);
}

/**************************************
スケール処理
***************************************/
void Tween::Scale(GameObject& ref, const D3DXVECTOR3& start, const D3DXVECTOR3& end, int duration, EaseType type, std::function<void(void)> callback)
{
	ScaleTweener *tweener = new ScaleTweener(ref.transform, start, end, duration, type, callback);
	mInstance->tweenerContainer.push_back(tweener);
}

/**************************************
スケール処理
***************************************/
void Tween::Scale(GameObject& ref, const D3DXVECTOR3& end, int duration, EaseType type, std::function<void(void)> callback)
{
	D3DXVECTOR3 start = ref.transform->GetScale();
	Scale(ref, start, end, duration, type, callback);
}

/**************************************
回転処理
***************************************/
void Tween::Rotate(GameObject& ref, const D3DXVECTOR3& start, const D3DXVECTOR3& end, int duration, EaseType type, std::function<void(void)> callback)
{
	RotateTweener *tweener = new RotateTweener(ref.transform, start, end, duration, type, callback);
	mInstance->tweenerContainer.push_back(tweener);
}

/**************************************
回転処理
***************************************/
void Tween::Rotate(GameObject& ref, const D3DXVECTOR3& end, int duration, EaseType type, std::function<void(void)> callback)
{
	D3DXVECTOR3 start = ref.transform->GetEulerAngle();
	Rotate(ref, start, end, duration, type, callback);
}

/**************************************
方向処理
***************************************/
void Tween::Turn(GameObject & ref, const D3DXVECTOR3 & end, int duration, EaseType type, const D3DXVECTOR3& dummyAxis, std::function<void()> callback)
{
	//始点となるクォータニオンを求める
	D3DXQUATERNION start = ref.transform->GetRotation();

	//終点となるクォータニオンを求める
	D3DXVECTOR3 axis = Vector3::Axis(ref.transform->Forward(), end);
	float angle = Vector3::Angle(ref.transform->Forward(), end);

	//axisがゼロベクトル（始点の向きと終点の向きが並行）の場合はダミーアクシスを使用
	if (Vector3::Equal(axis, Vector3::Zero))
	{
		axis = dummyAxis;
	}

	D3DXQUATERNION rotQ;
	D3DXQuaternionRotationAxis(&rotQ, &axis, D3DXToRadian(angle));

	D3DXQUATERNION endQuaternion = start * rotQ;

	//回転Tweener作成
	RotateTweener *tweener = new RotateTweener(ref.transform, start, endQuaternion, duration, type, callback);
	mInstance->tweenerContainer.push_back(tweener);
}

/**************************************
ビューア展開
***************************************/
void Tween::Expand(std::shared_ptr<Polygon2D>& ref, ExpandType expand, int duration, EaseType type, std::function<void()> callback)
{
	ViewerTweener* tweener = new ViewerTweener(ref, ConvertExpandType(expand), duration, type, callback);
	mInstance->tweenerContainer.push_back(tweener);
}

/**************************************
ビューア圧縮
***************************************/
void Tween::Close(std::shared_ptr<Polygon2D>& ref, CloseType close, int duration, EaseType type, std::function<void()> callback)
{
	ViewerTweener* tweener = new ViewerTweener(ref, ConvertCloseType(close), duration, type, callback);
	mInstance->tweenerContainer.push_back(tweener);
}

/**************************************
ビューアフェード
***************************************/
void Tween::Fade(std::shared_ptr<Polygon2D>& ref, float start, float end, int duration, EaseType type, std::function<void()> callback)
{
	ViewerTweener* tweener = new ViewerTweener(ref, start, end, duration, type, callback);
	mInstance->tweenerContainer.push_back(tweener);
}