#include "GameEnd.h"
#include "SceneTitle.h"
#include "SceneManager.h"

#include "../object/Camera.h"
#include "../object/objectManager.h"

#include "../util/game.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/ModelManager.h"
#include "../util/ExternalFile.h"

#include <DxLib.h>

GameEnd::GameEnd(SceneManager& manager) : SceneBase(manager),updateFunc_(&GameEnd::fadeInUpdate)
{
	//�Z�k��
	auto& model = ModelManager::GetInstance();
	auto& file = ExternalFile::GetInstance();

	//�C���X�^���X��
	objManager_ = std::make_shared<ObjectManager>();
	camera_ = std::make_shared<Camera>(VGet(265, 314, -803), VGet(265, 314, 1777));
	playerModel_ = std::make_shared<Model>(model.GetModelHandle(ObjectType::Player), Material::Other);

	//���f���̐ݒ�
	auto info = file.GetSpecifiedInfo("end", "player");
	playerModel_->SetPos(info.pos);
	playerModel_->SetRot(info.rot);
	playerModel_->SetScale(info.scale);

	playerModel_->SetAnimation(static_cast<int>(PlayerAnimType::Run), true, true);

	//�I�u�W�F�N�g�̐���
	objManager_->EndStageObjectGenerator();

	//���̂̐���
	objManager_->EnemyGenerator()
}

GameEnd::~GameEnd()
{
}

void GameEnd::Init()
{
}

void GameEnd::End()
{
}

void GameEnd::Update()
{
	(this->*updateFunc_)();
}

void GameEnd::Draw()
{
	camera_->Init(VGet(265, 314, 1777));

	camera_->ChangeOfFocus(VGet(265, 314, 1777),10.0f);

	playerModel_->Draw();

	objManager_->Draw({ 0,0,0 });

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//��ʑS�̂�^�����ɓh��Ԃ�
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameEnd::fadeInUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &GameEnd::normalUpdate;
		fadeValue_ = 0;
	}
}

void GameEnd::normalUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//���f���̍X�V
	playerModel_->Update();

	if (input.IsTriggered(InputType::space)) {
		updateFunc_ = &GameEnd::fadeOutUpdate;
	}
}

void GameEnd::fadeOutUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
		return;
	}
}
