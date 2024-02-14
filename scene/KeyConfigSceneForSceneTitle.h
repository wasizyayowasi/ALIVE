#pragma once
#include "SceneBase.h"
#include "../util/KeyData.h"
#include <memory>
#include <map>
#include <string>
#include <DxLib.h>

class UIItemManager;

class KeyConfigSceneForSceneTitle : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">managerを参照し、シーンベースに引数として渡す</param>
	/// <param name="input">inputの情報を参照する</param>
	KeyConfigSceneForSceneTitle(SceneManager& manager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~KeyConfigSceneForSceneTitle();

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
	/// 現在のkey入力情報を描画
	/// </summary>
	void KeyStateDraw();

	/// <summary>
	/// keyの画像を描画
	/// </summary>
	void KeyGraphDraw();

	/// <summary>
	/// コントローラーが使用されている場合の描画
	/// </summary>
	void ControllerDraw();

	/// <summary>
	/// 変更したいkeyを入力させるためのポップアップ描画
	/// </summary>
	void ChangeKeyPopUpText();

	/// <summary>
	/// どの入力装置のキーを変更するかを選択する
	/// </summary>
	void SelectChangeKeyUpdate();

	/// <summary>
	/// 変更するキーをどのキーに変更するのかを決定する
	/// </summary>
	void ChangeKeyborardUpdate();

	/// <summary>
	/// コントローラーの場合の更新
	/// </summary>
	void ControllerUpdate();

private:

	//消すかもしれない
	void FadeInUpdate();
	void NormalUpdate();
	void FadeOutUpdate();

private:

	int keyTypeHandle_[117] = {};			//keyTypeを描画するためのグラフを受け取るためのhandle
	int fontHandleSize21_ = -1;				//フォント16サイズを保管する変数
	int fontHandleSize42_ = -1;				//フォント32サイズを保管する変数
	int selectNum_ = 0;						//現在の選択番号

	//ここも消すかもしれない
	static constexpr int fadeInterval_ = 30;
	int fadeTimer_ = 0;
	int fadeValue_ = 0;
	int fadeColor_ = 0x000000;

	bool isEditing_ = false;							//編集中フラグ

	std::shared_ptr<UIItemManager> KeyUI_;				//UIマネージャーのスマートポインタ
	std::shared_ptr<UIItemManager> PadUI_;				//UIマネージャーのスマートポインタ

	std::map<int, Key> keyNum_;
	std::map<std::string, VECTOR> keyDrawPos_;
	std::map<std::string, VECTOR> padDrawPos_;

	void (KeyConfigSceneForSceneTitle::* updateFunc_)();
	void (KeyConfigSceneForSceneTitle::* changeKeyUpdateFunc_)();	//メンバ関数ポインタ
	void (KeyConfigSceneForSceneTitle::* drawFunc_)();				//メンバ関数ポインタ
};

