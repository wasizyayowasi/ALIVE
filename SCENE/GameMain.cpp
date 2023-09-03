#include "GameMain.h"
#include "SceneManager.h"
#include "GameEnd.h"

#include "../OBJECT/Player.h"
#include "../OBJECT/Camera.h"

#include "../util/game.h"
#include "../util/InputState.h"

GameMain::GameMain(SceneManager& manager) : SceneBase(manager),updateFunc_(&GameMain::fadeInUpdate)
{
	player_ = std::make_shared<Player>();
	camera_ = std::make_shared<Camera>();
}

GameMain::~GameMain()
{
}

void GameMain::update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void GameMain::draw()
{
	DrawString(0, 0, "GameMain", 0xffffff);

	{//グリッド線表示　※消去予定
		for (float x = -500.0f; x <= 500.0f; x += 100.0f)
		{
			VECTOR start = VGet(x, 0.0f, -500.0f);
			VECTOR end = VGet(x, 0.0f, 500.0f);
			DrawLine3D(start, end, GetColor(0, 255, 0));
		}
		for (float z = -500.0f; z <= 500.0f; z += 100.0f)
		{
			VECTOR start = VGet(-500.0f, 0.0f, z);
			VECTOR end = VGet(500.0f, 0.0f, z);
			DrawLine3D(start, end, GetColor(255, 255, 0));
		}
	}
	
	player_->draw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//画面全体を真っ黒に塗りつぶす
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameMain::fadeInUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &GameMain::normalUpdate;
		fadeValue_ = 0;
	}
}

void GameMain::normalUpdate(const InputState& input)
{

	player_->update(input);
	camera_->fixedPointCamera(player_->getPos());

	
}

void GameMain::fadeOutUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.changeScene(new GameEnd(manager_));
		return;
	}
}
