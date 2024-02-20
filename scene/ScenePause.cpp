#include "ScenePause.h"
#include "SceneTitle.h"
#include "DebugScene.h"
#include "SceneManager.h"
#include "SettingScene.h"
#include "KeyConfigScene.h"

#include "../util/Util.h"
#include "../util/game.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/FontsManager.h"
#include "../util/GraphManager.h"
#include "../util/UIItemManager.h"

#include <algorithm>

//�R���X�g���N�^
ScenePause::ScenePause(SceneManager& manager):SceneBase(manager)
{
}

//�f�X�g���N�^
ScenePause::~ScenePause()
{
}

//������
void ScenePause::Init()
{
	//�C���X�^���X��
	UI_ = std::make_shared<UIItemManager>();

	//UI������̍쐬
	menuName_.push_back("�߂�");
	menuName_.push_back("�ݒ�");
	menuName_.push_back("�^�C�g����");
#ifdef _DEBUG
	menuName_.push_back("�f�o�b�O�V�[����");
#endif // _DEBUG

	//UI�摜�̍쐬
	int font = FontsManager::GetInstance().GetFontHandle("�s�O�� 0042");
	int y = 100;
#ifdef _DEBUG
#else
	y += 40;
#endif // _DEBUG

	for (auto& menu : menuName_) {
		UI_->AddMenu(static_cast<float>(Game::screen_width / 2), static_cast<float>(Game::screen_height / 2 + y), 640, 200, menu.c_str(), font);
		y += 40;
	}
}

//�I��
void ScenePause::End()
{
}

//�X�V
void ScenePause::Update()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//TODO:�܂Ƃ߂�
	//���ڑI��
	{
		if (input.IsTriggered(InputType::Up)) {
			selectNum_ = (std::max)(selectNum_ - 1, 0);
		}
		if (input.IsTriggered(InputType::Down)) {
			selectNum_ = (std::min)(selectNum_ + 1, static_cast<int>(menuName_.size()) - 1);
		}
	}
	
	if (input.IsTriggered(InputType::Space)) {
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
	if (input.IsTriggered(InputType::Pause)) {
		manager_.PopFrontScene();
	}
}

//�`��
void ScenePause::Draw()
{
	//�w�i���̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

	//�^�C�g���摜�̕`��
	DrawRotaGraph(Game::screen_width / 2, Game::screen_height / 3, 1.0f, 0.0f, GraphManager::GetInstance().GetGraph("title"), true);

	//UI�̕`��
	UI_->AlphaChangeDraw(selectNum_,255);
}
