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
	//���ڑI��
	{
		if (input.isTriggered(InputType::up)) {
			selectionNum_ = (std::max)(selectionNum_ - 1, 0);
		}
		if (input.isTriggered(InputType::down)) {
			selectionNum_ = (std::min)(selectionNum_ + 1, 2);
		}
	}
	

	if (input.isTriggered(InputType::next)) {
		 switch(selectionNum_) {
			 //�L�[�R���t�B�O�V�[���ւ̑J��
			case 0:
				manager_.swapScene(std::shared_ptr<SceneBase>(std::make_shared<KeyConfigScene>(manager_,input)));
				break;
			//��O�̃V�[��(���C���V�[��)�֑J��
			case 1:
				manager_.popScene();
				break;
			//�^�C�g���V�[���ւ̑J��
			case 2:
				manager_.changeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
				break;
		}
	}

	//��O�̃V�[��(���C���V�[��)�֑J��
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
