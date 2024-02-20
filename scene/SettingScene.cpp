#include "SettingScene.h"
#include "SceneManager.h"
#include "ScenePause.h"
#include "KeyConfigScene.h"

#include "../util/game.h"
#include "../util/InputState.h"
#include "../util/SoundManager.h"
#include "../util/FontsManager.h"
#include "../util/UIItemManager.h"

#include <algorithm>

using namespace std;

namespace {
	//�t�H���g�̖��O
	const char* const pigumo42_font_name = "�s�O�� 0042";
	//�~���m�̊Ԋu
	constexpr float circle_distance = 53.5f;
	//�~�̔��a
	constexpr float circle_radius = 14.5f;
}

//�R���X�g���N�^
SettingScene::SettingScene(SceneManager& manager):SceneBase(manager),updateFunc_(&SettingScene::GaussFadeInUpdate)
{
}

//�f�X�g���N�^
SettingScene::~SettingScene()
{
}

//������
void SettingScene::Init()
{
	//�Z�k��
	auto& sound = SoundManager::GetInstance();
	auto& font = FontsManager::GetInstance();

	//�C���X�^���X��
	UIManager_ = std::make_shared<UIItemManager>();

	//���݂̃{�����[���̎擾
	volumeBGM_ = sound.GetBGMVolume() / 25;
	volumeSE_ = sound.GetSEVolume() / 25;

	//UI�摜�̍쐬
	//�t�H���g�̎擾
	int pigumo42Font = font.GetFontHandle(pigumo42_font_name);
	//�t�H���g��K�p����������̃T�C�Y�擾
	int windowFontSize = font.GetStringSize("���[�h", pigumo42_font_name);
	int BGMFontSize = font.GetStringSize("BGM", pigumo42_font_name);
	int SEFontSize = font.GetStringSize("SE", pigumo42_font_name);

	//UI�摜�̍쐬
	UIManager_->AddMenu(Game::screen_width / 4, Game::screen_height / 3, 320, 100, "���[�h", pigumo42Font);
	UIManager_->AddMenu(Game::screen_width / 4, Game::screen_height / 2 - 20, 320, 100, "BGM", pigumo42Font);
	UIManager_->AddMenu(Game::screen_width / 4, Game::screen_height / 3 * 2 - 20, 320, 100, "SE", pigumo42Font);
	UIManager_->AddMenu(Game::screen_width / 2, Game::screen_height / 4 * 3, 320, 100, "����ݒ�", pigumo42Font);
	UIManager_->AddMenu(Game::screen_width / 2, Game::screen_height / 4 * 3 + 40, 320, 100, "�߂�", pigumo42Font);

	makeScreenHandle_ = MakeScreen(Game::screen_width, Game::screen_height, true);

	if (manager_.GetWindowMode()) {
		windowModeText_ = "��  �E�B���h�E���[�h  ��";
	}
	else {
		windowModeText_ = "��  �t���X�N���[��  ��";
	}

}

//�I��
void SettingScene::End()
{
	DeleteGraph(makeScreenHandle_);
}

//�X�V
void SettingScene::Update()
{
	(this->*updateFunc_)();
}

//�`��
void SettingScene::Draw()
{
	//�������߂������̔w�i��`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�쐬������ʃT�C�Y�̉摜��
	//�ȍ~�̓��e����������
	SetDrawScreen(makeScreenHandle_);
	ClearDrawScreen();

	//UI������̕`��
	UIManager_->AlphaChangeDraw(selectNum_,fadeValue_);

	//�Z�k��
	auto& font = FontsManager::GetInstance();

	//���݂̉�ʃ��[�h��\��
	int pigumo42 = font.GetFontHandle(pigumo42_font_name);
	int windowModeFontSize = font.GetStringSize(windowModeText_.c_str(), pigumo42_font_name);

	int alpha[3];

	for (int i = 0; i < 3; i++) {
		if (selectNum_ == i) {
			alpha[i] = 250;
		}
		else {
			alpha[i] = 150;
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha[0]);
	DrawStringToHandle(Game::screen_width / 2 - windowModeFontSize / 2, Game::screen_height / 3, windowModeText_.c_str(), 0xffffff, pigumo42);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�~��`�悷��ʒu
	float circlePosX = Game::screen_width / 3 - circle_distance * 1.5f;

	//�g�k��
	float scale = 1.0f;

	//�F
	int color = 0xb1b3b6;

	//BGM�o�[
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha[1]);

	//���ʃo�[�摜�̕`��
	for (int i = 1; i < 11; i++) {
		scale = 1.0f;
		color = 0xb1b3b6;

		if (volumeBGM_ == i) {
			scale = 1.3f;
			color = 0xff0000;
		}

		DrawCircleAA(circlePosX + i * circle_distance, Game::screen_height / 2, circle_radius * scale, 32, color, true);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//SE�o�[
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha[2]);

	//���ʃo�[�摜�̕`��
	for (int i = 1; i < 11; i++) {
		scale = 1.0f;
		color = 0xb1b3b6;

		if (volumeSE_ == i) {
			scale = 1.3f;
			color = 0xff0000;
		}

		DrawCircleAA(circlePosX + i * circle_distance, Game::screen_height / 3 * 2, circle_radius * scale, 32, color, true);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetDrawScreen(DX_SCREEN_BACK);

	//�t�F�[�h�̎��A�A���t�@�l��ύX����ƂƂ��ɃK�E�X�ڂ������|����
	GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, 8, 255-fadeValue_);
	//�`��摜�̃A���t�@�l��ύX����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawGraph(0, 0, makeScreenHandle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//�ʏ펞�̍X�V
void SettingScene::NormalUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	if (input.IsTriggered(InputType::Up)) {
		selectNum_ = (std::max)(selectNum_ - 1, 0);
	}
	else if (input.IsTriggered(InputType::Down)) {
		selectNum_ = (std::min)(selectNum_ + 1, 4);
	}

	ChangeUpdateFunc();

	//�V�[���؂�ւ�
	if (input.IsTriggered(InputType::Pause)) {
		nextScene_ = std::make_shared<ScenePause>(manager_);
		updateFunc_ = &SettingScene::GaussFadeOutUpdate;
	}
}

//�K�E�X�������{�����t�F�[�h�C��
void SettingScene::GaussFadeInUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		updateFunc_ = &SettingScene::NormalUpdate;
		fadeValue_ = 255;
	}
}

//�K�E�X�������{�����t�F�[�h�A�E�g
void SettingScene::GaussFadeOutUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		manager_.SwapScene(std::shared_ptr<SceneBase>(nextScene_));
		fadeValue_ = 0;
		return;
	}
}

//BGM�̉��ʂ�ύX����
void SettingScene::BGMUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();
	auto& sound = SoundManager::GetInstance();

	//BGM���ʒ���
	if (input.IsTriggered(InputType::Left)) {
		volumeBGM_ = (max)(volumeBGM_ - 1, 1);
		sound.PlayBGM("checkSoundBGM");
	}
	if (input.IsTriggered(InputType::Right)) {
		volumeBGM_ = (min)(volumeBGM_ + 1, 10);
		sound.PlayBGM("checkSoundBGM");
	}

	//���ʂ̕ύX
	SoundManager::GetInstance().SetBGMVolume(volumeBGM_ * 25);
}

//SE�̉��ʂ�ύX����
void SettingScene::SEUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();
	auto& sound = SoundManager::GetInstance();

	//SE���ʒ���
	if (input.IsTriggered(InputType::Left)) {
		volumeSE_ = (max)(volumeSE_ - 1, 1);
		sound.PlaySE("checkSoundSE");
	}
	if (input.IsTriggered(InputType::Right)) {
		volumeSE_ = (min)(volumeSE_ + 1, 10);
		sound.PlaySE("checkSoundSE");
	}

	//���ʂ̕ύX
	SoundManager::GetInstance().SetSEVolume(volumeSE_ * 25);
}

//�E�B���h�E���[�h��ύX����
void SettingScene::ChangeWindowUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	if (input.IsTriggered(InputType::Left)) {
		windowModeText_ = "��  �E�B���h�E���[�h  ��";
		manager_.ChangeWindowMode(true);
	}
	if (input.IsTriggered(InputType::Right)) {
		windowModeText_ = "��  �t���X�N���[��  ��";
		manager_.ChangeWindowMode(false);
	}
}

//updateFunc�̒��g��ύX����
void SettingScene::ChangeUpdateFunc()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	switch (selectNum_) {
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
			nextScene_ = std::make_shared<KeyConfigScene>(manager_);
			updateFunc_ = &SettingScene::GaussFadeOutUpdate;
		}
		break;
	case 4:
		if (input.IsTriggered(InputType::Space)) {
			nextScene_ = std::make_shared<ScenePause>(manager_);
			updateFunc_ = &SettingScene::GaussFadeOutUpdate;
		}
		break;
	}
}
