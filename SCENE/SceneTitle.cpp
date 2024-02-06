#include "SceneTitle.h"
#include "GameMain.h"
#include "ScenePause.h"
#include "SceneManager.h"
#include "SelectChapterScene.h"
#include "SettingSceneForSceneTitle.h"

#include "../object/Camera.h"
#include "../object/LightBulb.h"
#include "../object/ObjectManager.h"

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
	auto& file  = ExternalFile::GetInstance();
	auto& model = ModelManager::GetInstance();

	//�C���X�^���X��
	UI_					= std::make_shared<UIItemManager>();
	objManager_			= std::make_shared<ObjectManager>();
	subPlayerModel_		= std::make_shared<Model>(model.GetModelHandle(ObjectType::Player),Material::Other);
	mainPlayerModel_	= std::make_shared<Model>(model.GetModelHandle(ObjectType::Player),Material::Other);
	camera_				= std::make_shared<Camera>(file.GetCameraTargetPos("start"),file.GetCameraTargetPos("startTargetPos"));
	lightBulb_			= std::make_shared<LightBulb>(model.GetModelHandle(ObjectType::LightBulb), file.GetSpecifiedInfo("title", "LightBulb"));

	//���C���v���C���[���f���̔z�u�f�[�^���Z�b�g����
	auto mainPlayerInfo = file.GetSpecifiedInfo("title", "Player");
	mainPlayerModel_->SetScale(mainPlayerInfo.scale);
	mainPlayerModel_->SetPos(mainPlayerInfo.pos);
	mainPlayerModel_->SetRot(mainPlayerInfo.rot);
	mainPlayerModel_->SetAnimation(static_cast<int>(PlayerAnimType::wakeUp), false, true);

	//�T�u�v���C���[���f���̔z�u�f�[�^���Z�b�g����
	auto subPlayerInfo = file.GetSpecifiedInfo("title", "SubPlayer");
	mainPlayerModel_->SetScale(subPlayerInfo.scale);
	mainPlayerModel_->SetPos(subPlayerInfo.pos);
	mainPlayerModel_->SetRot(subPlayerInfo.rot);
	mainPlayerModel_->SetAnimation(static_cast<int>(PlayerAnimType::jump), false, true);

	//UI��\��������W���擾
	menuDrawPos_["�^�C�g��"]			= file.GetUIPos("titleDrawPos");
	menuDrawPos_["�j���[�Q�[��"]		= file.GetUIPos("startDrawPos");
	menuDrawPos_["�Q�[�����ĊJ����"]	= file.GetUIPos("continueDrawPos");
	menuDrawPos_["�ݒ�"]				= file.GetUIPos("settingDrawPos");
	menuDrawPos_["���[�h"]				= file.GetUIPos("windowModeUIPos");
	menuDrawPos_["BGM"]					= file.GetUIPos("BGMUIPos");
	menuDrawPos_["SE"]					= file.GetUIPos("SEUIPos");
	menuDrawPos_["����ݒ�"]			= file.GetUIPos("advancedSettingUIPos");
	menuDrawPos_["�߂�"]				= file.GetUIPos("backUIPos");

	SetUseLighting(true);

	//�I�u�W�F�N�g�̐���
	objManager_->OpeningStageObjectGenerator();

	//�J�����̏�����
	camera_->Init(VGet(0, 140, 0));

	//�J�����̃|�W�V�����ƌ���ʒu�̐ݒ�
	camera_->SetCameraTargetPosAndView(file.GetCameraTargetPos("start"), file.GetCameraTargetPos("startTargetPos"),{0,1,0});

	outAngle_ = 90.0f * DX_PI_F / 180.0f;
	inAngle_ = 30.0f * DX_PI_F / 180.0f;
	//ChangeLightTypeSpot(lightBulb_->GetFramePos(), lightBulb_->GetFrontVec(), outAngle_, inAngle_, 1000.0f, 0.0f, 0.0006f, 0.0f);
	//ChangeLightTypePoint(lightBulb_->GetFramePos(), 500.0f, 0.0f, 0.0006f, 0.0f);

	//�^�C�g���摜�̓ǂݍ���
	titleHandle_ = LoadGraph("data/graph/title.png");

	//UI������̍쐬
	menuName_.push_back("�j���[�Q�[��");
	menuName_.push_back("�Q�[�����ĊJ����");
	menuName_.push_back("�ݒ�");
	menuName_.push_back("�I��");

	//UI�摜�̍쐬
	fontHandle_ = FontsManager::GetInstance().GetFontHandle("�s�O�� 0042");
	float y = 120.0f;
	for (auto& menu : menuName_) {
		UI_->AddMenu(static_cast<float>(Game::screen_width / 2), static_cast<float>(Game::screen_height / 2) + y, 320, 100, menu.c_str(), fontHandle_);
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

	//�d���̍X�V
	lightBulb_->Update();

	//���C�g�̍X�V
	//SetLightDirection(lightBulb_->GetFrontVec());
	//SetLightPosition(lightBulb_->GetFramePos());

	(this->*updateFunc_)();
}

void SceneTitle::Draw()
{
	//�Z�k��
	auto& input = InputState::GetInstance();
	auto& file = ExternalFile::GetInstance();

	//�J�����̏�����
	camera_->Init(VGet(0, 140, 0));

	//�I�u�W�F�N�g�̕`��
	objManager_->Draw({ 0,0,0 });

	lightBulb_->Draw();

	//���f���̕`��
	mainPlayerModel_->Draw();

	//UI�̕`��
	UI_->DrawBillBoard(menuDrawPos_,static_cast<float>(UIfadeValue_),200.0f);

	input.DrawKeyGraph(InputType::left, Game::screen_width / 6 * 4, Game::screen_height - 40, 0.8f);
	input.DrawKeyGraph(InputType::right, Game::screen_width / 7 * 5, Game::screen_height - 40, 0.8f);
	input.DrawKeyGraph(InputType::space, Game::screen_width / 6 * 5, Game::screen_height - 40, 0.8f);

	DrawStringToHandle(Game::screen_width / 4 * 3 - 25, Game::screen_height - 60, "�ړ�", 0xffffff, fontHandle_);
	input.DrawName(InputType::space, Game::screen_width / 14 * 12, Game::screen_height - 60, 0xffffff, fontHandle_, true, true, "/");

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
	mainPlayerModel_->Update();

	//�A�j���[�V�������I������
	if (mainPlayerModel_->IsAnimEnd()) {
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
