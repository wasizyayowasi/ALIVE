#include "SoundSettingScene.h"
#include "SceneManager.h"
#include "ScenePause.h"
#include "../util/SoundManager.h"
#include "../util/InputState.h"
#include "../util/game.h"
#include "../util/FontsManager.h"
#include "../util/UIItemManager.h"
#include <algorithm>

using namespace std;

namespace {
	//フォントの名前
	const char* const font_name = "ピグモ 0042";
	//移動距離
	constexpr float pictgram_move_distance = 58.4f;
}

SoundSettingScene::SoundSettingScene(SceneManager& manager):SceneBase(manager),updateFunc_(&SoundSettingScene::BGMUpdate)
{
}

SoundSettingScene::~SoundSettingScene()
{
}

void SoundSettingScene::Init()
{
	//画像の読み込み
	BGMBarHandle_ = LoadGraph("data/graph/bar.png");
	SEBarHandle_ = LoadGraph("data/graph/bar.png");
	pictogramGraph_ = LoadGraph("data/graph/pict.png");

	GetGraphSize(BGMBarHandle_, &barHandleWidth_, &barHandleHeight_);

	barHandleWidth_ = barHandleWidth_ * 0.8f;
	barHandleHeight_ = barHandleHeight_ * 0.8f;

	//現在のボリュームの取得
	volumeBGM_ = SoundManager::GetInstance().GetBGMVolume();
	volumeSE_ = SoundManager::GetInstance().GetSEVolume();

	UIManager_ = std::make_shared<UIItemManager>();

	int BGMPictPos = static_cast<int>((static_cast<float>(volumeBGM_) / 250.0f) * 10);
	int SEPictPos = static_cast<int>((static_cast<float>(volumeSE_) / 250.0f) * 10);

	BGMPictogramPosX_ = Game::screen_width / 4 + pictgram_move_distance + BGMPictPos * pictgram_move_distance;
	SEPictogramPosX_ = Game::screen_width / 4 + pictgram_move_distance + SEPictPos * pictgram_move_distance;

	DrawFormatString(Game::screen_width / 4, Game::screen_height / 3, 0xffffff, "%s", "BGM");
	DrawFormatString(Game::screen_width / 4, Game::screen_height / 3 * 2, 0xffffff, "%s", "SE");

	//UI画像の作成
	//フォントの取得
	int font = FontsManager::getInstance().GetFontHandle(font_name);
	//フォントを適用した文字列のサイズ取得
	int BGMFontSize = FontsManager::getInstance().GetStringSize("BGM", font_name);
	int SEFontSize = FontsManager::getInstance().GetStringSize("SE", font_name);
	//UI画像の作成
	UIManager_->AddMenu(Game::screen_width / 4 - BGMFontSize / 2, Game::screen_height / 2 - 20, 320, 100, "BGM", font);
	UIManager_->AddMenu(Game::screen_width / 4 - SEFontSize / 2, Game::screen_height / 3 * 2 - 20, 320, 100, "SE", font);

}

void SoundSettingScene::End()
{
	DeleteGraph(BGMBarHandle_);
	DeleteGraph(SEBarHandle_);
	DeleteGraph(pictogramGraph_);
}

void SoundSettingScene::Update(const InputState& input)
{
	
	if (input.IsTriggered(InputType::up)) {
		selectNum_ = (std::max)(selectNum_ - 1, 0);
	}
	else if (input.IsTriggered(InputType::down)) {
		selectNum_ = (std::min)(selectNum_ + 1, 2);
	}

	ChangeUpdateFunc();

	(this->*updateFunc_)(input);

	//シーン切り替え
	if (input.IsTriggered(InputType::pause)) {
		manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
	}

}

void SoundSettingScene::Draw()
{

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	int BGMPictPos = static_cast<int>((static_cast<float>(volumeBGM_) / 250.0f) * 10);
	int SEPictPos = static_cast<int>((static_cast<float>(volumeSE_) / 250.0f) * 10);

	UIManager_->Draw(selectNum_);

	//音量バー画像
	DrawRotaGraph(Game::screen_width / 2, Game::screen_height / 2, 0.8f, 0.0f, BGMBarHandle_, true);
	DrawRotaGraph(Game::screen_width / 2, Game::screen_height / 3 * 2, 0.8f, 0.0f, SEBarHandle_, true);

	time_++;

	MovePictogram(BGMPictPos, BGMPictogramPosX_,BGMPictogramRot_, BGMPictogramTransInversion_);
	MovePictogram(SEPictPos, SEPictogramPosX_,SEPictogramRot_, SEPictogramTransInversion_);

	//ピクトグラム
	DrawRotaGraph(BGMPictogramPosX_, Game::screen_height / 2, 0.3f, BGMPictogramRot_, pictogramGraph_, true, BGMPictogramTransInversion_);
	DrawRotaGraph(SEPictogramPosX_, Game::screen_height / 3 * 2, 0.3f, SEPictogramRot_, pictogramGraph_, true, SEPictogramTransInversion_);

	DrawFormatString(0, 16, 0x00ff00, "%.2f", volumeBGM_);
	DrawFormatString(0, 32, 0x00ff00, "%.2f", volumeSE_);

	DrawFormatString(0, 48, 0x00ff00, "%d", static_cast<int>(volumeBGM_));
	DrawFormatString(0, 64, 0x00ff00, "%d", static_cast<int>(volumeSE_));

}

void SoundSettingScene::BGMUpdate(const InputState& input)
{

	//BGM音量調整
	if (input.IsTriggered(InputType::left)) {
		volumeBGM_ = (max)(volumeBGM_ - 25, 0);
	}
	if (input.IsTriggered(InputType::right)) {
		volumeBGM_ = (min)(volumeBGM_ + 25, 250);
	}

	int BGMPictPos = static_cast<int>((static_cast<float>(volumeBGM_) / 250.0f) * 10);

	GraphFilterRectBlt(BGMBarHandle_, BGMBarHandle_, 0, 0, BGMPictPos * 73, barHandleHeight_, 0, 0, DX_GRAPH_FILTER_HSB, 0, 100, 100, 255);
//	GraphFilterRectBlt(barHandle_, barHandle_, BGMPictPos * 73, 0, barHandleWidth_, barHandleHeight_, 0, 0, DX_GRAPH_FILTER_HSB, 0, 0, 0, 0);

	//音量の変更
	SoundManager::GetInstance().SetBGMVolume(volumeBGM_);
	
}

void SoundSettingScene::SEUpdate(const InputState& input)
{
	//SE音量調整
	if (input.IsTriggered(InputType::left)) {
		volumeSE_ = (max)(volumeSE_ - 25, 0);
	}
	if (input.IsTriggered(InputType::right)) {
		volumeSE_ = (min)(volumeSE_ + 25, 250);
	}

	int SEPictPos = static_cast<int>((static_cast<float>(volumeSE_) / 250.0f) * 10);
	GraphFilterRectBlt(SEBarHandle_, SEBarHandle_, 0, 0, SEPictPos * 73, barHandleHeight_, 0, 0, DX_GRAPH_FILTER_HSB, 0, 100, 100, 255);

	//音量の変更
	SoundManager::GetInstance().SetSEVolume(volumeSE_);

}

void SoundSettingScene::ChangeWindowUpdate(const InputState& input)
{
	if (input.IsTriggered(InputType::left)) {
		ChangeWindowMode(true);
	}
	if (input.IsTriggered(InputType::right)) {
		ChangeWindowMode(false);
	}
}

void SoundSettingScene::MovePictogram(int pictPos, float& pos, float& rot, bool& inversion)
{
	float targetPos = Game::screen_width / 4 + pictgram_move_distance + pictPos * pictgram_move_distance;
	float distance = targetPos - pos;

	if (distance > 0) {
		pos = (std::min)(pos + 1.0f, targetPos);
		inversion = false;
		if (time_ % 8 == 0) {
			rot = -rot;
		}
	}
	else if(distance < 0) {
		pos = (std::max)(pos - 1.0f, targetPos);
		inversion = true;
		if (time_ % 8 == 0) {
			rot = -rot;
		}
	}

	if (time_ > 60) {
		time_ = 0;
	}

}

void SoundSettingScene::ChangeUpdateFunc()
{
	switch (selectNum_) {
	case 0:
		updateFunc_ = &SoundSettingScene::BGMUpdate;
		break;
	case 1:
		updateFunc_ = &SoundSettingScene::SEUpdate;
		break;
	case 2:
//		updateFunc_ = &SoundSettingScene::ChangeWindowUpdate;
		break;
	}
}
