#include "KeyConfigScene.h"
#include "SceneManager.h"
#include "SettingScene.h"
#include "PopUpTextScene.h"

#include "../util/game.h"
#include "../util/InputState.h"
#include "../util/FontsManager.h"
#include "../util/GraphManager.h"
#include "../util/DrawFunctions.h"
#include "../util/UIItemManager.h"

#include <DxLib.h>
#include <algorithm>
#include <string>

namespace {
	//key画像の一枚当たりのサイズ
	constexpr float graph_chip_size = 59.0f;
	//画像の隙間サイズ
	constexpr float graph_gap_size = 10.0f;
}

//コンストラクタ
KeyConfigScene::KeyConfigScene(SceneManager& manager):SceneBase(manager),
updateFunc_(&KeyConfigScene::FadeInUpdate)
{
	//短縮化
	auto& input = InputState::GetInstance();

	//最後に入力された装置によって更新、描画関数を変更する
	if (!input.LastInputDevice()) {
		changeKeyUpdateFunc_ = &KeyConfigScene::ControllerUpdate;
		drawFunc_ = &KeyConfigScene::ControllerDraw;
	}
	else {
		changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
		drawFunc_ = &KeyConfigScene::KeyStateDraw;
	}
}

//デストラクタ
KeyConfigScene::~KeyConfigScene()
{
}

//初期化
void KeyConfigScene::Init()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//インスタンス化
	KeyUI_ = std::make_shared<UIItemManager>();
	PadUI_ = std::make_shared<UIItemManager>();

	//スクリーンサイズのハンドルを作成
	makeScreenHandle_ = MakeScreen(Game::screen_width, Game::screen_height, true);

	//フォントの作成
	fontHandleSize21_ = FontsManager::GetInstance().GetFontHandle("ピグモ 0021");
	fontHandleSize42_ = FontsManager::GetInstance().GetFontHandle("ピグモ 0042");
	
	//キーの役割の一つ目の座標
	float namePosX = static_cast<float>(Game::screen_width / 4);
	float namePosY = static_cast<float>(Game::screen_height / 2) - (graph_chip_size * 4 + 45.0f);

	int nameNo = 0;
	for (auto& table : input.inputNameTable_) {
		//メニューの追加
		KeyUI_->AddMenu(namePosX, namePosY, 320, 100, table.second.c_str(), fontHandleSize21_);

		//ポジション調整
		namePosY += graph_chip_size + graph_gap_size;

		//inputstate.tempmaptableの半分を超えたら折り返す
		if (nameNo == input.tempMapTable_.size() / 2 - 1) {
			namePosY = static_cast<float>(static_cast<float>(Game::screen_height / 2) - (graph_chip_size * 4 + 30.0f));
			namePosX += static_cast<float>(Game::screen_width / 4) * 1.5f;
		}

		nameNo++;
	}

	//メニューの追加
	KeyUI_->AddMenu(static_cast<float>(Game::screen_width / 2), static_cast<float>(Game::screen_height / 5 * 4), 320, 100, "変更", fontHandleSize42_);
	KeyUI_->AddMenu(static_cast<float>(Game::screen_width / 2), static_cast<float>(Game::screen_height / 5 * 4 + 32), 320, 100, "キャンセル", fontHandleSize42_);
	PadUI_->AddMenu(static_cast<float>(Game::screen_width / 2), static_cast<float>(Game::screen_height / 5 * 4 + 32), 320, 100, "キャンセル", fontHandleSize42_);
}

//終了
void KeyConfigScene::End()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//現在のキー入力情報を外部データとして書き出す
	input.SavekeyInfo();
	DeleteGraph(makeScreenHandle_);
}

//更新
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
}

//キーの役割りの描画
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

	//書き込みスクリーンを裏のスクリーンに変更する
	SetDrawScreen(DX_SCREEN_BACK);
}

//キーの画像を描画する
void KeyConfigScene::KeyGraphDraw()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//ポジション設定
	float graphPosX = static_cast<float>(Game::screen_width) / 4 * 1.5f;
	float graphPosY = static_cast<float>(Game::screen_height / 2) - (graph_chip_size * 4 + 30.0f);

	//for文で何番目かを取得する
	int keyCount = 0;

	//明るさを引く値
	int subBrightness = 180;

	//画像の拡縮率
	float graphScale = 1.0f;

	for (auto& key : input.tempMapTable_) {
		
		if (key.first == InputType::Creative) {
			continue;
		}

		//現在カーソルがあっている場合
		//サイズと明るさを引く値を変更する
		if (keyCount == selectNum_) {
			graphScale = 1.2f;
			subBrightness = 0;
		}
		else {
			graphScale = 1.0f;
			subBrightness = 180;
		}

		//keyTypeの描画
		input.DrawKeyGraph(static_cast<int>(key.first), graphPosX, graphPosY, graphScale);

		//暗くした画像を画像の上に乗せる
		SetDrawBlendMode(DX_BLENDMODE_SUB, subBrightness);
		DrawBoxAA(graphPosX - 80.0f, graphPosY - graph_chip_size / 2 - graph_gap_size / 2, graphPosX - 240.0f + static_cast<float>(Game::screen_width / 4), graphPosY + graph_chip_size / 2 + graph_gap_size / 2, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//半分を超えたら折り返す処理
		graphPosY += graph_chip_size + 10;
		if (keyCount == input.tempMapTable_.size() / 2 - 1) {
			graphPosY = static_cast<float>(Game::screen_height / 2) - (graph_chip_size * 4 + 30.0f);
			graphPosX += static_cast<float>(Game::screen_width / 4) * 1.5f;
		}

		keyCount++;
	}
}

//コントローラー画像の描画
void KeyConfigScene::ControllerDraw()
{
	//書き込みスクリーンの変更
	SetDrawScreen(makeScreenHandle_);

	//スクリーンのクリア
	ClearDrawScreen();

	//UIの描画
	PadUI_->AlphaChangeDraw(selectNum_, fadeValue_);

	//操作説明画像の描画
	DrawGraph(0, 0, GraphManager::GetInstance().GetGraph("controller"), true);

	//書き込みスクリーンを裏のスクリーンに変更する
	SetDrawScreen(DX_SCREEN_BACK);
}

//変更したいkeyを入力させるためのポップアップ描画
void KeyConfigScene::ChangeKeyPopUpText()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//背景の黒描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(Game::screen_width / 6, Game::screen_height / 5, Game::screen_width / 6 * 5, Game::screen_height / 5 * 4, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	//文字列
	std::string text = input.inputNameTable_.find(static_cast<InputType>(selectNum_))->second;

	//文字列の横幅
	int strWidth = GetDrawStringWidthToHandle(text.c_str(), static_cast<int>(strlen(text.c_str())), fontHandleSize21_);

	//選択したキーの名前を出力
	DrawStringToHandle(Game::screen_width / 6 + strWidth / 2 , Game::screen_height / 5 - static_cast<int>(graph_chip_size / 2 - 8), text.c_str(), 0xffffff, fontHandleSize21_);

	//int型にキャストしたgraphChipSize
	int castGraphChipSize = static_cast<int>(graph_chip_size);

	//キー画像の描画
	input.DrawKeyGraph( selectNum_, static_cast<float>(Game::screen_width / 6) + strWidth * 2.5f,
						static_cast<float>(Game::screen_height / 5) - graph_chip_size / 2,
						1.0f);

	//文字列
	text = "変更したいキーを入力してください";
	//文字列の横幅
	strWidth = GetDrawStringWidthToHandle(text.c_str(), static_cast<int>(strlen(text.c_str())), fontHandleSize42_);
	//文字列の描画
	DrawStringToHandle(Game::screen_width / 2 - strWidth / 2, Game::screen_height / 2, text.c_str(), 0xffffff, fontHandleSize42_);

}

//どの入力装置のキーを変更するかを選択する
void KeyConfigScene::SelectChangeKeyUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//現在のキーの数を取得する
	const int keyNum = static_cast<int>(input.inputNameTable_.size() + 2);

	//選択操作
	if (input.IsTriggered(InputType::Up)) {
		if (selectNum_ == (keyNum - 1) / 2) {
			selectNum_ += keyNum / 2;
		}
		else {
			selectNum_ = ((selectNum_ - 1) + keyNum) % keyNum;
		}
	}
	else if (input.IsTriggered(InputType::Down)) {
		if (selectNum_ + 1 == (keyNum - 1) / 2) {
			selectNum_ += keyNum / 2;
		}
		else {
			selectNum_ = (selectNum_ + 1) % keyNum;
		}
	}
	else if (input.IsTriggered(InputType::Left) || input.IsTriggered(InputType::Right)) {
		if (selectNum_ < keyNum - 2) {
			selectNum_ = (selectNum_ + 7) % (keyNum - 1);
		}
	}

	//キーの変更を保存する
	if (selectNum_ == input.inputNameTable_.size()) {
		if (input.IsTriggered(InputType::Space)) {

			//仮のキー情報を実際に参照しているキー情報に代入する
			input.CommitChangedInputInfo();

			//シーンをポップアップテキストを描画するシーンに変更する
			manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<PopUpTextScene>(manager_)));		
			return;
		}
	}

	//仮キー情報を消去してポーズシーンに戻る
	if (selectNum_ == input.inputNameTable_.size() + 1) {
		if (input.IsTriggered(InputType::Space)) {
			input.ResetInputInfo();
			updateFunc_ = &KeyConfigScene::FadeOutUpdate;
			return;
		}
	}
	
	//どのキーを変更するかを仮決定
	if (input.IsTriggered(InputType::Space)) {
		isEditing_ = !isEditing_;
		drawFunc_ = &KeyConfigScene::ChangeKeyPopUpText;
		changeKeyUpdateFunc_ = &KeyConfigScene::ChangeKeyborardUpdate;
		GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, 32, 800);
		return;
	}

	//ひとつ前のシーンに戻る
	if (input.IsTriggered(InputType::Pause)) {
		updateFunc_ = &KeyConfigScene::FadeOutUpdate;
		input.RollbackChangedInputInfo();
		return;
	}

	//コントローラーの入力が入ったときに
	//コントローラー用の描画と更新用クラスに変更する
	if (!input.LastInputDevice()) {
		changeKeyUpdateFunc_ = &KeyConfigScene::ControllerUpdate;
		drawFunc_ = &KeyConfigScene::ControllerDraw;
	}

}

//変更するキーをどのキーに変更するのかを決定する
void KeyConfigScene::ChangeKeyborardUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//キーボードとパッドの入力を得る
	char keyState[256];
	GetHitKeyStateAll(keyState);

	int idx = 0;
	InputType currentType = InputType::Max;

	//一フレーム前の入力情報が残っていたらリターンする
	for (const auto& key : input.lastInput_) {
		if (key == true) return;
	}

	//メンバ関数ポインタを変更するキーを選択する関数に変更する
	if (input.IsTriggered(InputType::Pause)) {
		drawFunc_ = &KeyConfigScene::KeyStateDraw;
		changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
		isEditing_ = !isEditing_;
		input.UndoSelectKey(static_cast<InputType>(selectNum_), InputCategory::keybd);
		return;
	}

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
			drawFunc_ = &KeyConfigScene::KeyStateDraw;
			changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
			isEditing_ = !isEditing_;
			input.RewriteInputInfo(currentType, InputCategory::keybd, i);
			break;
		}
	}
}

//コントローラーの場合の更新
void KeyConfigScene::ControllerUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//仮キー情報を消去してポーズシーンに戻る
	if (selectNum_ == 0) {
		if (input.IsTriggered(InputType::Space)) {
			input.ResetInputInfo();
			updateFunc_ = &KeyConfigScene::FadeOutUpdate;
			return;
		}
	}

	//ひとつ前のシーンに戻る
	if (input.IsTriggered(InputType::Pause)) {
		updateFunc_ = &KeyConfigScene::FadeOutUpdate;
		input.RollbackChangedInputInfo();
		return;
	}

	//キーボードの入力が入ったときに
	//キーボード用の描画と更新用クラスに変更する
	if (input.LastInputDevice()) {
		changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
		drawFunc_ = &KeyConfigScene::KeyStateDraw;
	}

}

//フェードインの更新
void KeyConfigScene::FadeInUpdate()
{
	float timer = static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_);
	fadeValue_ = static_cast <int>(255 * timer);
	if (++fadeTimer_ == fadeInterval_) {
		updateFunc_ = &KeyConfigScene::NormalUpdate;
		fadeValue_ = 255;
	}
}

//通常時の更新
void KeyConfigScene::NormalUpdate()
{
	(this->*changeKeyUpdateFunc_)();
}

//フェードアウトの更新
void KeyConfigScene::FadeOutUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<SettingScene>(manager_)));
		fadeValue_ = 0;
		return;
	}
}
