#pragma once
#include "SceneBase.h"
#include <memory>
#include <map>

class UIItemManager;

class KeyConfigScene : public SceneBase
{
private:

	enum class Key
	{
		ESC,
		Key_1,
		Key_2,
		Key_3,
		Key_4,
		Key_5,
		Key_6,
		Key_7,
		Key_8,
		Key_9,
		Key_0,
		Key_equal,

		Back,
		Tab,
		Q, W, E, R, T, Y, U, I, O, P,
		Left_parenthesis_key,
		Right_parenthesis_key,
		Big_enter,
		L_ctrl,
		A, S, D, F, G, H, J, K, L,
		Semicolon,
		L_shift,
		Backslash,

		Z,X,C,V,B,N,M,
		Comma,
		Period,
		Slash,
		R_shift,
		Asterisk,
		L_alt,
		Space,
		CaspLock,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		NumLock,
		ScrollLock,
		Key10_7,
		Key10_8,
		Key10_9,
		Key10_minus,
		Key10_4,
		Key10_5,
		Key10_6,
		Key10_plus,
		Key10_1,
		Key10_2,
		Key10_3,
		Key10_0,
		Key10_period,
		F11,
		F12,
		カナ,
		変換,
		無変換,
		YEN_mark,
		Caret,
		At_mark,
		Colon,
		漢字,
		Key10_enter,
		R_ctrl,
		Key10_slash,
		Print,
		Ralt,
		Pause,
		Home,
		Up_Arrow,
		Page_up,
		Left_Arrow,
		Right_Arrow,
		END,
		Down_Arrow,
		Page_Down,
		Insert,
		Delete,
		L_win,
		Rwin,
		App,
	};

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">managerを参照し、シーンベースに引数として渡す</param>
	/// <param name="input">inputの情報を参照する</param>
	KeyConfigScene(SceneManager& manager, const InputState& input);
	//デストラクタ
	virtual ~KeyConfigScene();

	//初期化用関数
	void Init();
	//削除用関数
	void End();

	//更新
	void Update(const InputState& input);
	//描画
	void Draw();

	//現在のkey入力情報を描画
	void KeyStateDraw();
	
	//keyの画像を描画
	void KeyGraphDraw();

	//コントローラーが使用されている場合の描画
	void ControllerDraw();

	//変更したいkeyを入力させるためのポップアップ描画
	void ChangeKeyPopUpText();

	//どの入力装置のキーを変更するかを選択する
	void SelectChangeKeyUpdate();
	//変更するキーをどのキーに変更するのかを決定する
	void ChangeKeyborardUpdate();
	//コントローラーの場合の更新
	void ControllerUpdate();

private:

	//消すかもしれない
	void FadeInUpdate(const InputState& input);
	void NormalUpdate(const InputState& input);
	void FadeOutUpdate(const InputState& input);

private:

	int controllerHandle_ = -1;				//コントローラー画像
	int fontHandleSize21_ = -1;				//フォント16サイズを保管する変数
	int fontHandleSize42_ = -1;				//フォント32サイズを保管する変数
	int makeScreenHandle_ = -1;				//作成したハンドルを保管する変数
	int keyTypeHandle_ = -1;				//keyTypeを描画するためのグラフを受け取るためのhandle
	int textColor_ = 0xffffff;				//カラー
	int selectNum_ = 0;						//現在の選択番号

	//ここも消すかもしれない
	static constexpr int fadeInterval_ = 30;
	int fadeTimer_ = 0;
	int fadeValue_ = 0;
	int fadeColor_ = 0x000000;

	bool isEditing_ = false;							//編集中フラグ

	const InputState& inputState_;						//コンストラクタの引数input参照を受け取る

	std::shared_ptr<UIItemManager> KeyUI_;				//UIマネージャーのスマートポインタ
	std::shared_ptr<UIItemManager> PadUI_;				//UIマネージャーのスマートポインタ

	std::map<int, Key> keyNum_;

	void (KeyConfigScene::* updateFunc_)(const InputState& input);
	void (KeyConfigScene::* changeKeyUpdateFunc_)();	//メンバ関数ポインタ
	void (KeyConfigScene::* drawFunc_)();				//メンバ関数ポインタ

};

