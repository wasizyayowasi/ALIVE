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
	volumeBGM_ = SoundManager::getInstance().getBGMVolume();
	volumeSE_ = SoundManager::getInstance().getSEVolume();
}

SoundSettingScene::~SoundSettingScene()
{
}

void SoundSettingScene::update(const InputState& input)
{
	if (input.isPressed(InputType::down)) {
		volumeBGM_ = (max)(volumeBGM_ - 1.0f, 0.0f);
	}
	if (input.isPressed(InputType::up)) {
		volumeBGM_ = (min)(volumeBGM_ + 1.0f, 255.0f);
	}
	if (input.isPressed(InputType::left)) {
		volumeSE_ = (max)(volumeSE_ - 1.0f, 0.0f);
	}
	if (input.isPressed(InputType::right)) {
		volumeSE_ = (min)(volumeSE_ + 1.0f, 255.0f);
	}

	SoundManager::getInstance().setBGMVolume(volumeBGM_);
	SoundManager::getInstance().setSEVolume(volumeSE_);

	if (input.isTriggered(InputType::prev)) {
		manager_.swapScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
	}

}

void SoundSettingScene::draw()
{

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawFormatString(0, 16, 0x448844, "%.2f", volumeBGM_);
	DrawFormatString(0, 32, 0x448844, "%.2f", volumeSE_);

	DrawFormatString(0, 48, 0x448844, "%d", static_cast<int>(volumeBGM_));
	DrawFormatString(0, 64, 0x448844, "%d", static_cast<int>(volumeSE_));

}
