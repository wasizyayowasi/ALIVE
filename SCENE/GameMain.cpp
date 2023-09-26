#include "GameMain.h"
#include "SceneManager.h"
#include "GameEnd.h"

#include "../gimmick/GimmickBase.h"
#include "../gimmick/Switch.h"
#include "../gimmick/Steelyard.h"

#include "../object/Player.h"
#include "../object/Camera.h"
#include "../object/CharacterBase.h"
#include "../object/Enemy.h"

#include "../staging/Broom.h"
#include "../staging/DepthOfField.h"

#include "ScenePause.h" 

#include "../util/game.h"
#include "../util/SoundManager.h"
#include "../util/InputState.h"
#include "../util/model.h"
#include "../util/LoadExternalFile.h"


using namespace std;

namespace {
	const char* const temp_filepath = "data/model/tempFiled4.mv1";
	const char* const temp_stairs = "data/model/stairs.mv1";
	const char* const cube_filename = "data/model/cube.mv1";
	const VECTOR scale = { 0.5f,0.5f, 0.5f };
}

GameMain::GameMain(SceneManager& manager, bool continuation) : SceneBase(manager),updateFunc_(&GameMain::fadeInUpdate)
{
	isContinuation_ = continuation;

	camera_ = make_shared<Camera>();
	player_ = make_shared<Player>();
	//broom_ = make_shared<Broom>();
	//depthOfField_ = make_shared<DepthOfField>();
	models_.push_back(make_shared<Model>(temp_filepath));
	enemy_ = make_shared<Enemy>();
	switch_ = make_shared<Switch>();
	steelyard_ = make_shared<Steelyard>();

	models_[0]->setScale(scale);
	models_[0]->setCollFrame();

	SetUseLighting(false);

	Set3DSoundOneMetre(10.0f);

	auto data = LoadExternalFile::getInstance(isContinuation_);
	player_->setSaveData(data.getSaveData().checkPoint,data.getSaveData().totalDeathNum,isContinuation_);
	player_->init();

	//3D���X�i�[�̈ʒu��ݒ肷��
	SoundManager::getInstance().set3DSoundListenerInfo(camera_->getPos(), camera_->getTarget());
	//3D�T�E���h�Ɋ֘A�������ݒ肷��
	SoundManager::getInstance().set3DSoundInfo(VGet(575,120,-60),1000,"cafe");
	//����cafe�Ƃ������y�𗬂��Ă���
	SoundManager::getInstance().play("cafe");

}

GameMain::~GameMain()
{
	LoadExternalFile::getInstance(isContinuation_).saveDataRewriteInfo(checkPoint_, player_->getDeathNum());
}

//�X�V
void GameMain::update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

//�`��
void GameMain::draw()
{

	//broom_->writingScreenUpdate(player_->getPos());
	DrawString(0, 0, "GameMain", 0xffffff);

	for (auto& model : models_) {
		model->draw();
	}

	player_->draw();
	enemy_->draw();
	switch_->draw();
	steelyard_->draw();
	//broom_->graphFilterUpdate();
	//broom_->draw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//��ʑS�̂�^�����ɓh��Ԃ�
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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
	player_->update(input,models_);
	camera_->trackingCameraUpdate(input,player_->getPos());
	switch_->update();
	//enemy_->update();
	//camera_->fixedPointCamera(player_->getPos());

	SoundManager::getInstance().set3DSoundListenerInfo(camera_->getPos(), camera_->getTarget());

	if (player_->getPos().x > 1900) {
		checkPoint_ = { 1900,0,0 };
	}

	if (input.isTriggered(InputType::pause)) {
		manager_.pushScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
	}
}

//TODO�F�ʂ̃t�F�[�h�C�����o���������
void GameMain::fadeOutUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.changeScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
		return;
	}
}
