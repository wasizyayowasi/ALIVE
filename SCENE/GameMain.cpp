#include "GameMain.h"
#include "SceneManager.h"
#include "GameEnd.h"

#include "../object/Player.h"
#include "../object/Camera.h"

#include "../staging/Broom.h"
#include "../staging/DepthOfField.h"

#include "ScenePause.h" 

#include "../util/game.h"
#include "../util/SoundManager.h"
#include "../util/InputState.h"
#include "../util/model.h"

using namespace std;

namespace {
	const char* const temp_filepath = "data/model/tempFiled2.mv1";
	const char* const cube_filename = "DATA/model/cube.mv1";
	const VECTOR scale = { 0.5f,0.5f, 0.5f };
}

GameMain::GameMain(SceneManager& manager) : SceneBase(manager),updateFunc_(&GameMain::fadeInUpdate)
{
	camera_ = make_shared<Camera>();
	player_ = make_shared<Player>();
	//broom_ = make_shared<Broom>();
	//depthOfField_ = make_shared<DepthOfField>();
	models_.push_back(make_shared<Model>(temp_filepath));

	models_[0]->setScale(scale);
	models_[0]->setCollFrame();

	SetUseLighting(false);

	Set3DSoundOneMetre(10.0f);

	SoundManager::getInstance().set3DSoundInfo(VGet(575,120,-60),1000,"cafe");

	SoundManager::getInstance().play("cafe");


}

GameMain::~GameMain()
{
}

void GameMain::update(const InputState& input)
{
	(this->*updateFunc_)(input);
	SoundManager::getInstance().set3DSoundListenerInfo(player_->getPos(), VAdd(player_->getPos(), player_->getRot()));
}

void GameMain::draw()
{
	//broom_->writingScreenUpdate(player_->getPos());
	DrawString(0, 0, "GameMain", 0xffffff);

	for (auto& model : models_) {
		model->draw();
	}

	player_->draw();
	//broom_->graphFilterUpdate();
	//broom_->draw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//âÊñ ëSëÃÇê^Ç¡çïÇ…ìhÇËÇ¬Ç‘Ç∑
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

	player_->update(input,models_);
	camera_->trackingCameraUpdate(input,player_->getPos());

	if (input.isTriggered(InputType::pause)) {
		manager_.pushScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
	}
}

void GameMain::fadeOutUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.changeScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
		return;
	}
}
