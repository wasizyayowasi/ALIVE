#include "SceneTitle.h"
#include "SceneManager.h"
#include "GameMain.h"
#include "TitlemenuScene.h"

#include "../util/game.h"
#include "../util/InputState.h"
#include "../util/FontsManager.h"
#include "DxLib.h"
#include<algorithm>



SceneTitle::SceneTitle(SceneManager& manager) : SceneBase(manager), updateFunc_(&SceneTitle::fadeInUpdate)
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	manager_.PushScene(std::shared_ptr<SceneBase>(std::make_shared<TitlemenuScene>(manager_)));
}

void SceneTitle::End()
{
}

void SceneTitle::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void SceneTitle::Draw()
{
	DrawString(0, 0, "SceneTitle", 0xffffff);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//âÊñ ëSëÃÇê^Ç¡çïÇ…ìhÇËÇ¬Ç‘Ç∑
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneTitle::fadeInUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		manager_.PushScene(std::shared_ptr<SceneBase>(std::make_shared<TitlemenuScene>(manager_)));
		updateFunc_ = &SceneTitle::normalUpdate;
		fadeValue_ = 0;
		return;
	}
}

void SceneTitle::normalUpdate(const InputState& input)
{		
	updateFunc_ = &SceneTitle::fadeOutUpdate;
}

void SceneTitle::fadeOutUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_)));
		return;
	}
}
