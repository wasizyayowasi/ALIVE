#include "KeyConfigScene.h"
#include "SceneManager.h"
#include "SettingScene.h"
#include "PopUpTextScene.h"

#include"../util/InputState.h"
#include"../util/game.h"
#include"../util/DrawFunctions.h"
#include"../util/FontsManager.h"
#include "../util/UIItemManager.h"

#include <DxLib.h>
#include <algorithm>

namespace {
	//key画像の一枚当たりのサイズ
	constexpr int graph_chip_size = 59;
	//画像の隙間サイズ
	constexpr int graph_gap_size = 10;
}

//コンストラクタ
KeyConfigScene::KeyConfigScene(SceneManager& manager):SceneBase(manager),
updateFunc_(&KeyConfigScene::FadeInUpdate)
{
	//短縮化
	auto& input = InputState::GetInstance();

	if (!input.LastInputDevice()) {
		changeKeyUpdateFunc_ = &KeyConfigScene::ControllerUpdate;
		drawFunc_ = &KeyConfigScene::ControllerDraw;
	}
	else {
		changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
		drawFunc_ = &KeyConfigScene::KeyStateDraw;
	}

	{
		keyNum_[0] = Key::ESC;
		keyNum_[1] = Key::Key_1;
		keyNum_[2] = Key::Key_2;
		keyNum_[3] = Key::Key_3;
		keyNum_[4] = Key::Key_4;
		keyNum_[5] = Key::Key_5;
		keyNum_[6] = Key::Key_6;
		keyNum_[7] = Key::Key_7;
		keyNum_[8] = Key::Key_8;
		keyNum_[9] = Key::Key_9;
		keyNum_[10] = Key::Key_0;
		keyNum_[11] = Key::Key_equal;

		keyNum_[14] = Key::Back;
		keyNum_[15] = Key::Tab;
		keyNum_[16] = Key::Q;
		keyNum_[17] = Key::W;
		keyNum_[18] = Key::E;
		keyNum_[19] = Key::R;
		keyNum_[20] = Key::T;
		keyNum_[21] = Key::Y;
		keyNum_[22] = Key::U;
		keyNum_[23] = Key::I;
		keyNum_[24] = Key::O;
		keyNum_[25] = Key::P;
		keyNum_[26] = Key::Left_parenthesis_key;
		keyNum_[27] = Key::Right_parenthesis_key;
		keyNum_[28] = Key::Big_enter;
		keyNum_[29] = Key::L_ctrl;
		keyNum_[30] = Key::A;
		keyNum_[31] = Key::S;
		keyNum_[32] = Key::D;
		keyNum_[33] = Key::F;
		keyNum_[34] = Key::G;
		keyNum_[35] = Key::H;
		keyNum_[36] = Key::J;
		keyNum_[37] = Key::K;
		keyNum_[38] = Key::L;
		keyNum_[39] = Key::Semicolon;

		keyNum_[42] = Key::L_shift;
		keyNum_[43] = Key::Backslash;
		keyNum_[44] = Key::Z;
		keyNum_[45] = Key::X;
		keyNum_[46] = Key::C;
		keyNum_[47] = Key::V;
		keyNum_[48] = Key::B;
		keyNum_[49] = Key::N;
		keyNum_[50] = Key::M;
		keyNum_[51] = Key::Comma;
		keyNum_[52] = Key::Period;
		keyNum_[53] = Key::Slash;
		keyNum_[54] = Key::R_shift;
		keyNum_[55] = Key::Asterisk;
		keyNum_[56] = Key::L_alt;
		keyNum_[57] = Key::Space;
		keyNum_[58] = Key::CaspLock;
		keyNum_[59] = Key::F1;
		keyNum_[60] = Key::F2;
		keyNum_[61] = Key::F3;
		keyNum_[62] = Key::F4;
		keyNum_[63] = Key::F5;
		keyNum_[64] = Key::F6;
		keyNum_[65] = Key::F7;
		keyNum_[66] = Key::F8;
		keyNum_[67] = Key::F9;
		keyNum_[68] = Key::F10;
		keyNum_[69] = Key::NumLock;
		keyNum_[70] = Key::ScrollLock;
		keyNum_[71] = Key::Key10_7;
		keyNum_[72] = Key::Key10_8;
		keyNum_[73] = Key::Key10_9;
		keyNum_[74] = Key::Key10_minus;
		keyNum_[75] = Key::Key10_4;
		keyNum_[76] = Key::Key10_5;
		keyNum_[77] = Key::Key10_6;
		keyNum_[78] = Key::Key10_plus;
		keyNum_[79] = Key::Key10_1;
		keyNum_[80] = Key::Key10_2;
		keyNum_[81] = Key::Key10_3;
		keyNum_[82] = Key::Key10_0;
		keyNum_[83] = Key::Key10_period;

		keyNum_[87] = Key::F11;
		keyNum_[88] = Key::F12;
		keyNum_[112] = Key::KANA;
		keyNum_[121] = Key::変換;
		keyNum_[123] = Key::無変換;
		keyNum_[125] = Key::YEN_mark;
		keyNum_[144] = Key::Caret;
		keyNum_[145] = Key::At_mark;
		keyNum_[146] = Key::Colon;
		keyNum_[148] = Key::漢字;
		keyNum_[156] = Key::Key10_enter;
		keyNum_[157] = Key::R_ctrl;
		keyNum_[181] = Key::Key10_slash;
		keyNum_[183] = Key::Print;
		keyNum_[184] = Key::Ralt;
		keyNum_[197] = Key::Pause;
		keyNum_[199] = Key::Home;
		keyNum_[200] = Key::Up_Arrow;
		keyNum_[201] = Key::Page_up;
		keyNum_[203] = Key::Left_Arrow;
		keyNum_[205] = Key::Right_Arrow;
		keyNum_[207] = Key::END;
		keyNum_[208] = Key::Down_Arrow;
		keyNum_[209] = Key::Page_Down;
		keyNum_[210] = Key::Insert;
		keyNum_[211] = Key::Delete;
		keyNum_[219] = Key::L_win;
		keyNum_[220] = Key::Rwin;
		keyNum_[221] = Key::App;
	}
	
}

//デストラクタ
KeyConfigScene::~KeyConfigScene()
{
}

void KeyConfigScene::Init()
{
	//画像の読み込み
	controllerHandle_ = LoadGraph("data/graph/controller.png");

	//フォントの作成
	fontHandleSize21_ = FontsManager::GetInstance().GetFontHandle("ピグモ 0021");
	fontHandleSize42_ = FontsManager::GetInstance().GetFontHandle("ピグモ 0042");
	//インスタンス化
	KeyUI_ = std::make_shared<UIItemManager>();
	PadUI_ = std::make_shared<UIItemManager>();

	//短縮化
	auto& input = InputState::GetInstance();

	//キーの役割の一つ目の座標
	int namePosX = Game::screen_width / 4;
	int namePosY = Game::screen_height / 2 - (graph_chip_size * 4 + 45);

	int nameNo = 0;
	for (auto& table : input.inputNameTable_) {
		//メニューの追加
		KeyUI_->AddMenu(namePosX, namePosY, 320, 100, table.second.c_str(), fontHandleSize21_);

		//ポジション調整
		namePosY += graph_chip_size + graph_gap_size;

		//inputstate.tempmaptableの半分を超えたら折り返す
		if (nameNo == input.tempMapTable_.size() / 2 - 1) {
			namePosY = Game::screen_height / 2 - (graph_chip_size * 4 + 30);
			namePosX += Game::screen_width / 4 * 1.5f;
		}

		nameNo++;
	}

	//メニューの追加
	KeyUI_->AddMenu(Game::screen_width / 2, Game::screen_height / 5 * 4, 320, 100, "変更", fontHandleSize42_);
	KeyUI_->AddMenu(Game::screen_width / 2, Game::screen_height / 5 * 4 + 32, 320, 100, "キャンセル", fontHandleSize42_);
	PadUI_->AddMenu(Game::screen_width / 2, Game::screen_height / 5 * 4 + 32, 320, 100, "キャンセル", fontHandleSize42_);

	//スクリーンサイズのハンドルを作成
	makeScreenHandle_ = MakeScreen(Game::screen_width, Game::screen_height, true);
	//キーグラフを読み込む
	keyTypeHandle_ = Graph::LoadGraph("data/graph/key2.png");
}

void KeyConfigScene::End()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//現在のキー入力情報を外部データとして書き出す
	input.SavekeyInfo();
	DeleteGraph(makeScreenHandle_);
	DeleteGraph(keyTypeHandle_);
	DeleteGraph(controllerHandle_);
}

//メンバ関数ポインタの更新
void KeyConfigScene::Update()
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

	GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, 8, 255 - fadeValue_);

	//makeScreenで作成したハンドルを描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawGraph(0, 0, makeScreenHandle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

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
	KeyUI_->AlphaChangeDraw(selectNum_,fadeValue_);

	//キーに対応する画像を描画する
	KeyGraphDraw();

	SetDrawScreen(DX_SCREEN_BACK);

}

void KeyConfigScene::KeyGraphDraw()
{
	//短縮化
	auto& input = InputState::GetInstance();

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

	for (auto& key : input.tempMapTable_) {
		//key番号を取得する
		keyNum = static_cast<int>(keyNum_[key.second.begin()->id]);
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
		if (keyNo == input.tempMapTable_.size() / 2 - 1) {
			graphPosY = Game::screen_height / 2 - (graph_chip_size * 4 + 30);
			graphPosX += Game::screen_width / 4 * 1.5f;
		}

		keyNo++;
	}
}

void KeyConfigScene::ControllerDraw()
{
	//書き込みスクリーンの変更
	SetDrawScreen(makeScreenHandle_);
	//スクリーンのクリア
	ClearDrawScreen();

	PadUI_->AlphaChangeDraw(selectNum_, fadeValue_);

	DrawGraph(0, 0, controllerHandle_, true);

	SetDrawScreen(DX_SCREEN_BACK);
}

void KeyConfigScene::ChangeKeyPopUpText()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//背景の黒描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(Game::screen_width / 6, Game::screen_height / 5, Game::screen_width / 6 * 5, Game::screen_height / 5 * 4, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//文字列
	const char* text = input.inputNameTable_.find(static_cast<InputType>(selectNum_))->second.c_str();
	//文字列の横幅
	int strWidth = GetDrawStringWidthToHandle(text, strlen(text), fontHandleSize21_);
	//選択したキーの名前を出力
	DrawStringToHandle(Game::screen_width / 6 + strWidth / 2 , Game::screen_height / 5 - graph_chip_size / 2 - 8, text, 0xffffff, fontHandleSize21_);

	//選択したキーのidを取得して画像を分割する
	int num = static_cast<int>(keyNum_[input.tempMapTable_.find(static_cast<InputType>(selectNum_))->second.begin()->id]);
	int x = num % 9;
	int y = num / 9;
	//選択したキーの画像を出力
	Graph::DrawRectRotaGraph(Game::screen_width / 6 + strWidth *2.5f, Game::screen_height / 5 - graph_chip_size / 2, x * graph_chip_size, y * graph_chip_size, graph_chip_size, graph_chip_size, 1.0f, 0.0f, keyTypeHandle_, true, false);

	//文字列
	text = "変更したいキーを入力してください";
	//文字列の横幅
	strWidth = GetDrawStringWidthToHandle(text, strlen(text), fontHandleSize42_);
	//文字列の描画
	DrawStringToHandle(Game::screen_width / 2 - strWidth / 2, Game::screen_height / 2, text, 0xffffff, fontHandleSize42_);

}

//変更したいキーを選択する
void KeyConfigScene::SelectChangeKeyUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();
	//短縮化
	auto& configInput = const_cast<InputState&>(input);
	//現在のキーの数を取得する
	const int keyNum = static_cast<int>(input.inputNameTable_.size() + 2);

	int lastSelectNum = selectNum_;

	//TODO:まとめる
	//選択操作
	{
		if (input.IsTriggered(InputType::up)) {
			if (selectNum_ == (keyNum - 1) / 2) {
				selectNum_ += keyNum / 2;
			}
			else {
				selectNum_ = ((selectNum_ - 1) + keyNum) % keyNum;
			}
		}
		if (input.IsTriggered(InputType::down)) {
			if (selectNum_ + 1 == (keyNum - 1) / 2) {
				selectNum_ += keyNum / 2;
			}
			else {
				selectNum_ = (selectNum_ + 1) % keyNum;
			}
		}
		if (input.IsTriggered(InputType::left) || input.IsTriggered(InputType::right)) {
			selectNum_ = (selectNum_ + (keyNum / 2)) % (keyNum - 2);
		}
	}

	//キーの変更を保存する
	if (selectNum_ == input.inputNameTable_.size()) {
		if (input.IsTriggered(InputType::space)) {

			//仮のキー情報を実際に参照しているキー情報に代入する
			configInput.CommitChangedInputInfo();	

			//シーンをポップアップテキストを描画するシーンに変更する
			manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<PopUpTextScene>(manager_)));		
			return;
		}
	}

	//仮キー情報を消去してポーズシーンに戻る
	if (selectNum_ == input.inputNameTable_.size() + 1) {
		if (input.IsTriggered(InputType::space)) {
			configInput.ResetInputInfo();
			updateFunc_ = &KeyConfigScene::FadeOutUpdate;
			return;
		}
	}
	
	//どのキーを変更するかを仮決定
	if (input.IsTriggered(InputType::space)) {
		isEditing_ = !isEditing_;
		drawFunc_ = &KeyConfigScene::ChangeKeyPopUpText;
		changeKeyUpdateFunc_ = &KeyConfigScene::ChangeKeyborardUpdate;
		GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, 32, 800);
		return;
	}

	//ひとつ前のシーンに戻る
	if (input.IsTriggered(InputType::pause)) {
		updateFunc_ = &KeyConfigScene::FadeOutUpdate;
		configInput.RollbackChangedInputInfo();
		return;
	}

	//コントローラーの入力が入ったときに
	//コントローラー用の描画と更新用クラスに変更する
	if (!input.LastInputDevice()) {
		changeKeyUpdateFunc_ = &KeyConfigScene::ControllerUpdate;
		drawFunc_ = &KeyConfigScene::ControllerDraw;
	}

}

//変更したいキーをどのキーに変更するのか
void KeyConfigScene::ChangeKeyborardUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//一フレーム前の入力情報が残っていたらリターンする
	for (const auto& key : input.lastInput_) {
		if (key == true) return;
	}

	//短縮化
	auto& configInput = const_cast<InputState&>(input);

	//メンバ関数ポインタを変更するキーを選択する関数に変更する
	if (input.IsTriggered(InputType::pause)) {
		changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
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
	for (const auto& name : input.inputNameTable_) {
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
			changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
			drawFunc_ = &KeyConfigScene::KeyStateDraw;
			break;
		}
	}
	//パッドの入力を変更する
	if (padState != 0) {
		configInput.RewriteInputInfo(currentType, InputCategory::pad, padState);
		isEditing_ = !isEditing_;
		changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
		drawFunc_ = &KeyConfigScene::KeyStateDraw;
	}

}

void KeyConfigScene::ControllerUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();
	//短縮化
	auto& configInput = const_cast<InputState&>(input);

	//仮キー情報を消去してポーズシーンに戻る
	if (selectNum_ == 0) {
		if (input.IsTriggered(InputType::space)) {
			configInput.ResetInputInfo();
			updateFunc_ = &KeyConfigScene::FadeOutUpdate;
			return;
		}
	}

	//ひとつ前のシーンに戻る
	if (input.IsTriggered(InputType::pause)) {
		updateFunc_ = &KeyConfigScene::FadeOutUpdate;
		configInput.RollbackChangedInputInfo();
		return;
	}

	//キーボードの入力が入ったときに
	//キーボード用の描画と更新用クラスに変更する
	if (input.LastInputDevice()) {
		changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
		drawFunc_ = &KeyConfigScene::KeyStateDraw;
	}

}

void KeyConfigScene::FadeInUpdate()
{
	float timer = static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_);
	fadeValue_ = static_cast <int>(255 * timer);
	if (++fadeTimer_ == fadeInterval_) {
		updateFunc_ = &KeyConfigScene::NormalUpdate;
		fadeValue_ = 255;
	}
}

void KeyConfigScene::NormalUpdate()
{
	(this->*changeKeyUpdateFunc_)();
}

void KeyConfigScene::FadeOutUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<SettingScene>(manager_)));
		fadeValue_ = 0;
		return;
	}
}
