#pragma once
#include "SceneBase.h"
#include <DxLib.h>
#include <string>
#include <vector>
#include <memory>
#include <map>

class UIItemManager;
class ObjectManager;
class Camera;
class Model;

class SceneTitle : public SceneBase
{
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

	/// <summary>
	/// カメラの設定
	/// </summary>
	void CameraSetting();

private:

	static constexpr int fadeInterval_ = 30;	//インターバル
	int fadeTimer_ = 0;							//フェードする時間
	int fadeValue_ = 0;							//フェードの透過値
	int UIfadeValue_ = 0;						//UIのフェード透過値
	int fadeColor_ = 0x000000;					//黒

	int titleHandle_ = -1;						//タイトル画像の保管変数
	int selectNum_ = 1;							//選択

	std::vector<std::string> menuName_;			//メニューの文字列
	std::map<std::string, VECTOR> menuDrawPos_;	//メニューを描画する座標
	std::shared_ptr<Camera> camera_;			//カメラのスパートポインタ
	std::shared_ptr<UIItemManager> UI_;			//UIマネージャーのスマートポインタ
	std::shared_ptr<ObjectManager> objManager_;	//オブジェクトマネージャーのスマートポインタ
	std::shared_ptr<Model> playerModel_;		//プレイヤーのスマートポインタ

	void (SceneTitle::* updateFunc_)();
};

