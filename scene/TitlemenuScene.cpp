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

void TitlemenuScene::Init()
{
	titleHandle_ = LoadGraph("data/graph/title.png");
	UI_ = std::make_shared<UIItemManager>();

	menuName_.push_back("New Game");
	menuName_.push_back("Continue");
	menuName_.push_back("Setting");
	menuName_.push_back("end");

	int font = FontsManager::getInstance().GetFontHandle("High Tower Text");
	int y = 120;
	for (auto& menu : menuName_) {
		UI_->addMenu(Game::screen_width / 2, Game::screen_height / 2 + y,320, 100, menu.c_str(), font);
		y += 40;
	}

}

void TitlemenuScene::End()
{
	DeleteGraph(titleHandle_);
}

void TitlemenuScene::Update(const InputState& input)
{
	//TODO:‚Ü‚Æ‚ß‚é
	if (input.IsTriggered(InputType::up)) {
		selectNum_ = (max)(selectNum_ - 1, 0);
	}
	if (input.IsTriggered(InputType::down)) {
		selectNum_ = (min)(selectNum_ + 1, static_cast<int>(menuName_.size() - 1));
	}

	if (input.IsTriggered(InputType::space)) {
		SceneChange();
	}

}

void TitlemenuScene::Draw()
{
	UI_->draw(selectNum_);
	DrawRotaGraph(Game::screen_width / 2, Game::screen_height / 3, 1.0f, 0.0f, titleHandle_, true);
}

void TitlemenuScene::SceneChange()
{
	switch (selectNum_) {
	case 0:
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_,false)));
		break;
	case 1:
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_,true)));
		break;
	case 2:
		manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
		break;
	case 3:
		DxLib_End();
		break;
	}
}
