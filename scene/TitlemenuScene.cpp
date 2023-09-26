#include "TitlemenuScene.h"
#include "SceneManager.h"
#include "ScenePause.h"
#include "GameMain.h"
#include "../util/game.h"
#include "../util/FontsManager.h"
#include "../util/InputState.h"
#include <algorithm>

using namespace std;

TitlemenuScene::TitlemenuScene(SceneManager& manager):SceneBase(manager)
{
	menuName_.push_back("New Game");
	menuName_.push_back("Continue");
	menuName_.push_back("Setting");
	menuName_.push_back("end");
}

TitlemenuScene::~TitlemenuScene()
{
}

void TitlemenuScene::update(const InputState& input)
{
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
	int y = 150;
	int color = 0xffffff;
	for (int i = 0; i < menuName_.size(); i++) {
		if (i == selectNum_) {
			color = 0xff0000;
		}
		else {
			color = 0xffffff;
		}
		int width = FontsManager::getInstance().getStringSize(menuName_[i].c_str(), "High Tower Text");
		int handle = FontsManager::getInstance().getFontHandle("High Tower Text");
		DrawStringToHandle(Game::kScreenWidth / 2 - width / 2, Game::kScreenHeight / 2 + y, menuName_[i].c_str(), color, handle);
		y += 40;
	}
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
