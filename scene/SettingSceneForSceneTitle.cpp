#include "SettingSceneForSceneTitle.h"
#include "SceneManager.h"
#include "ScenePause.h"
#include "KeyConfigSceneForSceneTitle.h"

#include "../util/game.h"
#include "../util/InputState.h"
#include "../util/SoundManager.h"
#include "../util/FontsManager.h"
#include "../util/ExternalFile.h"
#include "../util/UIItemManager.h"

#include <algorithm>

using namespace std;

namespace {
	//����1�������̑傫��
	constexpr int sound_1_momery_size = 25;

	//���̍ő僁����
	constexpr int sound_max_memory = 10;
	 
	//���̍ŏ�������
	constexpr int sound_min_memory = 1;

	//�~���m�̊Ԋu
	constexpr float circle_distance = 53.5f;

	//�~�̔��a
	constexpr float circle_radius = 14.5f;

	//�t�H���g�̖��O
	const char* const pigumo42_font_name = "�s�O�� 0042";
}

//�R���X�g���N�^
SettingSceneForSceneTitle::SettingSceneForSceneTitle(SceneManager& manager) :SceneBase(manager), updateFunc_(&SettingSceneForSceneTitle::GaussFadeInUpdate)
{
}

//�f�X�g���N�^
SettingSceneForSceneTitle::~SettingSceneForSceneTitle()
{
}

//������
void SettingSceneForSceneTitle::Init()
{
	//�Z�k��
	auto& sound = SoundManager::GetInstance();
	auto& font = FontsManager::GetInstance();
	auto& file = ExternalFile::GetInstance();

	//�C���X�^���X��
	UIManager_ = std::make_shared<UIItemManager>();

	//���݂̃{�����[���̎擾
	volumeBGM_ = sound.GetBGMVolume() / sound_1_momery_size;
	volumeSE_ = sound.GetSEVolume() / sound_1_momery_size;

	//UI�摜�̍쐬
	//�t�H���g�̎擾
	int pigumo42Font = font.GetFontHandle(pigumo42_font_name);
	//�t�H���g��K�p����������̃T�C�Y�擾
	int windowFontSize = font.GetStringSize("���[�h", pigumo42_font_name);
	int BGMFontSize = font.GetStringSize("BGM", pigumo42_font_name);
	int SEFontSize = font.GetStringSize("SE", pigumo42_font_name);

	//�`����W
	menuDrawPos_["���[�h"]		= file.GetUIPos("windowModeUIPos");
	menuDrawPos_["BGM"]			= file.GetUIPos("BGMUIPos");
	menuDrawPos_["SE"]			= file.GetUIPos("SEUIPos");
	menuDrawPos_["����ݒ�"]	= file.GetUIPos("advancedSettingUIPos");
	menuDrawPos_["�߂�"]		= file.GetUIPos("backUIPos");

	//UI�摜�̍쐬
	UIManager_->AddMenu(static_cast<float>(Game::screen_width / 4), static_cast<float>(Game::screen_height / 3),          320, 100, "���[�h",   pigumo42Font);
	UIManager_->AddMenu(static_cast<float>(Game::screen_width / 4), static_cast<float>(Game::screen_height / 2 - 20),     320, 100, "BGM",      pigumo42Font);
	UIManager_->AddMenu(static_cast<float>(Game::screen_width / 4), static_cast<float>(Game::screen_height / 3 * 2 - 20), 320, 100, "SE",       pigumo42Font);
	UIManager_->AddMenu(static_cast<float>(Game::screen_width / 2), static_cast<float>(Game::screen_height / 4 * 3),      320, 100, "����ݒ�", pigumo42Font);
	UIManager_->AddMenu(static_cast<float>(Game::screen_width / 2), static_cast<float>(Game::screen_height / 4 * 3 + 40), 320, 100, "�߂�",     pigumo42Font);

	makeScreenHandle_ = MakeScreen(Game::screen_width, Game::screen_height, true);

	if (manager_.GetWindowMode()) 
	{
		windowModeText_ = "��  �E�B���h�E���[�h  ��";
	}
	else {
		windowModeText_ = "��  �t���X�N���[��  ��";
	}

}

//�I��
void SettingSceneForSceneTitle::End()
{
	DeleteGraph(makeScreenHandle_);
}

//�X�V
void SettingSceneForSceneTitle::Update()
{
	(this->*updateFunc_)();
}

//�`��
void SettingSceneForSceneTitle::Draw()
{
	//UI������̕`��
	UIManager_->AlphaChangeDrawBillBoard(menuDrawPos_, selectNum_,fadeValue_, 20.0f);

	//�쐬������ʃT�C�Y�̉摜��
	//�ȍ~�̓��e����������
	SetDrawScreen(makeScreenHandle_);
	ClearDrawScreen();

	//�Z�k��
	auto& font = FontsManager::GetInstance();

	//���݂̉�ʃ��[�h��\��
	int pigumo42 = font.GetFontHandle(pigumo42_font_name);
	float windowModeFontSize = static_cast<float>(font.GetStringSize(windowModeText_.c_str(), pigumo42_font_name));

	int alpha[3] = {};
	
	for (int i = 0; i < 3; i++) 
	{
		if (selectNum_ == i) 
		{
			alpha[i] = 250;
		}
		else 
		{
			alpha[i] = 150;
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha[0]);
	DrawStringFToHandle(static_cast<float>(Game::screen_width / 2) - windowModeFontSize / 2.5f, Game::screen_height / 3.5f, windowModeText_.c_str(), 0xffffff, pigumo42);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�~��`�悷��ʒu
	float circlePosX = static_cast<float>(Game::screen_width / 3) - circle_distance / 2;

	//�g�k��
	float scale = 1.0f;

	//�F
	int color = 0xb1b3b6;

	//BGM�o�[
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha[1]);

	//���ʃo�[�摜�̕`��
	for (int i = 1; i < 11; i++) 
	{
		scale = 1.0f;
		color = 0xb1b3b6;

		if (volumeBGM_ == i) 
		{
			scale = 1.3f;
			color = 0xff0000;
		}

		DrawCircleAA(circlePosX + i * circle_distance, static_cast<float>(Game::screen_height / 2) - circle_distance / 2, circle_radius * scale, 32, color, true);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//SE�o�[
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha[2]);

	//���ʃo�[�摜�̕`��
	for (int i = 1; i < 11; i++) 
	{
		scale = 1.0f;
		color = 0xb1b3b6;

		if (volumeSE_ == i) 
		{
			scale = 1.3f;
			color = 0xff0000;
		}

		DrawCircleAA(circlePosX + i * circle_distance, Game::screen_height / 3 * 2 - circle_distance, circle_radius * scale, 32, color, true);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetDrawScreen(DX_SCREEN_BACK);

	//�t�F�[�h�̎��A�A���t�@�l��ύX����ƂƂ��ɃK�E�X�ڂ������|����
	GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, 8, 255 - fadeValue_);
	//�`��摜�̃A���t�@�l��ύX����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawGraph(0, 0, makeScreenHandle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//�ʏ펞�̍X�V
void SettingSceneForSceneTitle::NormalUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	if (input.IsTriggered(InputType::Up))
	{
		selectNum_ = (std::max)(selectNum_ - 1, 0);
	}
	else if (input.IsTriggered(InputType::Down)) 
	{
		selectNum_ = (std::min)(selectNum_ + 1, 4);
	}

	ChangeUpdateFunc();

	//�V�[���؂�ւ�
	if (input.IsTriggered(InputType::Activate)) 
	{
		selectNum_ = 4;
		updateFunc_ = &SettingSceneForSceneTitle::GaussFadeOutUpdate;
	}
}

//�K�E�X�������{�����t�F�[�h�C��
void SettingSceneForSceneTitle::GaussFadeInUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_)
	{
		updateFunc_ = &SettingSceneForSceneTitle::NormalUpdate;
		fadeValue_ = 255;
	}
}

//�K�E�X�������{�����t�F�[�h�A�E�g
void SettingSceneForSceneTitle::GaussFadeOutUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) 
	{
		SceneChange();
		fadeValue_ = 0;
		return;
	}
}

//BGM�̉��ʂ�ύX����
void SettingSceneForSceneTitle::BGMUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//BGM���ʒ���
	if (input.IsTriggered(InputType::Left))
	{
		volumeBGM_ = (max)(volumeBGM_ - 1, sound_min_memory);
	}
	if (input.IsTriggered(InputType::Right)) 
	{
		volumeBGM_ = (min)(volumeBGM_ + 1, sound_max_memory);
	}

	//���ʂ̕ύX
	SoundManager::GetInstance().SetBGMVolume(volumeBGM_ * sound_1_momery_size);
}

//SE�̉��ʂ�ύX����
void SettingSceneForSceneTitle::SEUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//SE���ʒ���
	if (input.IsTriggered(InputType::Left))
	{
		volumeSE_ = (max)(volumeSE_ - 1, sound_min_memory);
	}
	if (input.IsTriggered(InputType::Right)) 
	{
		volumeSE_ = (min)(volumeSE_ + 1, sound_max_memory);
	}

	//���ʂ̕ύX
	SoundManager::GetInstance().SetSEVolume(volumeSE_ * sound_1_momery_size);
}

//�E�B���h�E���[�h��ύX����
void SettingSceneForSceneTitle::ChangeWindowUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	if (input.IsTriggered(InputType::Left))
	{
		windowModeText_ = "��  �E�B���h�E���[�h  ��";
		manager_.SetChangeWindowMode(true);
	}
	if (input.IsTriggered(InputType::Right))
	{
		windowModeText_ = "��  �t���X�N���[��  ��";
		manager_.SetChangeWindowMode(false);
	}
}

//updateFunc�̒��g��ύX����
void SettingSceneForSceneTitle::ChangeUpdateFunc()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	switch (selectNum_) 
	{
	case 0:
		ChangeWindowUpdate();
		break;
	case 1:
		BGMUpdate();
		break;
	case 2:
		SEUpdate();
		break;
	case 3:
		if (input.IsTriggered(InputType::Space)) {
			updateFunc_ = &SettingSceneForSceneTitle::GaussFadeOutUpdate;
		}
		break;
	case 4:
		if (input.IsTriggered(InputType::Space)) {
			updateFunc_ = &SettingSceneForSceneTitle::GaussFadeOutUpdate;
		}
		break;
	}
}

//�V�[����؂�ւ���
void SettingSceneForSceneTitle::SceneChange()
{
	switch (selectNum_)
	{
	case 3:
		manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<KeyConfigSceneForSceneTitle>(manager_)));
		break;
	case 4:
		manager_.PopFrontScene();
		break;
	}
}
