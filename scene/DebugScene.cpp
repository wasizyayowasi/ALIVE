#include "DebugScene.h"
#include "GameMain.h"
#include "GameEnd.h"
#include "ScenePause.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "TitlemenuScene.h"
#include "../util/InputState.h"
#include <algorithm>

DebugScene::DebugScene(SceneManager& manager):SceneBase(manager)
{
	//scene_.push_back(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_, true)));
	//scene_.push_back(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
	//scene_.push_back(std::shared_ptr<SceneBase>(std::make_shared<GameEnd>(manager_)));
	//scene_.push_back(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
}

DebugScene::~DebugScene()
{
}

void DebugScene::init()
{
	sceneName_[0] = { {SceneType::main},{"main"} };
	sceneName_[1] = { {SceneType::title }, { "title" } };
	sceneName_[2] = { {SceneType::end} ,{"end"} };
	sceneName_[3] = { {SceneType::pause} ,{"pause"} };
	sceneName_[4] = { {SceneType::max} ,{"ï¬Ç∂ÇÈ"} };
}

void DebugScene::end()
{
}

void DebugScene::update(const InputState& input)
{
	//TODO:Ç‹Ç∆ÇﬂÇÈ
	if (input.isTriggered(InputType::up)) {
		selectNum_ = (std::max)(selectNum_ - 1, 0);
	}
	if (input.isTriggered(InputType::down)) {
		selectNum_ = (std::min)(selectNum_ + 1,4);
	}

	if (input.isTriggered(InputType::space)) {
		//manager_.changeScene(scene_[selectNum_]);
		switch (selectNum_) {
		case 0:
			manager_.changeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_, true)));
			break;
		case 1:
			manager_.changeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
			break;
		case 2:
			manager_.changeScene(std::shared_ptr<SceneBase>(std::make_shared<GameEnd>(manager_)));
			break;
		case 3:
			manager_.changeScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
			break;
		case 4:
			DxLib_End();
			break;
		}
	}

}

void DebugScene::draw()
{

	int y = 150;
	for (auto& scene : sceneName_) {
		DrawFormatString(0, y, 0xffffff, "%s", scene.second.sceneNameText.c_str());
		y += 32;
	}

	DrawString(100, selectNum_ * 32 + 150, "Å©", 0xff0000);

}
