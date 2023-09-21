#include "DebugScene.h"
#include "GameMain.h"
#include "GameEnd.h"
#include "ScenePause.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "../util/InputState.h"
#include <algorithm>

DebugScene::DebugScene(SceneManager& manager):SceneBase(manager)
{
	scene_.push_back(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager)));
	scene_.push_back(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager)));
	scene_.push_back(std::shared_ptr<SceneBase>(std::make_shared<GameEnd>(manager)));
	scene_.push_back(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager)));

	sceneName_[0] = { {SceneType::main},{"main"}};
	sceneName_[1] = { {SceneType::title }, { "title" }};
	sceneName_[2] = { {SceneType::end} ,{"end"} };
	sceneName_[3] = { {SceneType::pause} ,{"pause"} };

}

DebugScene::~DebugScene()
{
}

void DebugScene::update(const InputState& input)
{
	if (input.isTriggered(InputType::up)) {
		selectNum_ = (std::max)(selectNum_ - 1, 0);
	}
	if (input.isTriggered(InputType::down)) {
		selectNum_ = (std::min)(selectNum_ + 1,static_cast<int>(scene_.size() - 1));
	}

	if (input.isTriggered(InputType::next)) {
		manager_.changeScene(scene_[selectNum_]);
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
