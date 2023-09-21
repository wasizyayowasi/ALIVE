#include "KeyConfigScene.h"
#include "SceneManager.h"
#include "ScenePause.h"
#include "PopUpTextScene.h"
#include"../util/InputState.h"
#include"../util/game.h"
#include"../util/DrawFunctions.h"
#include "DxLib.h"
#include <algorithm>

namespace {
	//key画像の一枚当たりのサイズ
	constexpr int graph_chip_size = 59;
}

//コンストラクタ
KeyConfigScene::KeyConfigScene(SceneManager& manager, const InputState& input):SceneBase(manager),updateFunc_(&KeyConfigScene::selectChangeKeyUpdate), inputState_(input)
{
	keyTypeHandle_ = Graph::loadGraph("data/graph/key2.png");
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

	int cslX = 50;
	int cslY = graph_chip_size * 2 * selectNum_ + 32;
	if (selectNum_ > 7) {
		cslX += 450;
		cslY = graph_chip_size * 2 * (selectNum_ - 8) + 32;
	}
	DrawFormatString(cslX, cslY, color_, "←");

	
	int x = 0;
	int y = 32;
	int num = 0;

	for (auto& key : inputState_.inputNameTable_) {
		DrawFormatString(x, y, 0xffffff, "%s", key.second.c_str());
		y += graph_chip_size * 2;
		if (y > Game::kScreenHeight) {
			y = 32;
			x += 400;
		}
	}

	y = 32;
	x = 150;
	for (auto& key : inputState_.tempMapTable_) {
		num = getKeyName(key.second.begin()->id);
		int x2 = num % 9;
		int y2 = num / 9;
		Graph::drawRectRotaGraph(x, y, x2 * graph_chip_size, y2 * graph_chip_size, graph_chip_size, graph_chip_size, 1.0f, 0.0f, keyTypeHandle_, true, false);
		y += graph_chip_size * 2;
		if (y > Game::kScreenHeight) {
			y = 32;
			x += 400;
		}
	}
	

	DrawString(400, y, "変更", 0xffffff);
	y += 16;
	DrawString(400, y, "キャンセル", 0xffffff);

	
	//----------------以降消去予定
	DrawString(0, 0, "keyConfig",0xffffff);
	DrawFormatString(0, 16, 0xffffff, "%d", selectNum_);
	DrawFormatString(0, y + 16, 0xffffff, "%d", aiu);
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
		if (inputState_.isTriggered(InputType::up)) {
			selectNum_ = (std::max)(selectNum_ - 1, 0);
		}
		if (inputState_.isTriggered(InputType::down)) {
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
		configInput.rollbackChangedInputInfo();
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

int KeyConfigScene::getKeyName(int num)
{
	if (num < 12 && num > 0) {
		return num - 1;
	}
	
	if (num >= 14 && num <= 39) {
		return num - 2;
	}

	if (num >= 42 && num <= 83) {
		return num - 4;
	}

	switch (num) {
	case 87:
		return 80;
	case 88:
		return 81;
	case 112:
		return 82;
	case 121:
		return 83;
	case 123:
		return 84;
	case 125:
		return 85;
	case 144:
		return 86;
	case 145:
		return 87;
	case 146:
		return 88;
	case 148:
		return 89;
	case 156:
		return 90;
	case 157:
		return 91;
	case 181:
		return 92;
	case 183:
		return 93;
	case 184:
		return 94;
	case 197:
		return 95;
	case 199:
		return 96;
	case 200:
		return 97;
	case 201:
		return 98;
	case 203:
		return 99;
	case 205:
		return 100;
	case 207:
		return 101;
	case 208:
		return 102;
	case 209:
		return 103;
	case 210:
		return 104;
	case 211:
		return 105;
	case 219:
		return 106;
	case 220:
		return 107;
	case 221:
		return 108;
	}

	return num;
}
