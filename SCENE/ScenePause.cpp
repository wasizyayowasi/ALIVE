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

void ScenePause::Init()
{
	menuName_.push_back("������");
	menuName_.push_back("����ݒ�");
	menuName_.push_back("�߂�");
	menuName_.push_back("�^�C�g����");
	menuName_.push_back("�f�o�b�O�V�[����");
}

void ScenePause::End()
{
}

void ScenePause::Update(const InputState& input)
{
	//TODO:�܂Ƃ߂�
	//���ڑI��
	{
		if (input.IsTriggered(InputType::up)) {
			selectionNum_ = (std::max)(selectionNum_ - 1, 0);
		}
		if (input.IsTriggered(InputType::down)) {
			selectionNum_ = (std::min)(selectionNum_ + 1, 4);
		}
	}
	

	if (input.IsTriggered(InputType::space)) {
		 switch(selectionNum_) {
		//�T�E���h�Z�b�e�B���O�ւ̑J��
		 case 0:
			 manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<SoundSettingScene>(manager_)));
			 break;
		//�L�[�R���t�B�O�V�[���ւ̑J��
		case 1:
			manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<KeyConfigScene>(manager_,input)));
			break;
		//��O�̃V�[��(���C���V�[��)�֑J��
		case 2:
			manager_.PopScene();
			break;
		//�^�C�g���V�[���ւ̑J��
		case 3:
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
			break;
		//�f�o�b�O�V�[���ɖ߂�
		case 4:
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<DebugScene>(manager_)));
			break;
		}
	}

	//��O�̃V�[��(���C���V�[��)�֑J��
	if (input.IsTriggered(InputType::pause)) {
		manager_.PopScene();
	}
}

void ScenePause::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	DrawString(0, 0, "scenePause",0xff0000);

	DrawString(90, 16 * selectionNum_ + 32, "��", 0xffff00);

	int y = 32;
	for (auto& menu : menuName_) {
		DrawFormatString(0, y, 0x00ff00, "%s", menu.c_str());
		y += 16;
	}
}
