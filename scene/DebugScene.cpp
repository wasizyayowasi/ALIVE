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

void DebugScene::Init()
{
	sceneName_[0] = { {SceneType::main},{"main"} };
	sceneName_[1] = { {SceneType::title }, { "title" } };
	sceneName_[2] = { {SceneType::end} ,{"end"} };
	sceneName_[3] = { {SceneType::pause} ,{"pause"} };
	sceneName_[4] = { {SceneType::max} ,{"����"} };
}

void DebugScene::End()
{
}

void DebugScene::Update(const InputState& input)
{
	//TODO:�܂Ƃ߂�
	if (input.IsTriggered(InputType::up)) {
		selectNum_ = (std::max)(selectNum_ - 1, 0);
	}
	if (input.IsTriggered(InputType::down)) {
		selectNum_ = (std::min)(selectNum_ + 1,4);
	}

	if (input.IsTriggered(InputType::space)) {
		//manager_.changeScene(scene_[selectNum_]);
		switch (selectNum_) {
		case 0:
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_, true)));
			break;
		case 1:
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
			break;
		case 2:
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameEnd>(manager_)));
			break;
		case 3:
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
			break;
		case 4:
			manager_.SetEndFlag(true);
			break;
		}
	}

}

void DebugScene::Draw()
{

	int y = 150;
	for (auto& scene : sceneName_) {
		DrawFormatString(0, y, 0xffffff, "%s", scene.second.sceneNameText.c_str());
		y += 32;
	}

	DrawString(100, selectNum_ * 32 + 150, "��", 0xff0000);

}
