#include "SettingSceneForSceneTitle.h"
#include "SceneManager.h"
#include "ScenePause.h"
#include "KeyConfigSceneForSceneTitle.h"

#include "../util/game.h"
#include "../util/InputState.h"
#include "../util/SoundManager.h"
#include "../util/FontsManager.h"
#include "../util/ExternalFile.h"
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

SettingSceneForSceneTitle::SettingSceneForSceneTitle(SceneManager& manager) :SceneBase(manager), updateFunc_(&SettingSceneForSceneTitle::GaussFadeInUpdate)
{
}

SettingSceneForSceneTitle::~SettingSceneForSceneTitle()
{
}

void SettingSceneForSceneTitle::Init()
{
	//短縮化
	auto& sound = SoundManager::GetInstance();
	auto& font = FontsManager::GetInstance();
	auto& file = ExternalFile::GetInstance();

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

	//描画座標
	menuDrawPos_["モード"]		= file.GetUIPos("windowModeUIPos");
	menuDrawPos_["BGM"]			= file.GetUIPos("BGMUIPos");
	menuDrawPos_["SE"]			= file.GetUIPos("SEUIPos");
	menuDrawPos_["操作設定"]	= file.GetUIPos("advancedSettingUIPos");
	menuDrawPos_["戻る"]		= file.GetUIPos("backUIPos");

	//UI画像の作成
	UIManager_->AddMenu(static_cast<float>(Game::screen_width / 4), static_cast<float>(Game::screen_height / 3),          320, 100, "モード",   pigumo42Font);
	UIManager_->AddMenu(static_cast<float>(Game::screen_width / 4), static_cast<float>(Game::screen_height / 2 - 20),     320, 100, "BGM",      pigumo42Font);
	UIManager_->AddMenu(static_cast<float>(Game::screen_width / 4), static_cast<float>(Game::screen_height / 3 * 2 - 20), 320, 100, "SE",       pigumo42Font);
	UIManager_->AddMenu(static_cast<float>(Game::screen_width / 2), static_cast<float>(Game::screen_height / 4 * 3),      320, 100, "操作設定", pigumo42Font);
	UIManager_->AddMenu(static_cast<float>(Game::screen_width / 2), static_cast<float>(Game::screen_height / 4 * 3 + 40), 320, 100, "戻る",     pigumo42Font);

	makeScreenHandle_ = MakeScreen(Game::screen_width, Game::screen_height, true);

	if (manager_.GetWindowMode()) {
		windowModeText_ = "≪  ウィンドウモード  ≫";
	}
	else {
		windowModeText_ = "≪  フルスクリーン  ≫";
	}

}

void SettingSceneForSceneTitle::End()
{
	DeleteGraph(makeScreenHandle_);
}

void SettingSceneForSceneTitle::Update()
{
	(this->*updateFunc_)();
}

void SettingSceneForSceneTitle::Draw()
{
	//UI文字列の描画
	UIManager_->AlphaChangeDrawBillBoard(menuDrawPos_, selectNum_,fadeValue_, 20.0f);

	//作成した画面サイズの画像に
	//以降の内容を書き込む
	SetDrawScreen(makeScreenHandle_);
	ClearDrawScreen();

	//短縮化
	auto& font = FontsManager::GetInstance();

	//現在の画面モードを表示
	int pigumo42 = font.GetFontHandle(pigumo42_font_name);
	float windowModeFontSize = static_cast<float>(font.GetStringSize(windowModeText_.c_str(), pigumo42_font_name));

	int alpha[3] = {};
	
	for (int i = 0; i < 3; i++) {
		if (selectNum_ == i) {
			alpha[i] = 250;
		}
		else {
			alpha[i] = 150;
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha[0]);
	DrawStringFToHandle(static_cast<float>(Game::screen_width / 2) - windowModeFontSize / 2.5f, Game::screen_height / 3.5f, windowModeText_.c_str(), 0xffffff, pigumo42);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//円を描画する位置
	float circlePosX = static_cast<float>(Game::screen_width / 3) - circle_distance / 2;

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

		DrawCircleAA(circlePosX + i * circle_distance, static_cast<float>(Game::screen_height / 2) - circle_distance / 2, circle_radius * scale, 32, color, true);
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

		DrawCircleAA(circlePosX + i * circle_distance, Game::screen_height / 3 * 2 - circle_distance, circle_radius * scale, 32, color, true);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetDrawScreen(DX_SCREEN_BACK);

	//フェードの時、アルファ値を変更するとともにガウスぼかしを掛ける
	GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, 8, 255 - fadeValue_);
	//描画画像のアルファ値を変更する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawGraph(0, 0, makeScreenHandle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SettingSceneForSceneTitle::NormalUpdate()
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
	if (input.IsTriggered(InputType::Activate)) {
		selectNum_ = 4;
		updateFunc_ = &SettingSceneForSceneTitle::GaussFadeOutUpdate;
	}
}

void SettingSceneForSceneTitle::GaussFadeInUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		updateFunc_ = &SettingSceneForSceneTitle::NormalUpdate;
		fadeValue_ = 255;
	}
}

void SettingSceneForSceneTitle::GaussFadeOutUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		SceneChange();
		fadeValue_ = 0;
		return;
	}
}

void SettingSceneForSceneTitle::BGMUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//BGM音量調整
	if (input.IsTriggered(InputType::Left)) {
		volumeBGM_ = (max)(volumeBGM_ - 1, 1);
	}
	if (input.IsTriggered(InputType::Right)) {
		volumeBGM_ = (min)(volumeBGM_ + 1, 10);
	}

	//音量の変更
	SoundManager::GetInstance().SetBGMVolume(volumeBGM_ * 25);
}

void SettingSceneForSceneTitle::SEUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//SE音量調整
	if (input.IsTriggered(InputType::Left)) {
		volumeSE_ = (max)(volumeSE_ - 1, 1);
	}
	if (input.IsTriggered(InputType::Right)) {
		volumeSE_ = (min)(volumeSE_ + 1, 10);
	}

	//音量の変更
	SoundManager::GetInstance().SetSEVolume(volumeSE_ * 25);
}

void SettingSceneForSceneTitle::ChangeWindowUpdate()
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

void SettingSceneForSceneTitle::ChangeUpdateFunc()
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
			updateFunc_ = &SettingSceneForSceneTitle::GaussFadeOutUpdate;
		}
		break;
	case 4:
		if (input.IsTriggered(InputType::Space)) {
			updateFunc_ = &SettingSceneForSceneTitle::GaussFadeOutUpdate;
		}
		break;
	}
}

void SettingSceneForSceneTitle::SceneChange()
{
	switch (selectNum_) {
	case 3:
		manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<KeyConfigSceneForSceneTitle>(manager_)));
		break;
	case 4:
		manager_.PopFrontScene();
		break;
	}
}
