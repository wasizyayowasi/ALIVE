#pragma once
#include "SceneBase.h"

class KeyConfigScene : public SceneBase
{
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
	//keyの名前を描画
	void KeyNameDraw();
	//keyの画像を描画
	void KeyGraphDraw();

	//変更したいkeyを入力させるためのポップアップ描画
	void ChangeKeyPopUpText();

	//どの入力装置のキーを変更するかを選択する
	void SelectChangeKeyUpdate();
	//変更するキーをどのキーに変更するのかを決定する
	void ChangeKeyUpdate();

	//形だけある今のところ
	int GetKeyName(int num);

private:

	int fontHandle_ = -1;

	int makeScreenHandle_ = -1;

	int keyTypeHandle_ = -1;	//keyTypeを描画するためのグラフを受け取るためのhandle

	int textColor_ = 0xffffff;		//カラー
	int selectNum_ = 0;			//現在の選択番号

	bool isEditing_ = false;	//編集中フラグ

	const InputState& inputState_;				//コンストラクタの引数input参照を受け取る

	void (KeyConfigScene::* updateFunc_)();		//メンバ関数ポインタ
	void (KeyConfigScene::* drawFunc_)();		//メンバ関数ポインタ

};

