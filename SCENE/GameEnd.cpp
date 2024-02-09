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
	//�A�j���[�V�����̕ύX
	playerModel_->SetAnimation(static_cast<int>(PlayerAnimType::Run), true, true);

	//���C�g�������s��Ȃ�
	SetUseLighting(false);

	//�I�u�W�F�N�g�̐���
	objManager_->EndStageObjectGenerator();

	//�v���C���[�̎�̍��W���擾
	VECTOR pos = playerModel_->GetFrameLocalPosition("hand.R_end");
	float height = 0.0f;

	//���̃��f���̐���
	for (int i = 0; i < file.GetDeathCount(); i++) {
		corpseModel_.push_back(std::make_pair(false, std::make_shared<Model>(model.GetModelHandle(ObjectType::Player), Material::Other)));
		corpseModel_[i].second->SetPos({ pos.x,pos.y + height,pos.z });
		corpseModel_[i].second->SetRot(info.rot);
		corpseModel_[i].second->SetScale(info.scale);
		corpseModel_[i].second->SetAnimEndFrame(static_cast<int>(PlayerAnimType::Death));
		//�}�e���A���̐F��ς���
		MV1SetMaterialDifColor(corpseModel_[i].second->GetModelHandle(), 8, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
		height += 15.0f;
	}

	//���̂�u���ꏊ����ʂ𕪊����Č��߂�
	//��ʂ̕������̎擾
	divisionNum_ = file.GetDeathCount() / 5 + 1;
	if (file.GetDeathCount() % 5 > 0) {
		divisionNum_++;
	}
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

	//�v���C���[���f���̕`��
	playerModel_->Draw();

	//���̃��f���̕`��
	for (auto& corpse : corpseModel_) {
		corpse.second->Draw();
	}

	//�I�u�W�F�N�g�̕`��
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

	//���삷�鎀�̂̔ԍ�
	static int corpseNum = 0;

	//�v���C���[���f���̍��W
	VECTOR playerPos = playerModel_->GetPos();

	//�v���C���[���f���̍X�V
	playerModel_->Update();

	//�v���C���[���f���̍��W�ύX
	playerModel_->SetPos(VAdd(playerPos, { 1.0f,0,0 }));

	//���삷�鎀�̂̔ԍ������̂̑����𒴂����烊�^�[��
	if (corpseNum > corpseModel_.size()) {
		return;
	}

	//�v���C���[�̍��W���X�N���[�����W�ɂ���
	VECTOR screenPos = ConvWorldPosToScreenPos(playerModel_->GetPos());

	//���̂̍����̕␳
	float height = 0.0f;

	//��ʂ𕪊�������̈������̉��̒���
	int divisionPosX = Game::screen_width / divisionNum_;

	//�v���C���[�̍��W�����Ɏ擾�����X�N���[�����W��
	//��L�̃T�C�Y�ŗ]������߂�
	int errorRange = static_cast<int>(screenPos.x) % divisionPosX;

	//�덷�͈͓���������A���̂��v���C���[�̍��W�ɒu��
	if (errorRange >= -1 && errorRange <= 1) {
		for (int i = corpseNum; i < corpseNum + 5; i++) {
			//���݂̔ԍ������̂̑����𒴂�����ȍ~�̏������s��Ȃ�
			if (i > corpseModel_.size() - 1) {
				continue;
			}

			//���̂̍��W���v���C���[�̍��W�ɐݒ肷��
			corpseModel_[i].second->SetPos({ playerPos.x,playerPos.y + height, playerPos.z });
			//�t���O��true�ɂ���
			corpseModel_[i].first = true;

			//������ύX����
			height += 15.0f;
		}

		//���̂�5�����ɒu������A�܂��茳�Ɏ��̂��c���Ă�����
		//���삷�鎀�̂̔ԍ��͈͂�ς���
		if ((corpseModel_.size() - 1 - corpseNum + 1) % 5 > 0) {
			corpseNum += 5;
		}
	}

	//�v���C���[���f���̎�̍��W���擾
	VECTOR pos = playerModel_->GetFrameLocalPosition("hand.R_end");
	height = 0.0f;

	//���̃��f���̍��W��ύX
	for (int i = 0; i < corpseModel_.size(); i++) {
		if (corpseModel_[i].first) {
			continue;
		}

		corpseModel_[i].second->SetPos({ pos.x,pos.y + height,pos.z });
		height += 15.0f;
	}

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
