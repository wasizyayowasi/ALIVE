#include "SceneTitle.h"
#include "GameMain.h"
#include "ScenePause.h"
#include "SceneManager.h"
#include "SelectChapterScene.h"
#include "SettingSceneForSceneTitle.h"

#include "../object/ObjectManager.h"
#include "../object/Camera.h"

#include "../util/game.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/FontsManager.h"
#include "../util/SoundManager.h"
#include "../util/ExternalFile.h"
#include "../util/ModelManager.h"
#include "../util/UIItemManager.h"

#include <algorithm>

namespace {
	const char* const enemy_model_Filename = "data/player/mv1/player.mv1";
}

SceneTitle::SceneTitle(SceneManager& manager): SceneBase(manager)
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();
	auto& model = ModelManager::GetInstance();

	//�C���X�^���X��
	UI_ = std::make_shared<UIItemManager>();
	objManager_ = std::make_shared<ObjectManager>();
	playerModel_ = std::make_shared<Model>(model.GetModelHandle(ObjectType::Player),Material::Other);
	camera_ = std::make_shared<Camera>(file.GetCameraTargetPos("start"),file.GetCameraTargetPos("startTargetPos"));

	//�v���C���[���f���̔z�u�f�[�^���Z�b�g����
	auto info = ExternalFile::GetInstance().GetSpecifiedInfo("title", "Player");
	playerModel_->SetScale(info.scale);
	playerModel_->SetPos(info.pos);
	playerModel_->SetRot(info.rot);
	playerModel_->SetAnimation(static_cast<int>(PlayerAnimType::wakeUp), false, true);

	menuDrawPos_["�^�C�g��"] = file.GetUIPos("titleDrawPos");
	menuDrawPos_["�j���[�Q�[��"] = file.GetUIPos("startDrawPos");
	menuDrawPos_["�Q�[�����ĊJ����"] = file.GetUIPos("continueDrawPos");
	menuDrawPos_["�ݒ�"] = file.GetUIPos("settingDrawPos");
	menuDrawPos_["���[�h"] = file.GetUIPos("windowModeUIPos");
	menuDrawPos_["BGM"] = file.GetUIPos("BGMUIPos");
	menuDrawPos_["SE"] = file.GetUIPos("SEUIPos");
	menuDrawPos_["����ݒ�"] = file.GetUIPos("advancedSettingUIPos");
	menuDrawPos_["�߂�"] = file.GetUIPos("backUIPos");

	//�I�u�W�F�N�g�̐���
	objManager_->OpeningStageObjectGenerator();

	//�J�����̏�����
	camera_->Init(VGet(0, 140, 0));

	camera_->SetCameraTargetPosAndView(file.GetCameraTargetPos("start"), file.GetCameraTargetPos("startTargetPos"),{0,1,0});

	//���Ń��C�g�����������Ă���
	SetUseLighting(true);

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
		UI_->AddMenu(static_cast<float>(Game::screen_width / 2), static_cast<float>(Game::screen_height / 2) + y, 320, 100, menu.c_str(), font);
		y += 40.0f;
	}
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	updateFunc_ = &SceneTitle::FadeInUpdate;

	fadeTimer_ = 0;
	fadeValue_ = 0;
}

void SceneTitle::End()
{
}

void SceneTitle::Update()
{
	//�J�����̍X�V
	camera_->EasingMoveCamera();

	(this->*updateFunc_)();
}

void SceneTitle::Draw()
{
	auto& file = ExternalFile::GetInstance();

	//�J�����̏�����
	camera_->Init(VGet(0, 140, 0));

	//�I�u�W�F�N�g�̕`��
	objManager_->Draw({ 0,0,0 });

	//���f���̕`��
	playerModel_->Draw();

	//UI�̕`��
	UI_->DrawBillBoard(menuDrawPos_,static_cast<float>(UIfadeValue_),200.0f);

	//fadeValue_�̒l�ɂ���ē��ߋ���ω�����^�C�g���̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, UIfadeValue_);
	DrawBillboard3D(menuDrawPos_["�^�C�g��"], 0.5f, 0.5f, 300.0f, 0.0f, titleHandle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�����t�F�[�h�pbox�̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneTitle::FadeInUpdate()
{
	//�t�F�[�h�C��
	float timer = (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_));
	UIfadeValue_ = static_cast <int>(255 * timer);
	if (++fadeTimer_ == fadeInterval_) {
		updateFunc_ = &SceneTitle::UIUpdate;
		UIfadeValue_ = 255;
		return;
	}
}

void SceneTitle::UIUpdate()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();
	auto& input = InputState::GetInstance();

	//�I��
	if (input.IsTriggered(InputType::left)) {
		selectNum_ = (std::max)(selectNum_ - 1, 0);
		CameraSetting();
	}
	if (input.IsTriggered(InputType::right)) {
		selectNum_ = (std::min)(selectNum_ + 1, static_cast<int>(menuName_.size() - 1));
		CameraSetting();
	}

	//����
	if (input.IsTriggered(InputType::space)) {
		if (!camera_->GetMoving()) {
			updateFunc_ = &SceneTitle::UIFadeOutUpdate;
		}
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
			sound.PlaySE("stopAlarm");
			updateFunc_ = &SceneTitle::OpeningUpdate;
		}
	}
}

void SceneTitle::OpeningUpdate()
{
	//���f���̕`��
	playerModel_->Update();

	//�A�j���[�V�������I������
	if (playerModel_->IsAnimEnd()) {
		updateFunc_ = &SceneTitle::SceneTitleFadeOutUpdate;
	}
}

void SceneTitle::UIFadeOutUpdate()
{
	//UI�̃t�F�[�h�A�E�g
	UIfadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		UIfadeValue_ = 0;
		SceneChange();
		return;
	}
}

void SceneTitle::SceneTitleFadeOutUpdate()
{
	//�t�F�[�h�A�E�g
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_)));
		return;
	}
}

void SceneTitle::SceneChange()
{
	switch (selectNum_) {
	case 0:
		Init();
		manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<SettingSceneForSceneTitle>(manager_)));
		break;
	case 1:
		ExternalFile::GetInstance().ClearSaveData();
		updateFunc_ = &SceneTitle::OpeningSoundUpdate;
		SoundManager::GetInstance().PlaySE("alarm");
		break;
	case 2:
		manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<SelectChapterScene>(manager_)));
		break;
	case 3:
		manager_.SetEndFlag(true);
		break;
	}
}

void SceneTitle::CameraSetting()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//�ڕW���W
	VECTOR targetPos = {};

	//����ڕW���W
	VECTOR targetViewPos = {};

	switch (selectNum_) {
	case 0:
		targetPos = file.GetCameraTargetPos("setting");
		targetViewPos = file.GetCameraTargetPos("settingTargetPos");
		camera_->SetCameraTargetPosAndView(targetPos, targetViewPos, { 0,1,0 });

		break;
	case 1:
		targetPos = file.GetCameraTargetPos("start");
		targetViewPos = file.GetCameraTargetPos("startTargetPos");
		camera_->SetCameraTargetPosAndView(targetPos, targetViewPos, { 0,1,0 });

		break;
	case 2:
		targetPos = file.GetCameraTargetPos("continue");
		targetViewPos = file.GetCameraTargetPos("continueTargetPos");
		camera_->SetCameraTargetPosAndView(targetPos, targetViewPos, { 0,0,1 });

		break;
	}
}
