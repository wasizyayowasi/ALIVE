#include "PopUpTextScene.h"
#include "SceneManager.h"
#include "ScenePause.h"
#include "../util/game.h"
#include "../util/InputState.h"

PopUpTextScene::PopUpTextScene(SceneManager& manager):SceneBase(manager)
{
}

PopUpTextScene::~PopUpTextScene()
{
}

void PopUpTextScene::loadText()
{
}

void PopUpTextScene::init()
{
}

void PopUpTextScene::end()
{
}

void PopUpTextScene::update(const InputState& input)
{
	if (input.isTriggered(InputType::next)) {
		manager_.swapScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
	}
}

void PopUpTextScene::draw()
{
	//ëΩï™çÌèúÇ∑ÇÈ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawFormatString(0, 0, 0xffffff, "ïœçXÇµÇ‹ÇµÇΩ");
}
