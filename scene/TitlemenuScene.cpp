#include "TitlemenuScene.h"
#include "SceneManager.h"
#include "ScenePause.h"
#include "GameMain.h"
#include "../util/game.h"
#include "../util/FontsManager.h"
#include "../util/InputState.h"
#include "../util/UIItemManager.h"
#include <algorithm>

using namespace std;

TitlemenuScene::TitlemenuScene(SceneManager& manager):SceneBase(manager)
{
	
}

TitlemenuScene::~TitlemenuScene()
{
}

void TitlemenuScene::init()
{
	titleHandle_ = LoadGraph("data/graph/title.png");
	UI_ = std::make_shared<UIItemManager>();

	menuName_.push_back("New Game");
	menuName_.push_back("Continue");
	menuName_.push_back("Setting");
	menuName_.push_back("end");

	int font = FontsManager::getInstance().getFontHandle("High Tower Text");
	int y = 120;
	for (auto& menu : menuName_) {
		UI_->addMenu(Game::kScreenWidth / 2, Game::kScreenHeight / 2 + y,320, 100, menu.c_str(), font);
		y += 40;
	}

}

void TitlemenuScene::end()
{
	DeleteGraph(titleHandle_);
}

void TitlemenuScene::update(const InputState& input)
{
	//TODO:‚Ü‚Æ‚ß‚é
	if (input.isTriggered(InputType::up)) {
		selectNum_ = (max)(selectNum_ - 1, 0);
	}
	if (input.isTriggered(InputType::down)) {
		selectNum_ = (min)(selectNum_ + 1, static_cast<int>(menuName_.size() - 1));
	}

	if (input.isTriggered(InputType::next)) {
		sceneChange();
	}

}

void TitlemenuScene::draw()
{
	UI_->draw(selectNum_);
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 3, 1.0f, 0.0f, titleHandle_, true);
}

void TitlemenuScene::sceneChange()
{
	switch (selectNum_) {
	case 0:
		manager_.changeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_,false)));
		break;
	case 1:
		manager_.changeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_,true)));
		break;
	case 2:
		manager_.swapScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
		break;
	case 3:
		DxLib_End();
		break;
	}
}
