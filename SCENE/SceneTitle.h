#pragma once
#include "SceneBase.h"
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <DxLib.h>

class Model;
class Camera;
class LightBulb;
class UIItemManager;
class ObjectManager;

class SceneTitle : public SceneBase
{
private:
	struct CameraInfo {
		VECTOR targetPos;	//目的の場所
		VECTOR targetView;	//見る場所
		VECTOR upVec;		//カメラの上方向
	};
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	SceneTitle(SceneManager& manager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneTitle();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 終了
	/// </summary>
	void End();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	/// <summary>
	/// フェードイン
	/// </summary>
	void FadeInUpdate();

	/// <summary>
	/// 通常時の更新
	/// </summary>
	void UIUpdate();

	/// <summary>
	/// オープニングサウンドの更新
	/// </summary>
	void OpeningSoundUpdate();

	/// <summary>
	/// オープニングの更新
	/// </summary>
	void OpeningUpdate();

	/// <summary>
	/// UIのフェードアウト
	/// </summary>
	void UIFadeOutUpdate();

	/// <summary>
	/// オープニングのフェードアウト
	/// </summary>
	void SceneTitleFadeOutUpdate();

	/// <summary>
	/// シーンを切り替える
	/// </summary>
	void SceneChange();

private:

	int handle[99] = {};

	static constexpr int fadeInterval_ = 30;	//インターバル
	int fadeTimer_ = 0;							//フェードする時間
	int fadeValue_ = 0;							//フェードの透過値
	int UIfadeValue_ = 0;						//UIのフェード透過値
	int fadeColor_ = 0x000000;					//黒

	int fontHandle_ = -1;
	int titleHandle_ = -1;						//タイトル画像の保管変数
	int selectNum_ = 1;							//選択

	float outAngle_ = 0.0f;						//スポットライトの外側の角度
	float inAngle_ = 0.0f;						//スポットライトの内側の角度

	std::shared_ptr<Camera> camera_;			//カメラのスパートポインタ
	std::shared_ptr<UIItemManager> UI_;			//UIマネージャーのスマートポインタ
	std::shared_ptr<LightBulb> lightBulb_;		//電球
	std::shared_ptr<Model> subPlayerModel_;		//プレイヤーのスマートポインタ
	std::shared_ptr<Model> mainPlayerModel_;	//プレイヤーのスマートポインタ
	std::shared_ptr<ObjectManager> objManager_;	//オブジェクトマネージャーのスマートポインタ

	std::vector<std::string> menuName_;			//メニューの文字列
	std::vector<int> lightHandle_;				//ライトハンドル
	std::vector<VECTOR> lightDir_;				//ライトのディレクション(方向)
	std::vector<CameraInfo> cameraInfo_;		//カメラの情報

	std::map<std::string, VECTOR> menuDrawPos_;	//メニューを描画する座標

	void (SceneTitle::* updateFunc_)();
};

