#include "GameMain.h"
#include "SceneManager.h"
#include "GameEnd.h"

#include "../gimmick/GimmickBase.h"
#include "../gimmick/Switch.h"
#include "../gimmick/Steelyard.h"

#include "../object/Player.h"
#include "../object/cube.h"
#include "../object/Camera.h"
#include "../object/CharacterBase.h"

#include "../staging/Broom.h"
#include "../staging/DepthOfField.h"

#include "ScenePause.h" 

#include "../util/game.h"
#include "../util/SoundManager.h"
#include "../util/InputState.h"
#include "../util/model.h"
#include "../util/LoadExternalFile.h"

#include "../util/ObjectManager.h"
#include "../util/ObjectData.h"

using namespace std;

namespace {
	const char* const player_Filename = "data/player/player16.mv1";
}

GameMain::GameMain(SceneManager& manager) : SceneBase(manager),
updateFunc_(&GameMain::fadeInUpdate)
{
}

GameMain::~GameMain()
{
	//�Q�[�����C�����I���Ƃ��Ƀv���C���Ɏ��񂾉񐔂�
	//saveData�ɋL�^����Ă��鎀�S�񐔂𑫂�
	totalDeathNum_ += player_->getDeathCount();
	LoadExternalFile::GetInstance().SaveDataRewriteInfo(checkPoint_, totalDeathNum_);
}

void GameMain::Init()
{
	makeScreenHandle_ = MakeScreen(Game::screen_width, Game::screen_height, true);

	//�I�u�W�F�N�g�𐶐�
	ObjectGenerater();

	//broom_ = make_shared<Broom>();
	//depthOfField_ = make_shared<DepthOfField>();

	//���Ń��C�g�����������Ă���
	SetUseLighting(false);

	//1m�͈̔͂�ݒ肷��
	Set3DSoundOneMetre(10.0f);

	//�Z�[�u�f�[�^�̓��e��ǂݎ��
	auto data = LoadExternalFile::GetInstance();
	//���S��
	totalDeathNum_ = data.GetSaveData().totalDeathNum;

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
	camera_->init();
	camera_->TrackingCameraUpdate(player_->GetStatus().pos);

	//broom_->writingScreenUpdate(player_->getPos());
	DrawString(0, 0, "GameMain", 0xffffff);
	DrawFormatString(0, 16, 0x448844, "%d", totalDeathNum_);

	//�v���C���[�̕`��
	player_->Draw();
	
	//�I�u�W�F�N�g�̕`��
	ObjectManager::GetInstance().Draw();

	//broom_->graphFilterUpdate();
	//broom_->draw();

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
	//�Z�k��
	auto& objManager = ObjectManager::GetInstance();
	auto& loadData = LoadExternalFile::GetInstance();

	//�J�����̃C���X�^���X��
	camera_ = make_shared<Camera>();
	//�v���C���[�̃C���X�^���X��
	player_ = make_shared<Player>(player_Filename);
	//�v���C���[�̏�����
	player_->Init(loadData.GetSpecifiedInfo("player").front());

	for (auto& objInfo : loadData.GetLoadObjectInfo()) {
		//�t�B�[���h���쐬
		if (objInfo.first == "field") {
			for (auto& objSecond : objInfo.second) {
				objManager.ObjectGenerator(ObjectBaseType::ornamentBase, ObjectType::field, objSecond);
			}
		}
		//�M�~�b�N�X�C�b�`���쐬
		else if (objInfo.first == "switch") {
			for (auto& objSecond : objInfo.second) {
				objManager.ObjectGenerator(ObjectBaseType::gimmickBase, ObjectType::gimmickSwitch, objSecond);
			}
		}
		//�M�~�b�N�V�����쐬
		else if (objInfo.first == "steelyard") {
			for (auto& objSecond : objInfo.second) {
				objManager.ObjectGenerator(ObjectBaseType::gimmickBase, ObjectType::gimmickSteelyard, objSecond);
			}
		}
		//�����쐬
		else if (objInfo.first == "box") {
			for (auto& objSecond : objInfo.second) {
				objManager.ObjectGenerator(ObjectBaseType::carryBase, ObjectType::carry, objSecond);
			}
		}
		//�G���쐬
		else if (objInfo.first == "player") {
			for (auto& objSecond : objInfo.second) {
				objManager.ObjectGenerator(ObjectBaseType::enemyBase, ObjectType::enemy, objSecond);
			}
		}
	}
}

//TODO�F�ʂ̃t�F�[�h�C�����o���������
void GameMain::fadeInUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &GameMain::normalUpdate;
		fadeValue_ = 0;
	}
}

//�X�V
void GameMain::normalUpdate(const InputState& input)
{
	//�Z�k��
	auto& objManager = ObjectManager::GetInstance();
	//�t�B���^�[�������s��Ȃ��p�ɂ���
	isFilterOn_ = false;
	//�I�u�W�F�N�g�̍X�V
	objManager.Update(*player_);

	//�v���C���[�̍X�V
	player_->Update(input);
	//�J�����̒����_��ύX����
	camera_->ChangeOfFocus(input);

	//camera_->fixedPointCamera(player_->getPos());

	//���X�i�[�̈ʒu�ƕ�����ݒ�
	//����́A�v���C���[�ł͂Ȃ��J�����̍��W�ɂ��Ă���
	SoundManager::GetInstance().Set3DSoundListenerInfo(camera_->GetPos(), camera_->GetTarget());

	//����Ȃ����
	if (player_->GetStatus().pos.x > 1900) {
		checkPoint_ = { 1900,0,0 };
	}

	//�|�[�Y�V�[�����J��
	if (input.IsTriggered(InputType::pause)) {
		isFilterOn_ = true;
		manager_.PushScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
	}
}

//TODO�F�ʂ̃t�F�[�h�C�����o���������
void GameMain::fadeOutUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
		return;
	}
}
