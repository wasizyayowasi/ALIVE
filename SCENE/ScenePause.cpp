#include "ScenePause.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "KeyConfigScene.h"
#include "SoundSettingScene.h"
#include "DebugScene.h"
#include "../util/InputState.h"
#include "../util/game.h"
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
			selectionNum_ = (std::min)(selectionNum_ + 1, 4);
		}
	}
	

	if (input.isTriggered(InputType::next)) {
		 switch(selectionNum_) {
		//�T�E���h�Z�b�e�B���O�ւ̑J��
		 case 0:
			 manager_.swapScene(std::shared_ptr<SceneBase>(std::make_shared<SoundSettingScene>(manager_)));
			 break;
		//�L�[�R���t�B�O�V�[���ւ̑J��
		case 1:
			manager_.swapScene(std::shared_ptr<SceneBase>(std::make_shared<KeyConfigScene>(manager_,input)));
			break;
		//��O�̃V�[��(���C���V�[��)�֑J��
		case 2:
			manager_.popScene();
			break;
		//�^�C�g���V�[���ւ̑J��
		case 3:
			manager_.changeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
			break;
		//�f�o�b�O�V�[���ɖ߂�
		case 4:
			manager_.changeScene(std::shared_ptr<SceneBase>(std::make_shared<DebugScene>(manager_)));
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

	DrawString(0, 32, "������",0x00ff00);
	DrawString(0, 48, "����ݒ�",0x00ff00);
	DrawString(0, 64, "�߂�",0x00ff00);
	DrawString(0, 80, "�^�C�g����",0x00ff00);
	DrawString(0, 96, "�f�o�b�O�V�[����",0x00ff00);
}
