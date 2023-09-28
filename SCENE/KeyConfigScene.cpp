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
	constexpr float graph_chip_size = 59.0f;
	//画像の隙間サイズ
	constexpr float graph_gap_size = 10.0f;
}

//コンストラクタ
KeyConfigScene::KeyConfigScene(SceneManager& manager, const InputState& input):SceneBase(manager),updateFunc_(&KeyConfigScene::selectChangeKeyUpdate),drawFunc_(&KeyConfigScene::keyStateDraw), inputState_(input)
{
}

//デストラクタ
KeyConfigScene::~KeyConfigScene()
{
}

void KeyConfigScene::init()
{
	makeScreenHandle_ = MakeScreen(Game::kScreenWidth, Game::kScreenHeight, true);
	keyTypeHandle_ = Graph::loadGraph("data/graph/key2.png");
}

void KeyConfigScene::end()
{
	//現在のキー入力情報を外部データとして書き出す
	//inputState_.savekeyInfo();
	inputState_.savekeyInfo2();
	DeleteGraph(makeScreenHandle_);
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
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ガウス処理を画像に施すか否か
	if (drawFunc_ == &KeyConfigScene::changeKeyPopUpText) {
		GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, 16, 50);
	}

	DrawGraph(0, 0, makeScreenHandle_, true);
	(this->*drawFunc_)();
	
	//----------------以降消去予定
	DrawString(0, 0, "keyConfig",0xffffff);
	DrawFormatString(0, 16, 0xffffff, "%d", selectNum_);
}

void KeyConfigScene::keyStateDraw()
{
	SetDrawScreen(makeScreenHandle_);
	ClearDrawScreen();

	float x = Game::kScreenWidth / 4;
	float y = Game::kScreenHeight / 2 - (graph_chip_size * 4 + 30.0f);
	int num = 0;

	int i = 0;
	for (auto& key : inputState_.inputNameTable_) {

		DrawFormatString(x, y - 8, textColor_, "%s", key.second.c_str());

		y += graph_chip_size + graph_gap_size;

		//inputstate.tempmaptableの半分を超えたら折り返す
		if (i == inputState_.tempMapTable_.size() / 2) {
			y = Game::kScreenHeight / 2 - (graph_chip_size * 4 + 30.0f);
			x += Game::kScreenWidth / 4 * 1.2f;
		}

		i++;
	}

	x = Game::kScreenWidth / 4 * 1.5f;
	y = Game::kScreenHeight / 2 - (graph_chip_size * 4 + 30.0f);
	i = 0;
	int alpha = 150;
	float graphScale = 1.0f;
	for (auto& key : inputState_.tempMapTable_) {
		num = getKeyName(key.second.begin()->id);
		int x2 = num % 9;
		int y2 = num / 9;

		if (i == selectNum_) {
			graphScale = 1.2f;
			alpha = 0;
		}
		else {
			graphScale = 1.0f;
			alpha = 150;
		}

		Graph::drawRectRotaGraph(x, y, x2 * graph_chip_size, y2 * graph_chip_size, graph_chip_size, graph_chip_size, graphScale, 0.0f, keyTypeHandle_, true, false);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawBox(x - 240, y - graph_chip_size / 2 - graph_gap_size / 2, x - 240 + Game::kScreenWidth / 4, y + graph_chip_size / 2 + graph_gap_size / 2, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		y += graph_chip_size + 10;
		if (i == inputState_.tempMapTable_.size() / 2) {
			y = Game::kScreenHeight / 2 - (graph_chip_size * 4 + 30.0f);
			x += Game::kScreenWidth / 4 * 1.2f;
		}

		i++;
	}

	//TODO:将来的には使わないから消す
	//一時的に見やすくするため
	int color = 0xffffff;
	int color2 = 0xffffff;
	if (selectNum_ == 15) {
		color = 0xff0000;
	}
	else if (selectNum_ == 16) {
		color2 = 0xff0000;
	}
	y += 50;
	DrawString(Game::kScreenWidth / 2 - 16, y, "変更", color);
	y += 16;
	DrawString(Game::kScreenWidth / 2 - 32, y, "キャンセル", color2);

	SetDrawScreen(DX_SCREEN_BACK);
}

void KeyConfigScene::changeKeyPopUpText()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(Game::kScreenWidth / 6, Game::kScreenHeight / 5, Game::kScreenWidth / 6 * 5, Game::kScreenHeight / 5 * 4, 0x444444, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//選択したキーの名前を出力
	DrawFormatString(Game::kScreenWidth / 6 + graph_chip_size / 2, Game::kScreenHeight / 5 - graph_chip_size / 2, 0xffffff, inputState_.inputNameTable_.find(static_cast<InputType>(selectNum_))->second.c_str());

	//選択したキーのidを取得して画像を分割する
	int num = getKeyName(inputState_.tempMapTable_.find(static_cast<InputType>(selectNum_))->second.begin()->id);
	int x = num % 9;
	int y = num / 9;
	//選択したキーの画像を出力
	Graph::drawRectRotaGraph(Game::kScreenWidth / 6 + graph_chip_size * 2, Game::kScreenHeight / 5 - graph_chip_size / 2, x * graph_chip_size, y * graph_chip_size, graph_chip_size, graph_chip_size, 1.0f, 0.0f, keyTypeHandle_, true, false);

	DrawString(Game::kScreenWidth / 2, Game::kScreenHeight / 2, "変更したいキーを入力してください", 0xffffff);

}

//変更したいキーを選択する
void KeyConfigScene::selectChangeKeyUpdate()
{
	//短縮化
	auto& configInput = const_cast<InputState&>(inputState_);
	//現在のキーの数を取得する
	const int keyNum = static_cast<int>(inputState_.inputNameTable_.size() + 2);

	int lastSelectNum = selectNum_;

	//TODO:まとめる
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
		if (inputState_.isTriggered(InputType::space)) {
			configInput.commitChangedInputInfo();		//仮のキー情報を実際に参照しているキー情報に代入する
			manager_.pushScene(std::shared_ptr<SceneBase>(std::make_shared<PopUpTextScene>(manager_)));		//シーンをポップアップテキストを描画するシーンに変更する
			return;
		}
	}

	//仮キー情報を消去してポーズシーンに戻る
	if (selectNum_ == inputState_.inputNameTable_.size() + 1) {
		if (inputState_.isTriggered(InputType::space)) {
			configInput.resetInputInfo();
			manager_.swapScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
			return;
		}
	}
	
	//どのキーを変更するかを仮決定
	if (inputState_.isTriggered(InputType::space)) {
		isEditing_ = !isEditing_;
		drawFunc_ = &KeyConfigScene::changeKeyPopUpText;
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
	//一フレーム前の入力情報が残っていたらリターンする
	for (const auto& key : inputState_.lastInput_) {
		if (key == true) return;
	}

	//短縮化
	auto& configInput = const_cast<InputState&>(inputState_);

	//メンバ関数ポインタを変更するキーを選択する関数に変更する
	if (inputState_.isTriggered(InputType::prev)) {
		updateFunc_ = &KeyConfigScene::selectChangeKeyUpdate;
		drawFunc_ = &KeyConfigScene::keyStateDraw;
		isEditing_ = !isEditing_;
		configInput.undoSelectKey(static_cast<InputType>(selectNum_), InputCategory::keybd);
		return;
	}

	//キーボードとパッドの入力を得る
	char keyState[256];
	auto padState = GetJoypadInputState(DX_INPUT_PAD1);
	GetHitKeyStateAll(keyState);

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
			isEditing_ = !isEditing_;
			updateFunc_ = &KeyConfigScene::selectChangeKeyUpdate;
			drawFunc_ = &KeyConfigScene::keyStateDraw;
			break;
		}
	}
	//パッドの入力を変更する
	if (padState != 0) {
		configInput.rewriteInputInfo(currentType, InputCategory::pad, padState);
		isEditing_ = !isEditing_;
		updateFunc_ = &KeyConfigScene::selectChangeKeyUpdate;
		drawFunc_ = &KeyConfigScene::keyStateDraw;
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
