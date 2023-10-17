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
	const char* const player_Filename = "data/player/player14.mv1";
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

	//�J�����̃C���X�^���X��
	camera_ = make_shared<Camera>();
	//�v���C���[�̃C���X�^���X��
	player_ = make_shared<Player>(player_Filename);
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
	//
	player_->SetSaveData(data.GetSaveData().checkPoint);

	//�v���C���[�̏�����
	player_->Init();

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

	camera_->init();
	camera_->TrackingCameraUpdate(player_->GetStatus().pos);

	//broom_->writingScreenUpdate(player_->getPos());
	DrawString(0, 0, "GameMain", 0xffffff);
	DrawFormatString(0, 16, 0x448844, "%d", totalDeathNum_);

	player_->Draw();
	
	ObjectManager::GetInstance().Draw();

	//broom_->graphFilterUpdate();
	//broom_->draw();

	SetDrawScreen(DX_SCREEN_BACK);

	DrawGraph(0, 0, makeScreenHandle_, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//��ʑS�̂�^�����ɓh��Ԃ�
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//�I�u�W�F�N�g�̐���
void GameMain::ObjectGenerater()
{
	auto& objManager = ObjectManager::GetInstance();

//	objManager.ObjectGenerator(ObjectBaseType::enemyBase, ObjectType::enemy);
//	objManager.ObjectGenerator(ObjectBaseType::ornamentBase, ObjectType::field);
//	objManager.ObjectGenerator(ObjectBaseType::carryBase, ObjectType::carry);
//	objManager.ObjectGenerator(ObjectBaseType::gimmickBase, ObjectType::gimmickSwitch);
//	objManager.ObjectGenerator(ObjectBaseType::gimmickBase, ObjectType::gimmickSwitch);
//	objManager.ObjectGenerator(ObjectBaseType::gimmickBase, ObjectType::gimmickSteelyard);

	for (auto& objInfo : LoadExternalFile::GetInstance().GetLoadObjectInfo()) {
		if (objInfo.first == "field") {
			for (auto& objSecond : objInfo.second) {
				objManager.ObjectGenerator(ObjectBaseType::ornamentBase, ObjectType::field, objSecond);
			}
		}
		else if (objInfo.first == "switch") {
			for (auto& objSecond : objInfo.second) {
				objManager.ObjectGenerator(ObjectBaseType::gimmickBase, ObjectType::gimmickSwitch, objSecond);
			}
		}
		else if (objInfo.first == "steelyard") {
			for (auto& objSecond : objInfo.second) {
				objManager.ObjectGenerator(ObjectBaseType::gimmickBase, ObjectType::gimmickSteelyard, objSecond);
			}
		}
		else if (objInfo.first == "box") {
			for (auto& objSecond : objInfo.second) {
				objManager.ObjectGenerator(ObjectBaseType::carryBase, ObjectType::carry, objSecond);
			}
		}
		else if (objInfo.first == "player") {
			for (auto& objSecond : objInfo.second) {
				objManager.ObjectGenerator(ObjectBaseType::enemyBase, ObjectType::enemy, objSecond);
			}
		}
	}

	/*for (auto& objInfo : LoadExternalFile::GetInstance(true).GetLoadObjectInfo()) {
		if (objInfo.first == "sofa") {
			for (auto& objSecond : objInfo.second) {
				objManager.ObjectGenerator(ObjectBaseType::ornamentBase, ObjectType::tempsofa, objSecond);
			}
			
		}
		else if(objInfo.first == "bed") {
			for (auto& objSecond : objInfo.second) {
				objManager.ObjectGenerator(ObjectBaseType::ornamentBase, ObjectType::tempbed, objSecond);
			}
		}
	}*/
	

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
	auto& objManager = ObjectManager::GetInstance();

	objManager.Update(*player_);

	player_->Update(input);
	camera_->ChangeOfFocus(input);

	//camera_->fixedPointCamera(player_->getPos());

	SoundManager::GetInstance().Set3DSoundListenerInfo(camera_->GetPos(), camera_->GetTarget());

	if (player_->GetStatus().pos.x > 1900) {
		checkPoint_ = { 1900,0,0 };
	}

	if (input.IsTriggered(InputType::pause)) {
		//GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, 32, 20000);
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
