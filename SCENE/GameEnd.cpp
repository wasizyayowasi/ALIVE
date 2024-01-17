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

void GameEnd::Update()
{
	(this->*updateFunc_)();
}

void GameEnd::Draw()
{
	DrawString(0, 0, "GameEnd", 0xffffff);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//‰æ–Ê‘S‘Ì‚ğ^‚Á•‚É“h‚è‚Â‚Ô‚·
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameEnd::fadeInUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &GameEnd::normalUpdate;
		fadeValue_ = 0;
	}
}

void GameEnd::normalUpdate()
{
	//’Zk‰»
	auto& input = InputState::GetInstance();

	if (input.IsTriggered(InputType::space)) {
		updateFunc_ = &GameEnd::fadeOutUpdate;
	}
}

void GameEnd::fadeOutUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
		return;
	}
}
