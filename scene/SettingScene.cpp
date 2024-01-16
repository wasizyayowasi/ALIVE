#include "SettingScene.h"
#include "SceneManager.h"
#include "ScenePause.h"
#include "KeyConfigScene.h"

#include "../util/SoundManager.h"
#include "../util/InputState.h"
#include "../util/game.h"
#include "../util/FontsManager.h"
#include "../util/UIItemManager.h"

#include <algorithm>

using namespace std;

namespace {
	//�t�H���g�̖��O
	const char* const pigumo42_font_name = "�s�O�� 0042";
	//�摜�t�@�C���p�X
	const char* const circle_white_graph_path = "data/graph/circleWhite.png";
	const char* const circle_trans_white_graph_path = "data/graph/circleTransWhite.png";
	const char* const pictgram_graph_path = "data/graph/pict.png";
	const char* const house_graph_path = "data/graph/house.png";
	//�ړ�����
	constexpr float pictgram_move_distance = 53.5f;

	constexpr float move_speed = 3.0f;
}

SettingScene::SettingScene(SceneManager& manager):SceneBase(manager),updateFunc_(&SettingScene::GaussFadeInUpdate)
{
}

SettingScene::~SettingScene()
{
}

void SettingScene::Init()
{
	//�摜�̓ǂݍ���
	circleWhiteHandle_ = LoadGraph(circle_white_graph_path);
	circleTransWhiteHandle_ = LoadGraph(circle_trans_white_graph_path);
	pictogramHandle_ = LoadGraph(pictgram_graph_path);
	houseHandle_ = LoadGraph(house_graph_path);

	//���݂̃{�����[���̎擾
	volumeBGM_ = SoundManager::GetInstance().GetBGMVolume();
	volumeSE_ = SoundManager::GetInstance().GetSEVolume();

	int BGMPictPos = static_cast<int>((static_cast<float>(volumeBGM_) / 250.0f) * 10);
	int SEPictPos = static_cast<int>((static_cast<float>(volumeSE_) / 250.0f) * 10);

	BGMPictogramPosX_ = Game::screen_width / 3 + BGMPictPos * pictgram_move_distance;
	SEPictogramPosX_ = Game::screen_width / 3 + SEPictPos * pictgram_move_distance;

	UIManager_ = std::make_shared<UIItemManager>();

	//UI�摜�̍쐬
	//�t�H���g�̎擾
	int pigumo42Font = FontsManager::GetInstance().GetFontHandle(pigumo42_font_name);
	//�t�H���g��K�p����������̃T�C�Y�擾
	int windowFontSize = FontsManager::GetInstance().GetStringSize("���[�h", pigumo42_font_name);
	int BGMFontSize = FontsManager::GetInstance().GetStringSize("BGM", pigumo42_font_name);
	int SEFontSize = FontsManager::GetInstance().GetStringSize("SE", pigumo42_font_name);

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

void SettingScene::End()
{
	DeleteGraph(circleWhiteHandle_);
	DeleteGraph(circleTransWhiteHandle_);
	DeleteGraph(pictogramHandle_);
	DeleteGraph(makeScreenHandle_);
}

void SettingScene::Update()
{
	(this->*updateFunc_)();
}

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

	time_++;

	DrawFormatString(0, 16, 0xffffff, "%.2f", BGMPictogramPosX_);
	DrawFormatString(0, 32, 0xffffff, "%.2f", SEPictogramPosX_);

	//���݂̉�ʃ��[�h��\��
	int pigumo42 = FontsManager::GetInstance().GetFontHandle(pigumo42_font_name);
	int windowModeFontSize = FontsManager::GetInstance().GetStringSize(windowModeText_.c_str(), pigumo42_font_name);

	int alpha = 150;

	if (selectNum_ == 0) {
		alpha = 250;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawStringFToHandle(Game::screen_width / 2 - windowModeFontSize / 2, Game::screen_height / 3, windowModeText_.c_str(), 0xffffff, pigumo42);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	if (selectNum_ == 1) {
		alpha = 250;
	}
	else {
		alpha = 150;
	}
	//BGM�o�[
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawRotaGraph(Game::screen_width / 3 - 13.0f, Game::screen_height / 2, 1.0f, 0.0f, houseHandle_, true, false);
	int BGMPictPos = static_cast<int>((static_cast<float>(volumeBGM_) / 250.0f) * 10);
	//���ʃo�[�摜
	for (int i = 0; i < 10; i++) {
		if (BGMPictPos > i) {
			DrawRotaGraphF(Game::screen_width / 3 + pictgram_move_distance + i * 53.5f, Game::screen_height / 2, 1.0, 0, circleWhiteHandle_, true, false);
		}
		else {
			DrawRotaGraphF(Game::screen_width / 3 + pictgram_move_distance + i * 53.5f, Game::screen_height / 2, 1.0, 0, circleTransWhiteHandle_, true, false);
		}
		
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	if (selectNum_ == 2) {
		alpha = 250;
	}
	else {
		alpha = 150;
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawRotaGraph(Game::screen_width / 3 - 13.0f, Game::screen_height / 3 * 2.0f, 1.0f, 0.0f, houseHandle_, true, false);
	int SEPictPos = static_cast<int>((static_cast<float>(volumeSE_) / 250.0f) * 10);
	for (int i = 0; i < 10; i++) {
		if (SEPictPos > i) {
			DrawRotaGraphF(Game::screen_width / 3 + pictgram_move_distance + i * 53.5f, Game::screen_height / 3 * 2.0f, 1.0, 0, circleWhiteHandle_, true, false);
		}
		else {
			DrawRotaGraphF(Game::screen_width / 3 + pictgram_move_distance + i * 53.5f, Game::screen_height / 3 * 2.0f, 1.0, 0, circleTransWhiteHandle_, true, false);
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�s�N�g�O����
	DrawRotaGraph(BGMPictogramPosX_, Game::screen_height / 2, 0.3f, BGMPictogramRot_, pictogramHandle_, true, BGMPictogramTransInversion_);
	DrawRotaGraph(SEPictogramPosX_, Game::screen_height / 3 * 2, 0.3f, SEPictogramRot_, pictogramHandle_, true, SEPictogramTransInversion_);

	DrawLine(Game::screen_width / 2, 0, Game::screen_width / 2, Game::screen_height, 0xff0000);

	SetDrawScreen(DX_SCREEN_BACK);

	//�t�F�[�h�̎��A�A���t�@�l��ύX����ƂƂ��ɃK�E�X�ڂ������|����
	GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, 8, 255-fadeValue_);
	//�`��摜�̃A���t�@�l��ύX����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawGraph(0, 0, makeScreenHandle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SettingScene::NormalUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	if (input.IsTriggered(InputType::up)) {
		selectNum_ = (std::max)(selectNum_ - 1, 0);
	}
	else if (input.IsTriggered(InputType::down)) {
		selectNum_ = (std::min)(selectNum_ + 1, 4);
	}

	ChangeUpdateFunc();

	//�V�[���؂�ւ�
	if (input.IsTriggered(InputType::pause)) {
		nextScene_ = std::make_shared<ScenePause>(manager_);
		updateFunc_ = &SettingScene::GaussFadeOutUpdate;
	}
}

void SettingScene::GaussFadeInUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		updateFunc_ = &SettingScene::NormalUpdate;
		fadeValue_ = 255;
	}
}

void SettingScene::GaussFadeOutUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		manager_.SwapScene(std::shared_ptr<SceneBase>(nextScene_));
		fadeValue_ = 0;
		return;
	}
}

void SettingScene::BGMUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//BGM���ʒ���
	if (input.IsTriggered(InputType::left)) {
		volumeBGM_ = (max)(volumeBGM_ - 25, 0);
	}
	if (input.IsTriggered(InputType::right)) {
		volumeBGM_ = (min)(volumeBGM_ + 25, 250);
	}

	int BGMPictPos = static_cast<int>((static_cast<float>(volumeBGM_) / 250.0f) * 10);

	MovePictogram(BGMPictPos, BGMPictogramPosX_, BGMPictogramRot_, BGMPictogramTransInversion_);

	//���ʂ̕ύX
	SoundManager::GetInstance().SetBGMVolume(volumeBGM_);
}

void SettingScene::SEUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//SE���ʒ���
	if (input.IsTriggered(InputType::left)) {
		volumeSE_ = (max)(volumeSE_ - 25, 0);
	}
	if (input.IsTriggered(InputType::right)) {
		volumeSE_ = (min)(volumeSE_ + 25, 250);
	}

	int SEPictPos = static_cast<int>((static_cast<float>(volumeSE_) / 250.0f) * 10);

	MovePictogram(SEPictPos, SEPictogramPosX_, SEPictogramRot_, SEPictogramTransInversion_);

	//���ʂ̕ύX
	SoundManager::GetInstance().SetSEVolume(volumeSE_);
}

void SettingScene::ChangeWindowUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	if (input.IsTriggered(InputType::left)) {
		windowModeText_ = "��  �E�B���h�E���[�h  ��";
		manager_.ChangeWindowMode(true);
	}
	if (input.IsTriggered(InputType::right)) {
		windowModeText_ = "��  �t���X�N���[��  ��";
		manager_.ChangeWindowMode(false);
	}
}

void SettingScene::MovePictogram(int pictPos, float& pos, float& rot, bool& inversion)
{
	float targetPos = Game::screen_width / 3 + pictPos * pictgram_move_distance;
	float distance = targetPos - pos;

	if (distance > 0) {
		pos = (std::min)(pos + move_speed, targetPos);
		inversion = false;
		if (time_ % 4 == 0) {
			rot = -rot;
		}
	}
	else if(distance < 0) {
		pos = (std::max)(pos - move_speed, targetPos);
		inversion = true;
		if (time_ % 4 == 0) {
			rot = -rot;
		}
	}

	if (time_ > 60) {
		time_ = 0;
	}

}

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
		if (input.IsTriggered(InputType::space)) {
			nextScene_ = std::make_shared<KeyConfigScene>(manager_);
			updateFunc_ = &SettingScene::GaussFadeOutUpdate;
		}
		break;
	case 4:
		if (input.IsTriggered(InputType::space)) {
			nextScene_ = std::make_shared<ScenePause>(manager_);
			updateFunc_ = &SettingScene::GaussFadeOutUpdate;
		}
		break;
	}
}
