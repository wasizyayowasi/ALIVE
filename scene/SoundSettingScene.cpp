#include "SoundSettingScene.h"
#include "SceneManager.h"
#include "ScenePause.h"
#include "../util/SoundManager.h"
#include "../util/InputState.h"
#include "../util/game.h"
#include <algorithm>

using namespace std;

SoundSettingScene::SoundSettingScene(SceneManager& manager):SceneBase(manager)
{
}

SoundSettingScene::~SoundSettingScene()
{
}

void SoundSettingScene::Init()
{
	barGraph_ = LoadGraph("data/graph/bar1.png");
	pictogramGraph_ = LoadGraph("data/graph/pict.png");

	volumeBGM_ = SoundManager::GetInstance().GetBGMVolume();
	volumeSE_ = SoundManager::GetInstance().GetSEVolume();
}

void SoundSettingScene::End()
{
	DeleteGraph(barGraph_);
	DeleteGraph(pictogramGraph_);
}

void SoundSettingScene::Update(const InputState& input)
{
	//仮サウンドの音量調整
	if (input.IsTriggered(InputType::down)) {
		volumeBGM_ = (max)(volumeBGM_ - 25, 0);
	}
	if (input.IsTriggered(InputType::up)) {
		volumeBGM_ = (min)(volumeBGM_ + 25, 250);
	}
	if (input.IsTriggered(InputType::left)) {
		volumeSE_ = (max)(volumeSE_ - 25, 0);
	}
	if (input.IsTriggered(InputType::right)) {
		volumeSE_ = (min)(volumeSE_ + 25, 250);
	}

	//音量の変更
	SoundManager::GetInstance().SetBGMVolume(volumeBGM_);
	SoundManager::GetInstance().SetSEVolume(volumeSE_);

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

	DrawFormatString(Game::screen_width / 3, Game::screen_height / 3, 0xffffff, "%s", "BGM");
	DrawFormatString(Game::screen_width / 3, Game::screen_height / 3 * 2, 0xffffff, "%s", "SE");

	int BGMpictPos = static_cast<int>((static_cast<float>(volumeBGM_) / 250.0f) * 10);
	int SEpictPos = static_cast<int>((static_cast<float>(volumeSE_) / 250.0f) * 10);

	//音量バー画像
	
	DrawRotaGraph(Game::screen_width / 5 * 3, Game::screen_height / 3, 0.8f, 0.0f, barGraph_, true);
	DrawRotaGraph(Game::screen_width / 5 * 3, Game::screen_height / 3 * 2, 0.8f, 0.0f, barGraph_, true);
	
	//ピクトグラム
	DrawRotaGraph(Game::screen_width / 5 * 2 + BGMpictPos * 52, Game::screen_height / 3, 0.3f, 0.0f, pictogramGraph_, true);
	DrawRotaGraph(Game::screen_width / 5 * 2 + SEpictPos * 52, Game::screen_height / 3 * 2, 0.3f, 0.0f, pictogramGraph_, true);

	DrawFormatString(0, 16, 0x00ff00, "%.2f", volumeBGM_);
	DrawFormatString(0, 32, 0x00ff00, "%.2f", volumeSE_);

	DrawFormatString(0, 48, 0x00ff00, "%d", static_cast<int>(volumeBGM_));
	DrawFormatString(0, 64, 0x00ff00, "%d", static_cast<int>(volumeSE_));

}
