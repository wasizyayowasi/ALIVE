#include "GameMain.h"
#include "SceneManager.h"
#include "GameEnd.h"

#include "../gimmick/GimmickBase.h"
#include "../gimmick/Switch.h"
#include "../gimmick/Steelyard.h"

#include "../object/Player.h"
#include "../object/Camera.h"
#include "../object/CharacterBase.h"
#include "../object/ShotManager.h"

#include "../staging/Broom.h"
#include "../staging/DepthOfField.h"

#include "ScenePause.h" 

#include "../util/game.h"
#include "../util/SoundManager.h"
#include "../util/InputState.h"
#include "../util/model.h"
#include "../util/ExternalFile.h"
#include "../util/CheckCollisionModel.h"
#include "../util/Tutorial.h"

#include "../util/ObjectManager.h"
#include "../util/ObjectData.h"

using namespace std;

namespace {
	const char* const enemy_Filename = "data/player/player16.mv1";
}

GameMain::GameMain(SceneManager& manager) : SceneBase(manager),
updateFunc_(&GameMain::NormalUpdate)
{
}

GameMain::~GameMain()
{
	//�Q�[�����C�����I���Ƃ��Ƀv���C���Ɏ��񂾉񐔂�
	//saveData�ɋL�^����Ă��鎀�S�񐔂𑫂�
	totalDeathNum_ += player_->GetDeathCount();
	ExternalFile::GetInstance().SaveDataRewriteInfo(checkPoint_, totalDeathNum_);
}

void GameMain::Init()
{
	makeScreenHandle_ = MakeScreen(Game::screen_width, Game::screen_height, true);

	checkCollisionModel_ = std::make_shared<CheckCollisionModel>();
	objManager_ = std::make_shared<ObjectManager>();
	shotManager_ = std::make_shared<ShotManager>();
	tutorial_ = std::make_shared<Tutorial>();

	//�I�u�W�F�N�g�𐶐�
	ObjectGenerater();

	//broom_ = make_shared<Broom>();
	//depthOfField_ = make_shared<DepthOfField>();

	//���Ń��C�g�����������Ă���
	SetUseLighting(false);

	//1m�͈̔͂�ݒ肷��
	Set3DSoundOneMetre(10.0f);

	//�Z�[�u�f�[�^�̓��e��ǂݎ��
	//���S��
	totalDeathNum_ = ExternalFile::GetInstance().GetSaveData().totalDeathNum;

	skyHandle_ = MV1LoadModel("data/model/skyDorm/SkyDorm.mv1");
	float scale = 30.0f;
	MV1SetScale(skyHandle_, VGet(scale, scale, scale));
	MV1SetPosition(skyHandle_, VGet(0, 200, 0));

	//3D���X�i�[�̈ʒu��ݒ肷��
	SoundManager::GetInstance().Set3DSoundListenerInfo(camera_->GetPos(), camera_->GetTarget());
	//3D�T�E���h�Ɋ֘A�������ݒ肷��
	SoundManager::GetInstance().Set3DSoundInfo(VGet(575, 120, -60), 1000, "cafe");
	//����cafe�Ƃ������y�𗬂��Ă���
	SoundManager::GetInstance().Play("cafe");
}

void GameMain::End()
{
	DeleteGraph(makeScreenHandle_);
}

//�X�V
void GameMain::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

//�`��
void GameMain::Draw()
{
	SetDrawScreen(makeScreenHandle_);
	ClearDrawScreen();

	//�J�����̏�����
	//SetDrawScreen���s���ƃJ�����̏�񂪃��Z�b�g����邽�߂�
	camera_->Init();
	camera_->Update(player_->GetStatus().pos, player_->GetStatus().height);

//	broom_->writingScreenUpdate(player_->GetStatus().pos);

	//�I�u�W�F�N�g�̕`��
	objManager_->Draw(player_->GetStatus().pos);

	MV1SetPosition(skyHandle_, player_->GetStatus().pos);
	MV1DrawModel(skyHandle_);

	//�v���C���[�̕`��
	player_->Draw();

	shotManager_->Draw();

	tutorial_->Draw();

//	broom_->graphFilterUpdate();
//	broom_->draw();

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

//�I�u�W�F�N�g�̐���
void GameMain::ObjectGenerater()
{

	auto& loadData = ExternalFile::GetInstance();

	//�v���C���[�̃C���X�^���X��
	player_ = make_shared<Player>(enemy_Filename);
	//�v���C���[�̏�����
	player_->Init(loadData.GetSpecifiedInfo("Player").front());
	//�Q�[���I�u�W�F�N�g�̐���
	objManager_->ObjectGenerator();
	//�J�����̃C���X�^���X��
	camera_ = make_shared<Camera>();

}

//TODO�F�ʂ̃t�F�[�h�C�����o���������
void GameMain::FadeInUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &GameMain::NormalUpdate;
		fadeValue_ = 0;
	}
}

//�X�V
void GameMain::NormalUpdate(const InputState& input)
{
	
	//�t�B���^�[�������s��Ȃ��p�ɂ���
	isFilterOn_ = false;

	//�v���C���[�̍X�V
	player_->Update(input, objManager_);

	//�I�u�W�F�N�g�̍X�V
	objManager_->Update(*player_,input,shotManager_);

	//�v���C���[�Ƃ��̑��I�u�W�F�N�g�Ƃ̏Փ˔���
	checkCollisionModel_->CheckCollision(player_,objManager_);

	//�J�����̒����_��ύX����
	camera_->ChangeOfFocus(input);

	shotManager_->Update();
	shotManager_->Hit(*player_);

	tutorial_->Update(player_->GetStatus().pos);

	//���X�i�[�̈ʒu�ƕ�����ݒ�
	//����́A�v���C���[�ł͂Ȃ��J�����̍��W�ɂ��Ă���
	SoundManager::GetInstance().Set3DSoundListenerInfo(camera_->GetPos(), camera_->GetTarget());

	//�|�[�Y�V�[�����J��
	if (input.IsTriggered(InputType::pause)) {
		isFilterOn_ = true;
		manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
	}

//	auto info = ExternalFile::GetInstance().GetSpecifiedInfo("GOAL");
//
//	auto result = MV1CollCheck_Capsule(player_->GetModelPointer()->GetModelHandle(), player_->GetModelPointer()->GetColFrameIndex(), VGet(info.front().pos.x, info.front().pos.y - info.front().scale.y, info.front().pos.z), VGet(info.front().pos.x, info.front().pos.y + info.front().scale.y, info.front().pos.z), 600);
//
//	if (result.HitNum > 0) {
//		updateFunc_ = &GameMain::FadeOutUpdate;
//	}
//
//	MV1CollResultPolyDimTerminate(result);

}

//TODO�F�ʂ̃t�F�[�h�C�����o���������
void GameMain::FadeOutUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameEnd>(manager_)));
		return;
	}
}
