#include "KeyConfigScene.h"
#include "SceneManager.h"
#include "ScenePause.h"
#include "PopUpTextScene.h"
#include"../util/InputState.h"
#include"../util/game.h"
#include"../util/DrawFunctions.h"
#include "DxLib.h"
#include <algorithm>

//コンストラクタ
KeyConfigScene::KeyConfigScene(SceneManager& manager, const InputState& input):SceneBase(manager),updateFunc_(&KeyConfigScene::selectChangeKeyUpdate), inputState_(input)
{
	keyTypeHandle_ = Graph::loadGraph("data/graph/key.png");
}

//デストラクタ
KeyConfigScene::~KeyConfigScene()
{
	//現在のキー入力情報を外部データとして書き出す
	//inputState_.savekeyInfo();
	inputState_.savekeyInfo2();
	DeleteGraph(keyTypeHandle_);
}

//メンバ関数ポインタの更新
void KeyConfigScene::update(const InputState& input)
{
	(this->*updateFunc_)();
}

//描画
void KeyConfigScene::draw()
{
	//多分削除する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawFormatString(50, 16 * selectNum_ + 32, color_, "←");

	int y = 32;
	for (auto& keyName : inputState_.inputNameTable_) {
		DrawFormatString(0, y, 0xffffff, "%s", keyName.second.c_str());
		y += 16;
	}

	DrawString(0, y, "変更", 0xffffff);
	y += 16;
	DrawString(0, y, "キャンセル", 0xffffff);

	
	//----------------以降消去予定
	DrawString(0, 0, "keyConfig",0xffffff);
	DrawFormatString(0, 16, 0xffffff, "%d", selectNum_);
}

//変更したいキーを選択する
void KeyConfigScene::selectChangeKeyUpdate()
{
	//短縮化
	auto& configInput = const_cast<InputState&>(inputState_);
	//現在のキーの数を取得する
	const int keyNum = static_cast<int>(inputState_.inputNameTable_.size() + 2);

	//選択操作
	{
		if (inputState_.isPressed(InputType::up)) {
			selectNum_ = (std::max)(selectNum_ - 1, 0);
		}
		if (inputState_.isPressed(InputType::down)) {
			selectNum_ = (std::min)(selectNum_ + 1, keyNum - 1);
		}
	}

	//キーの変更を保存する
	if (selectNum_ == inputState_.inputNameTable_.size()) {
		if (inputState_.isTriggered(InputType::next)) {
			configInput.commitChangedInputInfo();		//仮のキー情報を実際に参照しているキー情報に代入する
			manager_.pushScene(std::shared_ptr<SceneBase>(std::make_shared<PopUpTextScene>(manager_)));		//シーンをポップアップテキストを描画するシーンに変更する
			return;
		}
	}

	//仮キー情報を消去してポーズシーンに戻る
	if (selectNum_ == inputState_.inputNameTable_.size() + 1) {
		if (inputState_.isTriggered(InputType::next)) {
			configInput.resetInputInfo();
			manager_.swapScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
			return;
		}
	}
	
	//どのキーを変更するかを仮決定
	if (inputState_.isTriggered(InputType::next)) {
		updateFunc_ = &KeyConfigScene::changeKeyUpdate;
		return;
	}

	//ひとつ前のシーンに戻る
	if (inputState_.isTriggered(InputType::prev)) {
		manager_.swapScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
		return;
	}

}

//変更したいキーをどのキーに変更するのか
void KeyConfigScene::changeKeyUpdate()
{
	//短縮化
	auto& configInput = const_cast<InputState&>(inputState_);

	//変更途中の場合矢印の色を変える
	if (inputState_.isTriggered(InputType::next)) {
		isEditing_ = !isEditing_;
		if (isEditing_) {
			color_ = 0x00ff00;
		}
		else {
			color_ = 0xff0000;
		}
	}

	//キーボードとパッドの入力を得る
	if (isEditing_) {
		char keyState[256];
		GetHitKeyStateAll(keyState);
		auto padState = GetJoypadInputState(DX_INPUT_PAD1);

		int idx = 0;
		InputType currentType = InputType::max;
		//現在の選択inputNameTableを取得する
		for (const auto& name : inputState_.inputNameTable_) {
			if (selectNum_ == idx) {
				currentType = name.first;
				break;
			}
			idx++;
		}

		//キーボードの入力を変更する
		for (int i = 0; i < 256; i++) {
			if (keyState[i]) {
				configInput.rewriteInputInfo(currentType, InputCategory::keybd, i);
				break;
			}
		}
		//パッドの入力を変更する
		if (padState != 0) {
			configInput.rewriteInputInfo(currentType, InputCategory::pad, padState);
		}
	}
	
	//メンバ関数ポインタを変更するキーを選択する関数に変更する
	if (inputState_.isTriggered(InputType::prev)) {
		updateFunc_ = &KeyConfigScene::selectChangeKeyUpdate;
		isEditing_ = !isEditing_;
		color_ = 0xff0000;
		return;
	}

}

char KeyConfigScene::getKeyName(int num)
{
	switch (num) {
	
	}

	return 0;
}
