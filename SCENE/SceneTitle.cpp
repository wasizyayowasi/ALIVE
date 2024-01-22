#include "SceneTitle.h"
#include "GameMain.h"
#include "ScenePause.h"
#include "SceneManager.h"

#include "../object/ObjectManager.h"
#include "../object/Camera.h"

#include "../util/game.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/FontsManager.h"
#include "../util/SoundManager.h"
#include "../util/ExternalFile.h"
#include "../util/UIItemManager.h"

#include <DxLib.h>
#include <algorithm>

namespace {
	const char* const enemy_model_Filename = "data/player/mv1/player.mv1";
}

SceneTitle::SceneTitle(SceneManager& manager): SceneBase(manager)
{
	//�C���X�^���X��
	playerModel_ = std::make_shared<Model>(enemy_model_Filename);
	UI_ = std::make_shared<UIItemManager>();
	objManager_ = std::make_shared<ObjectManager>();
	camera_ = std::make_shared<Camera>(VGet(0, 140, -370));

	//���f���̔z�u�f�[�^���Z�b�g����
	auto info = ExternalFile::GetInstance().GetSpecifiedInfo("title", "Player");
	playerModel_->SetScale(info.scale);
	playerModel_->SetPos(info.pos);
	playerModel_->SetRot(info.rot);
	playerModel_->SetAnimation(static_cast<int>(PlayerAnimType::wakeUp2), false, true);

	//�I�u�W�F�N�g�̐���
	objManager_->OpeningStageObjectGenerator();

	//�J�����̏�����
	camera_->Init(VGet(0, 140, 0));

	//���Ń��C�g�����������Ă���
	//SetUseLighting(false);

	//�^�C�g���摜�̓ǂݍ���
	titleHandle_ = LoadGraph("data/graph/title.png");

	//UI������̍쐬
	menuName_.push_back("�j���[�Q�[��");
	menuName_.push_back("�Q�[�����ĊJ����");
	menuName_.push_back("�ݒ�");
	menuName_.push_back("�I��");

	//UI�摜�̍쐬
	int font = FontsManager::GetInstance().GetFontHandle("�s�O�� 0042");
	float y = 120.0f;
	for (auto& menu : menuName_) {
		UI_->AddMenu(Game::screen_width / 2, Game::screen_height / 2 + y, 320, 100, menu.c_str(), font);
		y += 40.0f;
	}
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	updateFunc_ = &SceneTitle::FadeInUpdate;
	drawFunc_ = &SceneTitle::UIDraw;

	int fadeTimer_ = 0;
	int fadeValue_ = 0;
}

void SceneTitle::End()
{
}

void SceneTitle::Update()
{
	(this->*updateFunc_)();
}

void SceneTitle::Draw()
{
	(this->*drawFunc_)();
}

void SceneTitle::FadeInUpdate()
{
	//�t�F�[�h�C��
	float timer = (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_));
	fadeValue_ = static_cast <int>(255 * timer);
	if (++fadeTimer_ == fadeInterval_) {
		updateFunc_ = &SceneTitle::UIUpdate;
		return;
	}
}

void SceneTitle::UIUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�I��
	if (input.IsTriggered(InputType::up)) {
		selectNum_ = (std::max)(selectNum_ - 1, 0);
	}
	if (input.IsTriggered(InputType::down)) {
		selectNum_ = (std::min)(selectNum_ + 1, static_cast<int>(menuName_.size() - 1));
	}

	//����
	if (input.IsTriggered(InputType::space)) {
		updateFunc_ = &SceneTitle::UIFadeOutUpdate;
	}
}

void SceneTitle::OpeningSoundUpdate()
{
	//�Z�k��
	auto& sound = SoundManager::GetInstance();
	int notPlaying = static_cast<int>(SoundPlay::notPlaying);

	//�T�E���h����I�������
	if (sound.CheckSoundFile("alarm") == notPlaying) {
		if (sound.CheckSoundFile("stopAlarm") == notPlaying) {
			sound.Play("stopAlarm");
			drawFunc_ = &SceneTitle::OpeningDraw;
			updateFunc_ = &SceneTitle::OpeningFadeInUpdate;
			fadeValue_ = 255;
			fadeTimer_ = fadeInterval_ * 4;
		}
	}
}

void SceneTitle::OpeningFadeInUpdate()
{
	//�J�����̍X�V
	camera_->OpeningCameraUpdate();

	//�t�F�[�h�C��
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		fadeValue_ = 0;
		updateFunc_ = &SceneTitle::OpeningUpdate;
		return;
	}
}

void SceneTitle::OpeningUpdate()
{
	//���f���̕`��
	playerModel_->Update();

	//�J�����̍X�V
	camera_->OpeningCameraUpdate();

	//�A�j���[�V�������I������
	if (playerModel_->IsAnimEnd()) {
		updateFunc_ = &SceneTitle::OpeningFadeOutUpdate;
	}

}

void SceneTitle::UIFadeOutUpdate()
{
	//�t�F�[�h�A�E�g
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		SceneChange();
		fadeValue_ = 0;
		return;
	}
}

void SceneTitle::OpeningFadeOutUpdate()
{
	//�t�F�[�h�A�E�g
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_)));
		return;
	}
}

void SceneTitle::UIDraw()
{
	//UI�̕`��
	UI_->AlphaChangeDraw(selectNum_, fadeValue_);

	//fadeValue_�̒l�ɂ���ē��ߋ���ω�����^�C�g���̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawRotaGraph(Game::screen_width / 2, Game::screen_height / 3, 1.0f, 0.0f, titleHandle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneTitle::OpeningDraw()
{
	//�I�u�W�F�N�g�̕`��
	objManager_->Draw({0,0,0});

	//���f���̕`��
	playerModel_->Draw();


	//fadeValue_�̒l�ɂ���ē��ߋ���ω�����w�i�̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void SceneTitle::SceneChange()
{
	switch (selectNum_) {
	case 0:
		ExternalFile::GetInstance().ClearSaveData();
		updateFunc_ = &SceneTitle::OpeningSoundUpdate;
		SoundManager::GetInstance().Play("alarm");
		break;
	case 1:
		ExternalFile::GetInstance().LoadSaveData();
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_)));
		break;
	case 2:
		Init();
		manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
		break;
	case 3:
		manager_.SetEndFlag(true);
		break;
	}
}
