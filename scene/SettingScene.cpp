#include "SettingScene.h"
#include "SceneManager.h"
#include "ScenePause.h"
#include "KeyConfigScene.h"

#include "../util/game.h"
#include "../util/InputState.h"
#include "../util/SoundManager.h"
#include "../util/FontsManager.h"
#include "../util/UIItemManager.h"

#include <algorithm>

using namespace std;

namespace {
	//フォントの名前
	const char* const pigumo42_font_name = "ピグモ 0042";
	//円同士の間隔
	constexpr float circle_distance = 53.5f;
	//円の半径
	constexpr float circle_radius = 14.5f;
}

//コンストラクタ
SettingScene::SettingScene(SceneManager& manager):SceneBase(manager),updateFunc_(&SettingScene::GaussFadeInUpdate)
{
}

//デストラクタ
SettingScene::~SettingScene()
{
}

//初期化
void SettingScene::Init()
{
	//短縮化
	auto& sound = SoundManager::GetInstance();
	auto& font = FontsManager::GetInstance();

	//インスタンス化
	UIManager_ = std::make_shared<UIItemManager>();

	//現在のボリュームの取得
	volumeBGM_ = sound.GetBGMVolume() / 25;
	volumeSE_ = sound.GetSEVolume() / 25;

	//UI画像の作成
	//フォントの取得
	int pigumo42Font = font.GetFontHandle(pigumo42_font_name);
	//フォントを適用した文字列のサイズ取得
	int windowFontSize = font.GetStringSize("モード", pigumo42_font_name);
	int BGMFontSize = font.GetStringSize("BGM", pigumo42_font_name);
	int SEFontSize = font.GetStringSize("SE", pigumo42_font_name);

	//UI画像の作成
	UIManager_->AddMenu(Game::screen_width / 4, Game::screen_height / 3, 320, 100, "モード", pigumo42Font);
	UIManager_->AddMenu(Game::screen_width / 4, Game::screen_height / 2 - 20, 320, 100, "BGM", pigumo42Font);
	UIManager_->AddMenu(Game::screen_width / 4, Game::screen_height / 3 * 2 - 20, 320, 100, "SE", pigumo42Font);
	UIManager_->AddMenu(Game::screen_width / 2, Game::screen_height / 4 * 3, 320, 100, "操作設定", pigumo42Font);
	UIManager_->AddMenu(Game::screen_width / 2, Game::screen_height / 4 * 3 + 40, 320, 100, "戻る", pigumo42Font);

	makeScreenHandle_ = MakeScreen(Game::screen_width, Game::screen_height, true);

	if (manager_.GetWindowMode()) {
		windowModeText_ = "≪  ウィンドウモード  ≫";
	}
	else {
		windowModeText_ = "≪  フルスクリーン  ≫";
	}

}

//終了
void SettingScene::End()
{
	DeleteGraph(makeScreenHandle_);
}

//更新
void SettingScene::Update()
{
	(this->*updateFunc_)();
}

//描画
void SettingScene::Draw()
{
	//少し透過した黒の背景を描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//作成した画面サイズの画像に
	//以降の内容を書き込む
	SetDrawScreen(makeScreenHandle_);
	ClearDrawScreen();

	//UI文字列の描画
	UIManager_->AlphaChangeDraw(selectNum_,fadeValue_);

	//短縮化
	auto& font = FontsManager::GetInstance();

	//現在の画面モードを表示
	int pigumo42 = font.GetFontHandle(pigumo42_font_name);
	int windowModeFontSize = font.GetStringSize(windowModeText_.c_str(), pigumo42_font_name);

	int alpha[3];

	for (int i = 0; i < 3; i++) {
		if (selectNum_ == i) {
			alpha[i] = 250;
		}
		else {
			alpha[i] = 150;
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha[0]);
	DrawStringToHandle(Game::screen_width / 2 - windowModeFontSize / 2, Game::screen_height / 3, windowModeText_.c_str(), 0xffffff, pigumo42);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//円を描画する位置
	float circlePosX = Game::screen_width / 3 - circle_distance * 1.5f;

	//拡縮率
	float scale = 1.0f;

	//色
	int color = 0xb1b3b6;

	//BGMバー
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha[1]);

	//音量バー画像の描画
	for (int i = 1; i < 11; i++) {
		scale = 1.0f;
		color = 0xb1b3b6;

		if (volumeBGM_ == i) {
			scale = 1.3f;
			color = 0xff0000;
		}

		DrawCircleAA(circlePosX + i * circle_distance, Game::screen_height / 2, circle_radius * scale, 32, color, true);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//SEバー
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha[2]);

	//音量バー画像の描画
	for (int i = 1; i < 11; i++) {
		scale = 1.0f;
		color = 0xb1b3b6;

		if (volumeSE_ == i) {
			scale = 1.3f;
			color = 0xff0000;
		}

		DrawCircleAA(circlePosX + i * circle_distance, Game::screen_height / 3 * 2, circle_radius * scale, 32, color, true);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetDrawScreen(DX_SCREEN_BACK);

	//フェードの時、アルファ値を変更するとともにガウスぼかしを掛ける
	GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, 8, 255-fadeValue_);
	//描画画像のアルファ値を変更する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawGraph(0, 0, makeScreenHandle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//通常時の更新
void SettingScene::NormalUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();

	if (input.IsTriggered(InputType::Up)) {
		selectNum_ = (std::max)(selectNum_ - 1, 0);
	}
	else if (input.IsTriggered(InputType::Down)) {
		selectNum_ = (std::min)(selectNum_ + 1, 4);
	}

	ChangeUpdateFunc();

	//シーン切り替え
	if (input.IsTriggered(InputType::Pause)) {
		nextScene_ = std::make_shared<ScenePause>(manager_);
		updateFunc_ = &SettingScene::GaussFadeOutUpdate;
	}
}

//ガウス処理を施したフェードイン
void SettingScene::GaussFadeInUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		updateFunc_ = &SettingScene::NormalUpdate;
		fadeValue_ = 255;
	}
}

//ガウス処理を施したフェードアウト
void SettingScene::GaussFadeOutUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		manager_.SwapScene(std::shared_ptr<SceneBase>(nextScene_));
		fadeValue_ = 0;
		return;
	}
}

//BGMの音量を変更する
void SettingScene::BGMUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();
	auto& sound = SoundManager::GetInstance();

	//BGM音量調整
	if (input.IsTriggered(InputType::Left)) {
		volumeBGM_ = (max)(volumeBGM_ - 1, 1);
		sound.PlayBGM("checkSoundBGM");
	}
	if (input.IsTriggered(InputType::Right)) {
		volumeBGM_ = (min)(volumeBGM_ + 1, 10);
		sound.PlayBGM("checkSoundBGM");
	}

	//音量の変更
	SoundManager::GetInstance().SetBGMVolume(volumeBGM_ * 25);
}

//SEの音量を変更する
void SettingScene::SEUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();
	auto& sound = SoundManager::GetInstance();

	//SE音量調整
	if (input.IsTriggered(InputType::Left)) {
		volumeSE_ = (max)(volumeSE_ - 1, 1);
		sound.PlaySE("checkSoundSE");
	}
	if (input.IsTriggered(InputType::Right)) {
		volumeSE_ = (min)(volumeSE_ + 1, 10);
		sound.PlaySE("checkSoundSE");
	}

	//音量の変更
	SoundManager::GetInstance().SetSEVolume(volumeSE_ * 25);
}

//ウィンドウモードを変更する
void SettingScene::ChangeWindowUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();

	if (input.IsTriggered(InputType::Left)) {
		windowModeText_ = "≪  ウィンドウモード  ≫";
		manager_.ChangeWindowMode(true);
	}
	if (input.IsTriggered(InputType::Right)) {
		windowModeText_ = "≪  フルスクリーン  ≫";
		manager_.ChangeWindowMode(false);
	}
}

//updateFuncの中身を変更する
void SettingScene::ChangeUpdateFunc()
{
	//短縮化
	auto& input = InputState::GetInstance();

	switch (selectNum_) {
	case 0:
		ChangeWindowUpdate();
		break;
	case 1:
		BGMUpdate();
		break;
	case 2:
		SEUpdate();
		break;
	case 3:
		if (input.IsTriggered(InputType::Space)) {
			nextScene_ = std::make_shared<KeyConfigScene>(manager_);
			updateFunc_ = &SettingScene::GaussFadeOutUpdate;
		}
		break;
	case 4:
		if (input.IsTriggered(InputType::Space)) {
			nextScene_ = std::make_shared<ScenePause>(manager_);
			updateFunc_ = &SettingScene::GaussFadeOutUpdate;
		}
		break;
	}
}
