#include "KeyConfigScene.h"
#include "SceneManager.h"
#include "ScenePause.h"
#include"../UTIL/InputState.h"
#include"../UTIL/game.h"
#include "DxLib.h"
#include <algorithm>

KeyConfigScene::KeyConfigScene(SceneManager& manager, const InputState& input):SceneBase(manager),updateFunc_(&KeyConfigScene::selectChangeKeyUpdate), inputState_(input)
{
}

KeyConfigScene::~KeyConfigScene()
{
	//現在のキー入力情報を外部データとして書き出す
	inputState_.savekeyInfo();
}

void KeyConfigScene::update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void KeyConfigScene::draw()
{
	//多分削除する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawFormatString(50, 16 * selectNum_ + 32, color_, "←");

	int y = 32;
	for (auto& keyName : inputState_.inputNameTable) {
		DrawFormatString(0, y, 0xffffff, "%s", keyName.second.c_str());
		y += 16;
	}

	DrawString(0, 0, "keyConfig",0xffffff);
	DrawFormatString(0, 16, 0xffffff, "%d", selectNum_);
}

void KeyConfigScene::selectChangeKeyUpdate(const InputState& input)
{

	auto& configInput = const_cast<InputState&>(input);
	
	const int keyNum = static_cast<int>(input.inputNameTable.size() + 2);

	//選択操作
	{
		if (input.isTriggered(InputType::up)) {
			selectNum_ = (std::max)(selectNum_ - 1, 0);
		}
		if (input.isTriggered(InputType::down)) {
			selectNum_ = (std::min)(selectNum_ + 1, keyNum - 1);
		}
	}

	
	//どのキーを変更するかを仮決定
	if (input.isTriggered(InputType::next)) {
		updateFunc_ = &KeyConfigScene::changeKeyUpdate;
		return;
	}

	//ひとつ前のシーンに戻る
	if (input.isTriggered(InputType::prev)) {
		manager_.swapScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
		return;
	}

}

void KeyConfigScene::changeKeyUpdate(const InputState& input)
{
	auto& configInput = const_cast<InputState&>(input);

	if (input.isTriggered(InputType::next)) {
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
		for (const auto& name : inputState_.inputNameTable) {
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
	

	if (input.isTriggered(InputType::prev)) {
		updateFunc_ = &KeyConfigScene::selectChangeKeyUpdate;
		isEditing_ = !isEditing_;
		color_ = 0xff0000;
		return;
	}

}
