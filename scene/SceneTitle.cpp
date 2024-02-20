#include "SceneTitle.h"
#include "GameMain.h"
#include "ScenePause.h"
#include "SceneMovie.h"
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
#include "../util/GraphManager.h"
#include "../util/UIItemManager.h"

#include <algorithm>

//�R���X�g���N�^
SceneTitle::SceneTitle(SceneManager& manager): SceneBase(manager)
{
	//�Z�k��
	auto& file  = ExternalFile::GetInstance();
	auto& model = ModelManager::GetInstance();

	//�C���X�^���X��
	UI_					= std::make_shared<UIItemManager>();
	objManager_			= std::make_shared<ObjectManager>();
	subPlayerModel_		= std::make_shared<Model>(model.GetModelHandle(ObjectName[static_cast<int>(ObjectType::Player)]),Material::Other);
	mainPlayerModel_	= std::make_shared<Model>(model.GetModelHandle(ObjectName[static_cast<int>(ObjectType::Player)]),Material::Other);
	camera_				= std::make_shared<Camera>(file.GetCameraTargetPos("start"),file.GetCameraTargetPos("startTargetPos"));
	lightBulb_			= std::make_shared<LightBulb>(model.GetModelHandle(ObjectName[static_cast<int>(ObjectType::LightBulb)]), file.GetSpecifiedInfo("title", "LightBulb"));

	//���C���v���C���[���f���̔z�u�f�[�^���Z�b�g����
	auto mainPlayerInfo = file.GetSpecifiedInfo("title", "Player");
	mainPlayerModel_->SetScale(mainPlayerInfo.scale);
	mainPlayerModel_->SetPos(mainPlayerInfo.pos);
	mainPlayerModel_->SetRot(mainPlayerInfo.rot);
	mainPlayerModel_->SetAnimation(static_cast<int>(PlayerAnimType::StopTimer), false, true);

	//�T�u�v���C���[���f���̔z�u�f�[�^���Z�b�g����
	auto subPlayerInfo = file.GetSpecifiedInfo("title", "SubPlayer");
	subPlayerModel_->SetScale(subPlayerInfo.scale);
	subPlayerModel_->SetPos(subPlayerInfo.pos);
	subPlayerModel_->SetRot(subPlayerInfo.rot);
	subPlayerModel_->SetAnimation(static_cast<int>(PlayerAnimType::Jump), false, true);

	//�I�u�W�F�N�g�̐���
	objManager_->OpeningStageObjectGenerator();

	//���C�g�̐ݒ�
	LightSetting();

	//�J�����̔z�u���̐ݒ�
	CameraSettingPos();

	//UI��\��������W���擾
	menuDrawPos_["SE"]				= file.GetUIPos("SEUIPos");
	menuDrawPos_["BGM"]				= file.GetUIPos("BGMUIPos");
	menuDrawPos_["�I��"]			= file.GetUIPos("endDrawPos");
	menuDrawPos_["�ݒ�"]			= file.GetUIPos("settingDrawPos");
	menuDrawPos_["�߂�"]			= file.GetUIPos("backUIPos");
	menuDrawPos_["���[�h"]			= file.GetUIPos("windowModeUIPos");
	menuDrawPos_["�^�C�g��"]		= file.GetUIPos("titleDrawPos");
	menuDrawPos_["����ݒ�"]		= file.GetUIPos("advancedSettingUIPos");
	menuDrawPos_["�V�[���I��"]		= file.GetUIPos("continueDrawPos");
	menuDrawPos_["�j���[�Q�[��"]	= file.GetUIPos("startDrawPos");

	//UI������̍쐬
	menuName_.push_back("�j���[�Q�[��");
	menuName_.push_back("�V�[���I��");
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

//�f�X�g���N�^
SceneTitle::~SceneTitle()
{
	for (auto& light : lightHandle_) {
		DeleteLightHandle(light);
	}
	DeleteLightHandleAll();
}

//������
void SceneTitle::Init()
{
	updateFunc_ = &SceneTitle::FadeInUpdate;

	fadeTimer_ = 0;
	fadeValue_ = 0;
}

//�I��
void SceneTitle::End()
{
}

//�X�V
void SceneTitle::Update()
{
	//�J�����̍X�V
	camera_->EasingMoveCamera();

	//�d���̍X�V
	lightBulb_->Update();

	//���C�g�̍X�V
	SetLightDirection(lightBulb_->GetFrontVec());
	SetLightPosition(lightBulb_->GetFramePos());

	//���C�g�̕����ݒ�
	SetLightDirectionHandle(lightHandle_[0], lightDir_[0]);
	SetLightDirectionHandle(lightHandle_[1], lightDir_[1]);

	(this->*updateFunc_)();
}

//�`��
void SceneTitle::Draw()
{
	//�Z�k��
	auto& input = InputState::GetInstance();
	auto& file = ExternalFile::GetInstance();

	//�J�����̏�����
	camera_->Init(VGet(0, 140, 0));

	//�I�u�W�F�N�g�̕`��
	objManager_->Draw({ 0,0,0 });

	//�d���̕`��
	lightBulb_->Draw();

	//���f���̕`��
	mainPlayerModel_->Draw();
	subPlayerModel_->Draw();

	//UI�̕`��
	UI_->DrawBillBoard(menuDrawPos_,UIfadeValue_,200.0f);

	if (input.LastInputDevice()) {
		//�L�[�摜�`��
		input.DrawKeyGraph(static_cast<int>(InputType::Up)	 , Game::screen_width / 128 * 92 , Game::screen_height - 90, 0.8f);
		input.DrawKeyGraph(static_cast<int>(InputType::Left) , Game::screen_width / 128 * 87 , Game::screen_height - 40, 0.8f);
		input.DrawKeyGraph(static_cast<int>(InputType::Down) , Game::screen_width / 128 * 92 , Game::screen_height - 40, 0.8f);
		input.DrawKeyGraph(static_cast<int>(InputType::Right), Game::screen_width / 128 * 97 , Game::screen_height - 40, 0.8f);
		input.DrawKeyGraph(static_cast<int>(InputType::Space), Game::screen_width / 128 * 112, Game::screen_height - 40, 0.8f);
	}
	else {
		input.DrawPadGraph(static_cast<int>(XboxBotton::Up)     , Game::screen_width / 128 * 92 , Game::screen_height - 90, 0.66f);
		input.DrawPadGraph(static_cast<int>(XboxBotton::Left)	, Game::screen_width / 128 * 87 , Game::screen_height - 40, 0.66f);
		input.DrawPadGraph(static_cast<int>(XboxBotton::Down)   , Game::screen_width / 128 * 92 , Game::screen_height - 40, 0.66f);
		input.DrawPadGraph(static_cast<int>(XboxBotton::Right)  , Game::screen_width / 128 * 97 , Game::screen_height - 40, 0.66f);
		input.DrawPadGraph(static_cast<int>(XboxBotton::A)      , Game::screen_width / 128 * 112, Game::screen_height - 40, 0.66f);
	}
	
	//������̕`��
	DrawStringToHandle(Game::screen_width / 10 * 8 - 25, Game::screen_height - 60, "�ړ�", 0xffffff, fontHandle_);
	input.DrawName(static_cast<int>(InputType::Space), Game::screen_width / 20 * 18, Game::screen_height - 60, 0xffffff, fontHandle_, true, true, "/");

	//fadeValue_�̒l�ɂ���ē��ߋ���ω�����^�C�g���̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, UIfadeValue_);
	DrawBillboard3D(menuDrawPos_["�^�C�g��"], 0.5f, 0.5f, 300.0f, 0.0f, GraphManager::GetInstance().GetGraph("title"), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�����t�F�[�h�pbox�̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//���C�g�̐ݒ�
void SceneTitle::LightSetting()
{
	SetUseLighting(true);

	//�X�|�b�g���C�g�ɕύX����B���C�g�̐ݒ�
	outAngle_ = 90.0f * DX_PI_F / 180.0f;
	inAngle_ = 30.0f * DX_PI_F / 180.0f;
	ChangeLightTypeSpot(lightBulb_->GetFramePos(), lightBulb_->GetFrontVec(), outAngle_, inAngle_, 600.0f, 0.0f, 0.003f, 0.0f);

	//���C�g�̃f�B�t���[�Y�J���[�̐ݒ�
	SetLightDifColor(GetColorF(0.87f, 0.72f, 0.52f, 1.0f));

	//�f�B���N�V�������C�g�p�x�擾
	VECTOR dir = {};
	dir.x = -33 / DX_PI_F * 180.0f;
	dir.y = -33 / DX_PI_F * 180.0f;
	dir.z = -33 / DX_PI_F * 180.0f;

	//�f�B���N�V�������C�g�̍쐬
	lightHandle_.push_back(CreateDirLightHandle(dir));
	lightDir_.push_back(dir);
	SetLightDifColorHandle(lightHandle_[0], GetColorF(0.41f, 0.41f, 0.41f, 1.0f));

	//�f�B���N�V�������C�g�p�x�擾
	dir.x = 33 / DX_PI_F * 180.0f;
	dir.y = -33 / DX_PI_F * 180.0f;
	dir.z = 33 / DX_PI_F * 180.0f;

	//�f�B���N�V�������C�g�̍쐬
	lightHandle_.push_back(CreateDirLightHandle(dir));
	lightDir_.push_back(dir);
	SetLightDifColorHandle(lightHandle_[1], GetColorF(0.41f, 0.41f, 0.41f, 1.0f));
}

//�J�����̐ݒ�
void SceneTitle::CameraSettingPos()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//�J�����̏�����
	camera_->Init(VGet(0, 140, 0));

	//�J�����̃|�W�V�����ƌ���ʒu�̐ݒ�
	camera_->SetCameraTargetPosAndView(file.GetCameraTargetPos("start"), file.GetCameraTargetPos("startTargetPos"), { 0,1,0 });

	//�J�������̐ݒ�
	CameraInfo info = {};

	//�ݒ�I�u�W�F�N�g������J����
	info.targetPos = file.GetCameraTargetPos("setting");
	info.targetView = file.GetCameraTargetPos("settingTargetPos");
	info.upVec = VGet(0, 1, 0);

	cameraInfo_.push_back(info);

	//�x�b�h������J����
	info.targetPos = file.GetCameraTargetPos("start");
	info.targetView = file.GetCameraTargetPos("startTargetPos");
	info.upVec = VGet(0, 1, 0);

	cameraInfo_.push_back(info);

	//�{������J����
	info.targetPos = file.GetCameraTargetPos("continue");
	info.targetView = file.GetCameraTargetPos("continueTargetPos");
	info.upVec = VGet(0, 0, 1);

	cameraInfo_.push_back(info);

	//��������J����
	info.targetPos = file.GetCameraTargetPos("end");
	info.targetView = file.GetCameraTargetPos("endTargetPos");
	info.upVec = VGet(0, 1, 0);

	cameraInfo_.push_back(info);
}

//�I��ԍ��̍X�V
void SceneTitle::SelectNumUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	bool isTriggerLeft = input.IsTriggered(InputType::Left);
	bool isTriggerRight = input.IsTriggered(InputType::Right);
	bool isTriggerUp = input.IsTriggered(InputType::Up);
	bool isTriggerDown = input.IsTriggered(InputType::Down);

	switch (selectNum_)
	{
	case 0:
		if (isTriggerRight || isTriggerDown)
		{
			oldSelectNum_ = selectNum_;
			selectNum_ = 1;
		}
		break;
	case 1:
		if (isTriggerUp)
		{
			oldSelectNum_ = selectNum_;
			selectNum_ = 2;
		}
		else if (isTriggerLeft)
		{
			oldSelectNum_ = selectNum_;
			selectNum_ = 0;
		}
		else if (isTriggerRight)
		{
			oldSelectNum_ = selectNum_;
			selectNum_ = 3;
		}
		break;
	case 2:
		if (isTriggerLeft || isTriggerDown) 
		{
			oldSelectNum_ = selectNum_;
			selectNum_ = 1;
		}
		break;
	case 3:
		if (isTriggerLeft || isTriggerDown) 
		{
			oldSelectNum_ = selectNum_;
			selectNum_ = 1;
		}
		break;
	}
}

//�t�F�[�h�C���̍X�V
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

//UI�̍X�V
void SceneTitle::UIUpdate()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();
	auto& input = InputState::GetInstance();

	if (camera_->GetMoving()) {
		return;
	}

	//�I��
	SelectNumUpdate();

	//���݂̃t���[���ƑO�̃t���[���Ŕԍ����������
	//�J�����̐ݒ���s��
	if (selectNum_ != oldSelectNum_) {
		camera_->SetCameraTargetPosAndView(cameraInfo_[selectNum_].targetPos, cameraInfo_[selectNum_].targetView, cameraInfo_[selectNum_].upVec);
		camera_->LesetElapsedTime();
	}

	//����
	if (input.IsTriggered(InputType::Space)) {
		updateFunc_ = &SceneTitle::UIFadeOutUpdate;
	}

	//������Đ�����
	if (input.IsTriggered(InputType::Pause)) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneMovie>(manager_)));
	}
}

//�I�[�v�j���O�̍X�V
void SceneTitle::OpeningUpdate()
{
	//�Z�k��
	auto& sound = SoundManager::GetInstance();

	//�A�j���[�V�����ԍ��`�F�b�N
	bool checkStopTimerAnim = PlayerAnimType::StopTimer == static_cast<PlayerAnimType>(mainPlayerModel_->GetCurrentAnimNo());
	bool checkStopTimerCancelAnim = PlayerAnimType::StopTimerCancel == static_cast<PlayerAnimType>(mainPlayerModel_->GetCurrentAnimNo());
	bool checkWakeUpAnim = PlayerAnimType::WakeUp == static_cast<PlayerAnimType>(mainPlayerModel_->GetCurrentAnimNo());

	//���f���̍X�V
	mainPlayerModel_->Update();

	//���v���Ƃ߂�A�j���[�V�����̌�����ɖ߂�
	if (mainPlayerModel_->IsAnimEnd() && checkStopTimerAnim) {
		sound.StopSE("alarm");
		sound.PlaySE("stopAlarm");
		mainPlayerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::StopTimerCancel), false, false, 10);
	}

	//�����ɂȂ�A�j���[�V�������I�������
	if (mainPlayerModel_->IsAnimEnd() && checkStopTimerCancelAnim) {
		mainPlayerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::WakeUp), false, false, 10);
	}

	//�N���オ��A�j���[�V�������I�������
	if (mainPlayerModel_->IsAnimEnd() && checkWakeUpAnim) {
		updateFunc_ = &SceneTitle::SceneTitleFadeOutUpdate;
	}
}

//UI�̃t�F�[�h�A�E�g
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

//�^�C�g���̃t�F�[�h�A�E�g
void SceneTitle::SceneTitleFadeOutUpdate()
{
	//�t�F�[�h�A�E�g
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_)));
		return;
	}
}

//�V�[����؂�ւ���
void SceneTitle::SceneChange()
{
	switch (selectNum_) {
	case 0:
		Init();
		manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<SettingSceneForSceneTitle>(manager_)));
		break;
	case 1:
		updateFunc_ = &SceneTitle::OpeningUpdate;
		SoundManager::GetInstance().PlaySE("alarm");
		break;
	case 2:
		Init();
		manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<SelectChapterScene>(manager_)));
		break;
	case 3:
		manager_.SetEndFlag(true);
		break;
	}
}
