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

void PopUpTextScene::Init()
{
}

void PopUpTextScene::End()
{
}

void PopUpTextScene::Update(const InputState& input)
{
	if (input.IsTriggered(InputType::space)) {
		manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
	}
}

void PopUpTextScene::Draw()
{
	//多分削除する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawFormatString(0, 0, 0xffffff, "変更しました");
}
