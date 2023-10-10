#include "GameEnd.h"
#include "SceneManager.h"
#include "SceneTitle.h"

#include "../util/game.h"
#include "../util/InputState.h"
#include "DxLib.h"

GameEnd::GameEnd(SceneManager& manager) : SceneBase(manager),updateFunc_(&GameEnd::fadeInUpdate)
{
}

GameEnd::~GameEnd()
{
}

void GameEnd::Init()
{
}

void GameEnd::End()
{
}

void GameEnd::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void GameEnd::Draw()
{
	DrawString(0, 0, "GameEnd", 0xffffff);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//画面全体を真っ黒に塗りつぶす
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameEnd::fadeInUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &GameEnd::normalUpdate;
		fadeValue_ = 0;
	}
}

void GameEnd::normalUpdate(const InputState& input)
{
	if (input.IsTriggered(InputType::space)) {
		updateFunc_ = &GameEnd::fadeOutUpdate;
	}
}

void GameEnd::fadeOutUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
		return;
	}
}
