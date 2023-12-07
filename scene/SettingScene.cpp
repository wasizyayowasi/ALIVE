#include "SettingScene.h"
#include "SceneManager.h"
#include "ScenePause.h"
#include "KeyConfigScene.h"

#include "../util/SoundManager.h"
#include "../util/InputState.h"
#include "../util/game.h"
#include "../util/FontsManager.h"
#include "../util/UIItemManager.h"

#include <algorithm>

using namespace std;

namespace {
	//フォントの名前
	const char* const pigumo42_font_name = "ピグモ 0042";
	//画像ファイルパス
	const char* const bar_graph_path = "data/graph/circleWhite.png";
	//移動距離
	constexpr float pictgram_move_distance = 58.4f;

	constexpr float move_speed = 3.0f;
}

SettingScene::SettingScene(SceneManager& manager):SceneBase(manager),updateFunc_(&SettingScene::GaussFadeInUpdate)
{
}

SettingScene::~SettingScene()
{
}

void SettingScene::Init()
{
	//画像の読み込み
	circleWhiteHandle_ = LoadGraph(bar_graph_path);
	circleGrayHandle_ = LoadGraph(bar_graph_path);
	pictogramGraph_ = LoadGraph("data/graph/pict.png");

	//barHandleの画像サイズを取得
	GetGraphSize(circleWhiteHandle_, &circleWhiteHandleWidth_, &circleWhiteHandleHeight_);

	//現在のボリュームの取得
	volumeBGM_ = SoundManager::GetInstance().GetBGMVolume();
	volumeSE_ = SoundManager::GetInstance().GetSEVolume();

	int BGMPictPos = static_cast<int>((static_cast<float>(volumeBGM_) / 250.0f) * 10);
	int SEPictPos = static_cast<int>((static_cast<float>(volumeSE_) / 250.0f) * 10);

	BGMPictogramPosX_ = Game::screen_width / 4 + pictgram_move_distance + BGMPictPos * pictgram_move_distance;
	SEPictogramPosX_ = Game::screen_width / 4 + pictgram_move_distance + SEPictPos * pictgram_move_distance;

	UIManager_ = std::make_shared<UIItemManager>();

	//UI画像の作成
	//フォントの取得
	int pigumo42Font = FontsManager::getInstance().GetFontHandle(pigumo42_font_name);
	//フォントを適用した文字列のサイズ取得
	int windowFontSize = FontsManager::getInstance().GetStringSize("モード", pigumo42_font_name);
	int BGMFontSize = FontsManager::getInstance().GetStringSize("BGM", pigumo42_font_name);
	int SEFontSize = FontsManager::getInstance().GetStringSize("SE", pigumo42_font_name);

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

void SettingScene::End()
{
	DeleteGraph(circleWhiteHandle_);
	DeleteGraph(circleGrayHandle_);
	DeleteGraph(pictogramGraph_);
	DeleteGraph(makeScreenHandle_);
}

void SettingScene::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void SettingScene::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetDrawScreen(makeScreenHandle_);
	ClearDrawScreen();

	int BGMPictPos = static_cast<int>((static_cast<float>(volumeBGM_) / 250.0f) * 10);
	int SEPictPos = static_cast<int>((static_cast<float>(volumeSE_) / 250.0f) * 10);

	UIManager_->AlphaChangeDraw(selectNum_,fadeValue_);

	//音量バー画像
	for (int i = 0; i < 10; i++) {
		//DrawGraph(i * 72, 360, circleGrayHandle_, true);
		DrawRotaGraph(365 + i * 29, 360, 1.0, 0, circleGrayHandle_, true, false);
	}


	time_++;

	MovePictogram(BGMPictPos, BGMPictogramPosX_,BGMPictogramRot_, BGMPictogramTransInversion_);
	MovePictogram(SEPictPos, SEPictogramPosX_,SEPictogramRot_, SEPictogramTransInversion_);

	//ピクトグラム
	DrawRotaGraph(BGMPictogramPosX_, Game::screen_height / 2, 0.3f, BGMPictogramRot_, pictogramGraph_, true, BGMPictogramTransInversion_);
	DrawRotaGraph(SEPictogramPosX_, Game::screen_height / 3 * 2, 0.3f, SEPictogramRot_, pictogramGraph_, true, SEPictogramTransInversion_);

	//現在の画面モードを表示
	int pigumo42 = FontsManager::getInstance().GetFontHandle(pigumo42_font_name);
	int windowModeFontSize = FontsManager::getInstance().GetStringSize(windowModeText_.c_str(), pigumo42_font_name);

	int alpha = 150;

	if (selectNum_ == 0) {
		alpha = 250;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawStringFToHandle(Game::screen_width / 2 - windowModeFontSize / 2, Game::screen_height / 3, windowModeText_.c_str(), 0xffffff, pigumo42);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawLine(Game::screen_width / 2, 0, Game::screen_width / 2, Game::screen_height, 0xff0000);

	SetDrawScreen(DX_SCREEN_BACK);

	GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, 8, 255-fadeValue_);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawGraph(0, 0, makeScreenHandle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void SettingScene::NormalUpdate(const InputState& input)
{

	if (input.IsTriggered(InputType::up)) {
		selectNum_ = (std::max)(selectNum_ - 1, 0);
	}
	else if (input.IsTriggered(InputType::down)) {
		selectNum_ = (std::min)(selectNum_ + 1, 4);
	}

	ChangeUpdateFunc(input);

	//シーン切り替え
	if (input.IsTriggered(InputType::pause)) {
		nextScene_ = std::make_shared<ScenePause>(manager_);
		updateFunc_ = &SettingScene::GaussFadeOutUpdate;
	}
}

void SettingScene::GaussFadeInUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		updateFunc_ = &SettingScene::NormalUpdate;
		fadeValue_ = 255;
	}
}

void SettingScene::GaussFadeOutUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		manager_.SwapScene(std::shared_ptr<SceneBase>(nextScene_));
		fadeValue_ = 0;
		return;
	}
}

void SettingScene::BGMUpdate(const InputState& input)
{

	//BGM音量調整
	if (input.IsTriggered(InputType::left)) {
		volumeBGM_ = (max)(volumeBGM_ - 25, 0);
	}
	if (input.IsTriggered(InputType::right)) {
		volumeBGM_ = (min)(volumeBGM_ + 25, 250);
	}

	int BGMPictPos = static_cast<int>((static_cast<float>(volumeBGM_) / 250.0f) * 10);

	//音量の変更
	SoundManager::GetInstance().SetBGMVolume(volumeBGM_);
	
}

void SettingScene::SEUpdate(const InputState& input)
{
	//SE音量調整
	if (input.IsTriggered(InputType::left)) {
		volumeSE_ = (max)(volumeSE_ - 25, 0);
	}
	if (input.IsTriggered(InputType::right)) {
		volumeSE_ = (min)(volumeSE_ + 25, 250);
	}

	int SEPictPos = static_cast<int>((static_cast<float>(volumeSE_) / 250.0f) * 10);

	//音量の変更
	SoundManager::GetInstance().SetSEVolume(volumeSE_);

}

void SettingScene::ChangeWindowUpdate(const InputState& input)
{
	if (input.IsTriggered(InputType::left)) {
		windowModeText_ = "≪  ウィンドウモード  ≫";
		manager_.ChangeWindowMode(true);
	}
	if (input.IsTriggered(InputType::right)) {
		windowModeText_ = "≪  フルスクリーン  ≫";
		manager_.ChangeWindowMode(false);
	}
}

void SettingScene::MovePictogram(int pictPos, float& pos, float& rot, bool& inversion)
{
	float targetPos = Game::screen_width / 3 + pictgram_move_distance + pictPos * pictgram_move_distance;
	float distance = targetPos - pos;

	if (distance > 0) {
		pos = (std::min)(pos + move_speed, targetPos);
		inversion = false;
		if (time_ % 4 == 0) {
			rot = -rot;
		}
	}
	else if(distance < 0) {
		pos = (std::max)(pos - move_speed, targetPos);
		inversion = true;
		if (time_ % 4 == 0) {
			rot = -rot;
		}
	}

	if (time_ > 60) {
		time_ = 0;
	}

}

void SettingScene::ChangeUpdateFunc(const InputState& input)
{
	switch (selectNum_) {
	case 0:
		ChangeWindowUpdate(input);
		break;
	case 1:
		BGMUpdate(input);
		break;
	case 2:
		SEUpdate(input);
		break;
	case 3:
		if (input.IsTriggered(InputType::space)) {
			nextScene_ = std::make_shared<KeyConfigScene>(manager_, input);
			updateFunc_ = &SettingScene::GaussFadeOutUpdate;
		}
		break;
	case 4:
		if (input.IsTriggered(InputType::space)) {
			nextScene_ = std::make_shared<ScenePause>(manager_);
			updateFunc_ = &SettingScene::GaussFadeOutUpdate;
		}
		break;
	}
}
