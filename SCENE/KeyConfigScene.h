#pragma once
#include "SceneBase.h"

enum class KeySelect {
	next,			/// 次へ行くボタン
	prev,			/// 前に戻るボタン
	pause,			/// ポーズボタン
	up,				///上
	down,			///下
	left,			///左
	right,			///右
	space,			///space
	z,				//死亡入力
	shift,			//shift
	ctrl,			///ctrl
};

class KeyConfigScene : public SceneBase
{
public:

	KeyConfigScene(SceneManager& manager, const InputState& input);
	virtual ~KeyConfigScene();

	void update(const InputState& input);
	void draw();

	void selectChangeKeyUpdate(const InputState& input);
	void changeKeyUpdate(const InputState& input);

private:

	int color_ = 0xff0000;		//カラー
	int selectNum_ = 0;			//現在の選択番号

	bool isEditing_ = false;	//編集中フラグ

	const InputState& inputState_;

	void (KeyConfigScene::* updateFunc_)(const InputState& input);

};

