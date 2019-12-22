//=====================================
//
//トゥイーンヘッダ[Transformween.h]
//Author:GP12B332 21 立花雄太
//
//=====================================
#ifndef _TWEEN_H_
#define _TWEEN_H_

#include "../../main.h"
#include "../Math/Easing.h"
#include "../Core/Transform.h"
#include "Tweener/ValueTweener.h"

#include <list>
#include <memory>
#include <iterator>
#include <functional>

/**************************************
前方宣言
***************************************/
class BaseTweener;
class Polygon2D;

/**************************************
マクロ・列挙子定義
***************************************/
enum class ExpandType : int
{
	None,
	LeftToRight,
	RightToLeft,
	UpToDown,
	DownToUp,
	ToUpDown,
	ToLeftRight
};

enum class CloseType : int
{
	None,
	LeftToRight,
	RightToLeft,
	UpToDown,
	DownToUp,
	FromUpDown,
	FromLeftRight
};

/**************************************
クラス定義
***************************************/

class Tween
{
	friend class BaseGame;
	friend class SceneManager;
public:
	/**************************************
	移動トゥイーン
	引数 ref：トゥイーン対象のゲームオブジェクト
	引数 start : 移動開始座標
	引数 end : 移動先座標
	引数 duration : 移動にかける時間
	引数 type : イージングタイプ
	引数 callback : 終了時のコールバック関数
	***************************************/
	static void Move(GameObject& ref, const D3DXVECTOR3& startPosition, const D3DXVECTOR3& endPosition, int duration, EaseType type, std::function<void(void)> callback = nullptr);

	/**************************************
	移動トゥイーン
	基本的には上記と一緒だが、移動開始座標を現在座標に自動で設定してくれる
	***************************************/
	static void Move(GameObject& ref, const D3DXVECTOR3& endPosition, int duratino, EaseType type, std::function<void(void)> callback = nullptr);

	/**************************************
	スケールトゥイーン
	引数 ref：トゥイーン対象のゲームオブジェクト
	引数 start : 開始時のスケール
	引数 end : 終了時のスケール
	引数 duration : スケールにかける時間
	引数 type : イージングタイプ
	引数 callback : 終了時のコールバック関数
	***************************************/
	static void Scale(GameObject& ref, const D3DXVECTOR3& startScale, const D3DXVECTOR3& endScale, int duration, EaseType type, std::function<void(void)> callback = nullptr);

	/**************************************
	スケールトゥイーン
	基本的には上記と一緒だが開始時のスケールを現在スケールに自動で設定してくれる
	***************************************/
	static void Scale(GameObject& ref, const D3DXVECTOR3& endScale, int duration, EaseType type, std::function<void(void)> callback = nullptr);

	/**************************************
	回転トゥイーン
	引数 ref：トゥイーン対象のゲームオブジェクト
	引数 start : 回転開始角度（オイラー角）
	引数 end : 回転終了角度（オイラー角）
	引数 duration : 回転にかける時間
	引数 type : イージングタイプ
	引数 callback : 終了時のコールバック関数
	※回転角度は自動で0.0f~360.0fに補正されてしまうので注意
	***************************************/
	static void Rotate(GameObject& ref, const D3DXVECTOR3& startEulerAngle, const D3DXVECTOR3& endEulerAngle, int duration, EaseType type, std::function<void(void)> callback = nullptr);

	/**************************************
	回転トゥイーン
	基本的には上記と一緒だが開始の回転角度を自動で現在角度に設定してくれる
	***************************************/
	static void Rotate(GameObject& ref, const D3DXVECTOR3& endEulaerAngle, int duration, EaseType type, std::function<void(void)> callback = nullptr);

	/**************************************
	方向トゥイーン
	引数 ref：トゥイーン対象のゲームオブジェクト
	引数 endDirection : 回転終了時の前方向
	引数 duration : 回転にかける時間
	引数 type : イージングタイプ
	引数 dummyAxis : 始点の向きと終点の向きが平行だった場合に使用する回転軸
	引数 callback : 終了時のコールバック関数
	***************************************/
	static void Turn(GameObject& ref, const D3DXVECTOR3& endDirection, int duration, EaseType type, const D3DXVECTOR3& dummyAxis, std::function<void()> callback = nullptr);

	/**************************************
	値トゥイーン
	引数 ref：トゥイーン対象の変数へのスマートポインタ
	引数 start : 開始時の値
	引数 end : 終了時の値
	引数 duration : 変化にかける時間
	引数 type : イージングタイプ
	引数 callback : 終了時のコールバック関数
	***************************************/
	template<class T>
	static void To(std::shared_ptr<T>& ref, const T& start, const T& end, int duration, EaseType type, std::function<void()> callback = nullptr)
	{
		ValueTweener<T> *tweener = new ValueTweener<T>(ref, start, end, duration, type, callback);
		mInstance->tweenerContainer.push_back(tweener);
	}

	/**************************************
	値トゥイーン
	引数 ref：トゥイーン対象の変数へのスマートポインタ
	引数 end : 終了時の値
	引数 duration : 変化にかける時間
	引数 type : イージングタイプ
	引数 callback : 終了時のコールバック関数
	***************************************/
	template<class T>
	static void To(std::shared_ptr<T>& ref, const T& end, int duration, EaseType type, std::function<void()> callback = nullptr)
	{
		T start = *ref;
		ValueTweener<T> *tweener = new ValueTweener<T>(ref, start, end, duration, type, callback);
		mInstance->tweenerContainer.push_back(tweener);
	}

	static void Expand(std::shared_ptr<Polygon2D>& ref, ExpandType expand, int duration, EaseType type, std::function<void()> callback = nullptr);

	static void Close(std::shared_ptr<Polygon2D>& ref, CloseType close, int duration, EaseType type, std::function<void()> callback = nullptr);

	static void Fade(std::shared_ptr<Polygon2D>& ref, float start, float end, int duration, EaseType type, std::function<void()> callback = nullptr);

private:
	void Update();
	void ClearContainer();
	void ClearAll();

	std::list<BaseTweener*> tweenerContainer;

	static Tween* mInstance;
	Tween();
	~Tween();
};

#endif