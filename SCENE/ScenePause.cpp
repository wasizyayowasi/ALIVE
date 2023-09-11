#include "ScenePause.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "KeyConfigScene.h"
#include "../UTIL/InputState.h"
#include "../UTIL/game.h"
#include <algorithm>

ScenePause::ScenePause(SceneManager& manager):SceneBase(manager)
{
}

ScenePause::~ScenePause()
{
}

void ScenePause::update(const InputState& input)
{

	if (input.isTriggered(InputType::up)) {
		selectionNum_ = (std::max)(selectionNum_ - 1, 0);
	}
	if (input.isTriggered(InputType::down)) {
		selectionNum_ = (std::min)(selectionNum_ + 1, 2);
	}

	if (input.isTriggered(InputType::next)) {
		 switch(selectionNum_) {
			case 0:
				manager_.swapScene(new KeyConfigScene(manager_,input));
				break;
			case 1:
				manager_.popScene();
				break;
			case 2:
				//manager_.changeScene(new SceneTitle(manager_));
				break;
		}
	}

	if (input.isTriggered(InputType::pause)) {
		manager_.popScene();
	}
}

void ScenePause::draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	DrawString(0, 0, "scenePause",0xff0000);

	DrawString(90, 16 * selectionNum_ + 32, "��", 0xffff00);

	DrawString(0, 32, "����ݒ�",0x00ff00);
	DrawString(0, 48, "�߂�",0x00ff00);
	DrawString(0, 64, "�^�C�g����",0x00ff00);

	
}
