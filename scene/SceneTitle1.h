#pragma once
#include "SceneBase.h"
#include<string>
#include <vector>
#include <memory>

class UIItemManager;
class ObjectManager;
class Camera;
class Model;

class SceneTitle1 : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	SceneTitle1(SceneManager& manager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneTitle1();

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
	void NormalUpdate();

	/// <summary>
	/// オープニングサウンドの更新
	/// </summary>
	void OpeningSoundUpdate();

	/// <summary>
	/// オープニングフェードインの描画
	/// </summary>
	void OpeningFadeInUpdate();

	/// <summary>
	/// フェードアウト
	/// </summary>
	void FadeOutUpdate();

	/// <summary>
	/// 通常時の描画
	/// </summary>
	void NormalDraw();

	/// <summary>
	/// オープニングの描画
	/// </summary>
	void OpeningDraw();

	/// <summary>
	/// シーンを切り替える
	/// </summary>
	void SceneChange();

private:
	int fontHandle_ = -1;
	int titleWidth = 0;

	static constexpr int fadeInterval_ = 30;
	int fadeTimer_ = 0;
	int fadeValue_ = 0;
	int fadeColor_ = 0x000000;

	int titleHandle_ = -1;						//タイトル画像の保管変数
	int selectNum_ = 0;							//選択

	std::vector<std::string> menuName_;			//メニューの文字列
	std::shared_ptr<Camera> camera_;			//カメラのスパートポインタ
	std::shared_ptr<UIItemManager> UI_;			//UIマネージャーのスマートポインタ
	std::shared_ptr<ObjectManager> objManager_;	//オブジェクトマネージャーのスマートポインタ
	std::shared_ptr<Model> model_;				//プレイヤーのスマートポインタ

	void (SceneTitle1::* updateFunc_)();
	void (SceneTitle1::* drawFunc_)();
};

