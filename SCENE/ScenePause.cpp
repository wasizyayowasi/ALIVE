#include "ScenePause.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "KeyConfigScene.h"
#include "SettingScene.h"
#include "DebugScene.h"

#include "../util/InputState.h"
#include "../util/game.h"
#include "../util/FontsManager.h"
#include "../util/UIItemManager.h"
#include "../util/Util.h"

#include <algorithm>

ScenePause::ScenePause(SceneManager& manager):SceneBase(manager)
{
}

ScenePause::~ScenePause()
{
}

void ScenePause::Init()
{
	//�C���X�^���X��
	UI_ = std::make_shared<UIItemManager>();

	//UI������̍쐬
	menuName_.push_back("�ĊJ");
	menuName_.push_back("�ݒ�");
	menuName_.push_back("�^�C�g����");
	menuName_.push_back("�f�o�b�O�V�[����");

	//UI�摜�̍쐬
	int font = FontsManager::getInstance().GetFontHandle("�s�O�� 0042");
	int y = 100;
	for (auto& menu : menuName_) {
		UI_->AddMenu(Game::screen_width / 2, Game::screen_height / 2 + y, 640, 200, menu.c_str(), font);
		y += 40;
	}

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
			selectNum_ = (std::max)(selectNum_ - 1, 0);
		}
		if (input.IsTriggered(InputType::down)) {
			selectNum_ = (std::min)(selectNum_ + 1, 3);
		}
	}
	
	if (input.IsTriggered(InputType::space)) {
		 switch(selectNum_) {
		//��O�̃V�[��(���C���V�[��)�֑J��
		case 0:
			manager_.PopFrontScene();
			break;
		//�T�E���h�Z�b�e�B���O�ւ̑J��
		 case 1:
			 manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<SettingScene>(manager_)));
			 break;
		//�^�C�g���V�[���ւ̑J��
		case 2:
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
			break;
		//�f�o�b�O�V�[���ɖ߂�
		case 3:
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<DebugScene>(manager_)));
			break;
		}
	}

	//��O�̃V�[��(���C���V�[��)�֑J��
	if (input.IsTriggered(InputType::pause)) {
		manager_.PopFrontScene();
	}
}

void ScenePause::Draw()
{
	//�w�i���̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

	//UI�̕`��
	UI_->AlphaChangeDraw(selectNum_);

}
