#include "GameMain.h"
#include "SceneManager.h"
#include "GameEnd.h"

#include "../OBJECT/Player.h"
#include "../OBJECT/Camera.h"

#include "../staging/Broom.h"
#include "../staging/DepthOfField.h"

#include "../util/game.h"
#include "../util/InputState.h"
#include "../util/model.h"

namespace {
	const char* const tempFilepath = "data/model/tempFiled.mv1";
	const VECTOR scale = { 0.5f,0.5f, 0.5f };
}

GameMain::GameMain(SceneManager& manager) : SceneBase(manager),updateFunc_(&GameMain::fadeInUpdate)
{
	player_ = std::make_shared<Player>();
	broom_ = std::make_shared<Broom>();
	depthOfField_ = std::make_shared<DepthOfField>();
	temp_ = std::make_shared<Model>(tempFilepath);

	temp_->setScale(scale);

	SetUseLighting(false);

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
	broom_->writingScreenUpdate(player_->getPos());
	DrawString(0, 0, "GameMain", 0xffffff);

	int color;
	{//グリッド線表示　※消去予定
		for (float x = -500.0f; x <= 500.0f; x += 100.0f)
		{
			VECTOR start = VGet(x, 0.0f, -500.0f);
			VECTOR end = VGet(x, 0.0f, 500.0f);
			if (x == 0.0f) {
				color = 0xff0000;
			}
			else {
				color = 0xffff00;
			}
			DrawLine3D(start, end, color);
		}
		for (float z = -500.0f; z <= 500.0f; z += 100.0f)
		{
			VECTOR start = VGet(-500.0f, 0.0f, z);
			VECTOR end = VGet(500.0f, 0.0f, z);
			DrawLine3D(start, end, GetColor(255, 255, 0));
		}

		for (float x = 500.0f; x <= 1500.0f; x += 100.0f)
		{
			VECTOR start = VGet(x, 0.0f, -500.0f);
			VECTOR end = VGet(x, 0.0f, 500.0f);
			if (x == 1000.0f) {
				color = 0xff0000;
			}
			else {
				color = 0x00ff00;
			}
			DrawLine3D(start, end, color);
		}
		for (float z = -500.0f; z <= 500.0f; z += 100.0f)
		{
			VECTOR start = VGet(500.0f, 0.0f, z);
			VECTOR end = VGet(1500.0f, 0.0f, z);
			DrawLine3D(start, end, GetColor(0, 255, 0));
		}

		for (float x = 1500.0f; x <= 2500.0f; x += 100.0f)
		{
			VECTOR start = VGet(x, 0.0f, -500.0f);
			VECTOR end = VGet(x, 0.0f, 500.0f);
			if (x == 2000.0f) {
				color = 0xff0000;
			}
			else {
				color = 0x0000ff;
			}
			DrawLine3D(start, end, color);
		}
		for (float z = -500.0f; z <= 500.0f; z += 100.0f)
		{
			VECTOR start = VGet(1500.0f, 0.0f, z);
			VECTOR end = VGet(2500.0f, 0.0f, z);
			DrawLine3D(start, end, GetColor(0, 0, 255));
		}
	}
	
	DrawSphere3D(VAdd(player_->getPos(), { 40,0,0 }), 16, 32, 0xffffff,0xffffff, true);

	temp_->draw();
	player_->draw();
	broom_->graphFilterUpdate();
	broom_->draw();

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

}

void GameMain::fadeOutUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.changeScene(new GameEnd(manager_));
		return;
	}
}
