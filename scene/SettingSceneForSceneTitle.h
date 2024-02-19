#pragma once
#include "SceneBase.h"
#include <map>
#include <memory>
#include <string>
#include <DxLib.h>

class InputState;
class UIItemManager;

class SettingSceneForSceneTitle : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	SettingSceneForSceneTitle(SceneManager& manager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SettingSceneForSceneTitle();

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

	/// <summary>
	/// 通常時の更新
	/// </summary>
	void NormalUpdate();

	/// <summary>
	/// ガウス処理を施したフェードイン
	/// </summary>
	void GaussFadeInUpdate();

	/// <summary>
	/// ガウス処理を施したフェードアウト
	/// </summary>
	void GaussFadeOutUpdate();

	/// <summary>
	/// BGMの音量を変更する
	/// </summary>
	void BGMUpdate();

	/// <summary>
	/// SEの音量を変更する
	/// </summary>
	void SEUpdate();

	/// <summary>
	/// ウィンドウモードを変更する
	/// </summary>
	void ChangeWindowUpdate();

	/// <summary>
	/// updateFuncの中身を変更する
	/// </summary>
	void ChangeUpdateFunc();

	/// <summary>
	/// シーンを切り替える
	/// </summary>
	void SceneChange();
private:

	int makeScreenHandle_ = -1;

	int volumeBGM_ = 0;
	int volumeSE_ = 0;

	int selectNum_ = 0;

	bool windowMode_ = true;					//ウィンドウモード

	//ここも消すかもしれない
	static constexpr int fadeInterval_ = 30;
	int fadeTimer_ = 0;
	int fadeValue_ = 0;
	int fadeColor_ = 0x000000;

	std::string windowModeText_ = "≪  ウィンドウモード  ≫";
	std::shared_ptr<UIItemManager> UIManager_;
	std::map<std::string, VECTOR> menuDrawPos_;	//メニューを描画する座標

	void(SettingSceneForSceneTitle::* updateFunc_)();
};

