#include "KeyConfigScene.h"
#include "SceneManager.h"
#include "ScenePause.h"
#include "PopUpTextScene.h"
#include"../util/InputState.h"
#include"../util/game.h"
#include"../util/DrawFunctions.h"
#include"../util/FontsManager.h"
#include "../util/UIItemManager.h"
#include "DxLib.h"
#include <algorithm>

namespace {
	//key画像の一枚当たりのサイズ
	constexpr int graph_chip_size = 59;
	//画像の隙間サイズ
	constexpr int graph_gap_size = 10;
}

//コンストラクタ
KeyConfigScene::KeyConfigScene(SceneManager& manager, const InputState& input):SceneBase(manager),updateFunc_(&KeyConfigScene::SelectChangeKeyUpdate),drawFunc_(&KeyConfigScene::KeyStateDraw), inputState_(input)
{
}

//デストラクタ
KeyConfigScene::~KeyConfigScene()
{
}

void KeyConfigScene::Init()
{
	//フォントの作成
	fontHandleSize16_ = FontsManager::getInstance().GetFontHandle("ピグモ 0021");
	fontHandleSize32_ = FontsManager::getInstance().GetFontHandle("ピグモ 0042");
	//インスタンス化
	UI_ = std::make_shared<UIItemManager>();

	//キーの役割の一つ目の座標
	int namePosX = Game::screen_width / 4;
	int namePosY = Game::screen_height / 2 - (graph_chip_size * 4 + 45);

	int nameNo = 0;
	for (auto& table : inputState_.inputNameTable_) {
		//メニューの追加
		UI_->addMenu(namePosX, namePosY, 320, 100, table.second.c_str(), fontHandleSize16_);

		//ポジション調整
		namePosY += graph_chip_size + graph_gap_size;

		//inputstate.tempmaptableの半分を超えたら折り返す
		if (nameNo == inputState_.tempMapTable_.size() / 2 - 1) {
			namePosY = Game::screen_height / 2 - (graph_chip_size * 4 + 30);
			namePosX += Game::screen_width / 4 * 1.5f;
		}

		nameNo++;
	}

	//メニューの追加
	UI_->addMenu(Game::screen_width / 2, Game::screen_height / 5 * 4, 320, 100, "変更", fontHandleSize32_);
	UI_->addMenu(Game::screen_width / 2, Game::screen_height / 5 * 4 + 32, 320, 100, "キャンセル", fontHandleSize32_);

	//スクリーンサイズのハンドルを作成
	makeScreenHandle_ = MakeScreen(Game::screen_width, Game::screen_height, true);
	//キーグラフを読み込む
	keyTypeHandle_ = Graph::LoadGraph("data/graph/key2.png");
}

void KeyConfigScene::End()
{
	//現在のキー入力情報を外部データとして書き出す
	//inputState_.savekeyInfo();
	inputState_.SavekeyInfo2();
	DeleteGraph(makeScreenHandle_);
	DeleteGraph(keyTypeHandle_);
}

//メンバ関数ポインタの更新
void KeyConfigScene::Update(const InputState& input)
{
	(this->*updateFunc_)();
}

//描画
void KeyConfigScene::Draw()
{
	//背景に黒の透過
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//makeScreenで作成したハンドルを描画
	DrawGraph(0, 0, makeScreenHandle_, true);

	(this->*drawFunc_)();
	
	//----------------以降消去予定
	DrawString(0, 0, "keyConfig",0xffffff);
//	DrawLine(Game::screen_width / 2, 0, Game::screen_width / 2, Game::screen_height, 0xff0000);
//	DrawLine(Game::screen_width / 4, 0, Game::screen_width / 4, Game::screen_height, 0xff0000);
//	DrawLine(Game::screen_width / 4 * 3, 0, Game::screen_width / 4 * 3, Game::screen_height, 0xff0000);
//	DrawLine(Game::screen_width / 6, 0, Game::screen_width / 6, Game::screen_height, 0xff0000);
}

void KeyConfigScene::KeyStateDraw()
{
	//書き込みスクリーンの変更
	SetDrawScreen(makeScreenHandle_);
	//スクリーンのクリア
	ClearDrawScreen();

	//キーの役割を描画する
	UI_->draw(selectNum_);

	//キーに対応する画像を描画する
	KeyGraphDraw();

	SetDrawScreen(DX_SCREEN_BACK);

}

void KeyConfigScene::KeyGraphDraw()
{
	//ポジション設定
	float graphPosX = Game::screen_width / 4 * 1.5f;
	float graphPosY = Game::screen_height / 2 - (graph_chip_size * 4 + 30.0f);
	//dxlib内の各キー番号を入手する
	int keyNum = 0;
	//for文で何番目かを取得する
	int keyNo = 0;
	//明るさを引く値
	int subBrightness = 180;
	//画像の拡縮率
	float graphScale = 1.0f;

	for (auto& key : inputState_.tempMapTable_) {
		//key番号を取得する
		keyNum = GetKeyName(key.second.begin()->id);
		//画像を分割するための配列番号を取得する
		int graphArrayX = keyNum % 9;
		int graphArrayY = keyNum / 9;

		//現在カーソルがあっている場合
		//サイズと明るさを引く値を変更する
		if (keyNo == selectNum_) {
			graphScale = 1.2f;
			subBrightness = 0;
		}
		else {
			graphScale = 1.0f;
			subBrightness = 180;
		}

		//分割画像の描画
		Graph::DrawRectRotaGraph(graphPosX, graphPosY, graphArrayX * graph_chip_size, graphArrayY * graph_chip_size, graph_chip_size, graph_chip_size, graphScale, 0.0f, keyTypeHandle_, true, false);

		//暗くした画像を画像の上に乗せる
		SetDrawBlendMode(DX_BLENDMODE_SUB, subBrightness);
		DrawBoxAA(graphPosX - 80.0f, graphPosY - graph_chip_size / 2 - graph_gap_size / 2, graphPosX - 240.0f + Game::screen_width / 4, graphPosY + graph_chip_size / 2 + graph_gap_size / 2, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//半分を超えたら折り返す処理
		graphPosY += graph_chip_size + 10;
		if (keyNo == inputState_.tempMapTable_.size() / 2 - 1) {
			graphPosY = Game::screen_height / 2 - (graph_chip_size * 4 + 30);
			graphPosX += Game::screen_width / 4 * 1.5f;
		}

		keyNo++;
	}
}

void KeyConfigScene::ChangeKeyPopUpText()
{
	//背景の黒描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(Game::screen_width / 6, Game::screen_height / 5, Game::screen_width / 6 * 5, Game::screen_height / 5 * 4, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//文字列
	const char* text = inputState_.inputNameTable_.find(static_cast<InputType>(selectNum_))->second.c_str();
	//文字列の横幅
	int strWidth = GetDrawStringWidthToHandle(text, strlen(text), fontHandleSize16_);
	//選択したキーの名前を出力
	DrawStringToHandle(Game::screen_width / 6 + strWidth / 2 , Game::screen_height / 5 - graph_chip_size / 2 - 8, text, 0xffffff, fontHandleSize16_);

	//選択したキーのidを取得して画像を分割する
	int num = GetKeyName(inputState_.tempMapTable_.find(static_cast<InputType>(selectNum_))->second.begin()->id);
	int x = num % 9;
	int y = num / 9;
	//選択したキーの画像を出力
	Graph::DrawRectRotaGraph(Game::screen_width / 6 + strWidth *2.5f, Game::screen_height / 5 - graph_chip_size / 2, x * graph_chip_size, y * graph_chip_size, graph_chip_size, graph_chip_size, 1.0f, 0.0f, keyTypeHandle_, true, false);

	//文字列
	text = "変更したいキーを入力してください";
	//文字列の横幅
	strWidth = GetDrawStringWidthToHandle(text, strlen(text), fontHandleSize32_);
	//文字列の描画
	DrawStringToHandle(Game::screen_width / 2 - strWidth / 2, Game::screen_height / 2, text, 0xffffff, fontHandleSize32_);

}

//変更したいキーを選択する
void KeyConfigScene::SelectChangeKeyUpdate()
{
	//短縮化
	auto& configInput = const_cast<InputState&>(inputState_);
	//現在のキーの数を取得する
	const int keyNum = static_cast<int>(inputState_.inputNameTable_.size() + 2);

	int lastSelectNum = selectNum_;

	//TODO:まとめる
	//選択操作
	{
		if (inputState_.IsTriggered(InputType::up)) {
			selectNum_ = ((selectNum_ - 1) + keyNum) % keyNum;
		}
		if (inputState_.IsTriggered(InputType::down)) {
			selectNum_ = (selectNum_ + 1)% keyNum;
		}
		if (inputState_.IsTriggered(InputType::left) || inputState_.IsTriggered(InputType::right)) {
			selectNum_ = (selectNum_ + (keyNum / 2 - 1)) % (keyNum - 2);
		}
	}

	//キーの変更を保存する
	if (selectNum_ == inputState_.inputNameTable_.size()) {
		if (inputState_.IsTriggered(InputType::space)) {
			configInput.CommitChangedInputInfo();		//仮のキー情報を実際に参照しているキー情報に代入する
			manager_.PushScene(std::shared_ptr<SceneBase>(std::make_shared<PopUpTextScene>(manager_)));		//シーンをポップアップテキストを描画するシーンに変更する
			return;
		}
	}

	//仮キー情報を消去してポーズシーンに戻る
	if (selectNum_ == inputState_.inputNameTable_.size() + 1) {
		if (inputState_.IsTriggered(InputType::space)) {
			configInput.ResetInputInfo();
			manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
			return;
		}
	}
	
	//どのキーを変更するかを仮決定
	if (inputState_.IsTriggered(InputType::space)) {
		isEditing_ = !isEditing_;
		drawFunc_ = &KeyConfigScene::ChangeKeyPopUpText;
		updateFunc_ = &KeyConfigScene::ChangeKeyUpdate;
		GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, 32, 800);
		return;
	}

	//ひとつ前のシーンに戻る
	if (inputState_.IsTriggered(InputType::pause)) {
		manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
		configInput.RollbackChangedInputInfo();
		return;
	}

}

//変更したいキーをどのキーに変更するのか
void KeyConfigScene::ChangeKeyUpdate()
{
	//一フレーム前の入力情報が残っていたらリターンする
	for (const auto& key : inputState_.lastInput_) {
		if (key == true) return;
	}

	//短縮化
	auto& configInput = const_cast<InputState&>(inputState_);

	//メンバ関数ポインタを変更するキーを選択する関数に変更する
	if (inputState_.IsTriggered(InputType::pause)) {
		updateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
		drawFunc_ = &KeyConfigScene::KeyStateDraw;
		isEditing_ = !isEditing_;
		configInput.UndoSelectKey(static_cast<InputType>(selectNum_), InputCategory::keybd);
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
			configInput.RewriteInputInfo(currentType, InputCategory::keybd, i);
			isEditing_ = !isEditing_;
			updateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
			drawFunc_ = &KeyConfigScene::KeyStateDraw;
			break;
		}
	}
	//パッドの入力を変更する
	if (padState != 0) {
		configInput.RewriteInputInfo(currentType, InputCategory::pad, padState);
		isEditing_ = !isEditing_;
		updateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
		drawFunc_ = &KeyConfigScene::KeyStateDraw;
	}

}

int KeyConfigScene::GetKeyName(int num)
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
