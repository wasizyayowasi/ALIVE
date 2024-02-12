#include "GameMain.h"
#include "GameEnd.h"
#include "ScenePause.h" 
#include "SceneManager.h"

#include "../gimmick/Switch.h"
#include "../gimmick/GimmickBase.h"

#include "../object/Player.h"
#include "../object/Camera.h"
#include "../object/ShotManager.h"
#include "../object/CharacterBase.h"

#include "../staging/Broom.h"
#include "../staging/DepthOfField.h"

#include "../util/game.h"
#include "../util/model.h"
#include "../util/Tutorial.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/EffectManager.h"
#include "../util/CheckCollisionModel.h"

#include "../object/ObjectManager.h"
#include "../object/ObjectData.h"

GameMain::GameMain(SceneManager& manager) : SceneBase(manager),updateFunc_(&GameMain::FadeInUpdate)
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//�C���X�^���X��
	player_ = std::make_shared<Player>(file.GetSpecifiedInfo("main", "Player"));
	tutorial_ = std::make_shared<Tutorial>();
	shotManager_ = std::make_shared<ShotManager>();
	objManager_ = std::make_shared<ObjectManager>();
	checkCollisionModel_ = std::make_shared<CheckCollisionModel>();

	VECTOR pos = VAdd(player_->GetStatus().pos, VGet(0, 300, -550));
	camera_ = std::make_shared<Camera>(pos, player_->GetStatus().pos);

	//�Q�[���I�u�W�F�N�g�̐���
	objManager_->MainStageObjectGenerator();
}

GameMain::~GameMain()
{
	//�Q�[�����C�����I���Ƃ��Ƀv���C���Ɏ��񂾉񐔂�
	//saveData�ɋL�^����Ă��鎀�S�񐔂𑫂�
	ExternalFile::GetInstance().SaveDataRewriteInfo(totalDeathNum_);
}

void GameMain::Init()
{
	makeScreenHandle_ = MakeScreen(Game::screen_width, Game::screen_height, true);

	//���Ń��C�g�����������Ă���
	SetUseLighting(false);

	//1m�͈̔͂�ݒ肷��
	Set3DSoundOneMetre(10.0f);

	skyHandle_ = MV1LoadModel("data/model/skyDorm/SkyDorm.mv1");
	float scale = 30.0f;
	MV1SetScale(skyHandle_, VGet(scale, scale, scale));
	MV1SetPosition(skyHandle_, VGet(0, 200, 0));
}

void GameMain::End()
{
	DeleteGraph(makeScreenHandle_);
	MV1DeleteModel(skyHandle_);
}

//�X�V
void GameMain::Update()
{
	(this->*updateFunc_)();
}

//�`��
void GameMain::Draw()
{
	SetDrawScreen(makeScreenHandle_);
	ClearDrawScreen();

	//�J�����̏�����
	//SetDrawScreen���s���ƃJ�����̏�񂪃��Z�b�g����邽�߂�
	camera_->Init(camera_->GetTarget());
	camera_->Update(player_->GetStatus().pos, player_->GetStatus().height);

	//�v���C���[�̕`��
	player_->Draw();

	//�I�u�W�F�N�g�̕`��
	objManager_->Draw(player_->GetStatus().pos);

	//�G�t�F�N�g�̕`��
	EffectManager::GetInstance().Draw();

	//�X�J�C�h�[���̕`��
	MV1SetPosition(skyHandle_, player_->GetStatus().pos);
	MV1DrawModel(skyHandle_);

#ifdef _DEBUG
	camera_->DebugDraw();
#endif // _DEBUG

	//�e�̕`��
	shotManager_->Draw();

	//�`���[�g���A���̕`��
	tutorial_->Draw();

#ifdef _DEBUG
	VECTOR pos = player_->GetStatus().pos;
	DrawFormatString(0, 48, 0xffffff, "%.2f,%.2f,%.2f", pos.x, pos.y, pos.z);
#endif // _DEBUG

	SetDrawScreen(DX_SCREEN_BACK);

	//�t�B���^�[�������s����
	if (isFilterOn_) {
		GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, 32, 800);
	}
	//makescreenHandle�ɏ������񂾓��e��`�悷��
	DrawGraph(0, 0, makeScreenHandle_, true);

	//��ʑS�̂�^�����ɓh��Ԃ�
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//TODO�F�ʂ̃t�F�[�h�C�����o���������
void GameMain::FadeInUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &GameMain::NormalUpdate;
		fadeValue_ = 0;
	}
}

//�X�V
void GameMain::NormalUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�t�B���^�[�������s��Ȃ��p�ɂ���
	isFilterOn_ = false;

	//�v���C���[�̍X�V
	player_->Update(objManager_);

	//�I�u�W�F�N�g�̍X�V
	objManager_->Update(*player_,shotManager_);

	//�G�t�F�N�g�̍X�V
	EffectManager::GetInstance().Update();

	//�v���C���[�Ƃ��̑��I�u�W�F�N�g�Ƃ̏Փ˔���
	checkCollisionModel_->CheckCollision(player_,objManager_);

	//�e�̍X�V
	shotManager_->Update();

	//�e�ƃv���C���[�̏Փ˔���
	shotManager_->Hit(*player_);

	//�`���[�g���A��
	tutorial_->Update(player_->GetStatus().pos);

	//���X�i�[�̈ʒu�ƕ�����ݒ�
	//����́A�v���C���[�ł͂Ȃ��J�����̍��W�ɂ��Ă���
	SoundManager::GetInstance().Set3DSoundListenerInfo(camera_->GetPos(), camera_->GetTarget());

	//�|�[�Y�V�[�����J��
	if (input.IsTriggered(InputType::Pause)) {
		isFilterOn_ = true;
		manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
	}

	auto info = ExternalFile::GetInstance().GetSpecifiedInfo("main","GOAL");

	auto result = MV1CollCheck_Capsule(	player_->GetModelPointer()->GetModelHandle(),
										player_->GetModelPointer()->GetColFrameIndex(),
										VGet(info.pos.x, info.pos.y - info.scale.y, info.pos.z), 
										VGet(info.pos.x, info.pos.y + info.scale.y, info.pos.z), 600);

	if (result.HitNum > 0) {
		totalDeathNum_ += player_->GetDeathCount();
		ExternalFile::GetInstance().SetDeathCount(totalDeathNum_);
		updateFunc_ = &GameMain::FadeOutUpdate;
	}

	MV1CollResultPolyDimTerminate(result);
}

//TODO�F�ʂ̃t�F�[�h�C�����o���������
void GameMain::FadeOutUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameEnd>(manager_)));
		return;
	}
}
