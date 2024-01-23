#include "DebugScene.h"
#include "GameMain.h"
#include "GameEnd.h"
#include "ScenePause.h"
#include "SceneTitle.h"
#include "LoadingScene.h"
#include "SceneManager.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include <algorithm>

DebugScene::DebugScene(SceneManager& manager):SceneBase(manager)
{
}

DebugScene::~DebugScene()
{
}

void DebugScene::Init()
{
	sceneName_.push_back("main");
	sceneName_.push_back("title");
	sceneName_.push_back("testScene");
	sceneName_.push_back("end");
	sceneName_.push_back("pause");
	sceneName_.push_back("exit");
}

void DebugScene::End()
{
}

void DebugScene::Update()
{
	//íZèkâª
	auto& input = InputState::GetInstance();

	//TODO:Ç‹Ç∆ÇﬂÇÈ
	if (input.IsTriggered(InputType::up)) {
		selectNum_ = (std::max)(selectNum_ - 1, 0);
	}
	if (input.IsTriggered(InputType::down)) {
		selectNum_ = (std::min)(selectNum_ + 1,4);
	}

	if (input.IsTriggered(InputType::space)) {
		switch (selectNum_) {
		case 0:
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_)));
			break;
		case 1:
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
			break;
		case 2:
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<LoadingScene>(manager_)));
			break;
		case 3:
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameEnd>(manager_)));
			break;
		case 4:
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
			break;
		case 5:
			manager_.SetEndFlag(true);
			break;
		}
	}

}

void DebugScene::Draw()
{

	int y = 150;
	for (auto& scene : sceneName_) {
		DrawFormatString(0, y, 0xffffff, "%s", scene.c_str());
		y += 32;
	}

	DrawString(100, selectNum_ * 32 + 150, "Å©", 0xff0000);

}
