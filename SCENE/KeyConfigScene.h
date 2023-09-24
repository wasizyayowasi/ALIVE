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

	//更新
	void update(const InputState& input);
	//描画
	void draw();

	//どの入力装置のキーを変更するかを選択する
	void selectChangeKeyUpdate();
	//変更するキーをどのキーに変更するのかを決定する
	void changeKeyUpdate();

	//形だけある今のところ
	int getKeyName(int num);

private:

	int aiu = 1;

	int keyTypeHandle_ = -1;	//keyTypeを描画するためのグラフを受け取るためのhandle

	int textColor_ = 0xff0000;		//カラー
	int selectNum_ = 0;			//現在の選択番号

	bool isEditing_ = false;	//編集中フラグ

	const InputState& inputState_;				//コンストラクタの引数input参照を受け取る

	void (KeyConfigScene::* updateFunc_)();		//メンバ関数ポインタ

};

