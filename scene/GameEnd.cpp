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
#include "../util/FontsManager.h"
#include "../util/SoundManager.h"

#include <algorithm>

namespace {
	//�_�O���t�̉��̃T�C�Y
	constexpr int bar_graph_width = 20;

	//�_�O���t�̏c�̃T�C�Y
	constexpr int bar_graph_height = 10;

	//���̕�����
	constexpr int division_width = 10;

	//�c�̕�����
	constexpr int division_height = 15;
}

//�R���X�g���N�^
GameEnd::GameEnd(SceneManager& manager) : SceneBase(manager),updateFunc_(&GameEnd::FadeInUpdate)
{
	//�Z�k��
	auto& model = ModelManager::GetInstance();
	auto& file = ExternalFile::GetInstance();
	auto& font = FontsManager::GetInstance();
	auto& sound = SoundManager::GetInstance();

	//�C���X�^���X��
	objManager_ = std::make_shared<ObjectManager>();
	camera_ = std::make_shared<Camera>(VGet(265, 314, -803), VGet(265, 314, 1777));
	playerModel_ = std::make_shared<Model>(model.GetModelHandle(objData[static_cast<int>(ObjectType::Player)].name), Material::Other);
	boardModel_  = std::make_shared<Model>(model.GetModelHandle(objData[static_cast<int>(ObjectType::WhiteBoard)].name), Material::Iron);

	//���f���̐ݒ�
	auto info = file.GetSpecifiedInfo("end", "player");
	playerModel_->SetPos(info.pos);
	playerModel_->SetRot(info.rot);
	playerModel_->SetScale(info.scale);
	//�A�j���[�V�����̕ύX
	playerModel_->SetAnimation(static_cast<int>(PlayerAnimType::Run), true, true);

	//���f���̐ݒ�
	info = file.GetSpecifiedInfo("end", "WhiteBoard");
	boardModel_->SetPos(info.pos);
	boardModel_->SetRot(info.rot);
	boardModel_->SetScale(info.scale);

	//�I�u�W�F�N�g�̐���
	objManager_->EndStageObjectGenerator();

	//�v���C���[�̎�̍��W���擾
	VECTOR pos = playerModel_->GetFrameLocalPosition("hand.R_end");
	float height = 0.0f;

	info = file.GetSpecifiedInfo("end", "player");

	//���̃��f���̐���
	for (int i = 0; i < file.GetTotalDeathNum().back(); i++) {
		corpseModel_.push_back(std::make_pair(false, std::make_shared<Model>(model.GetModelHandle(objData[static_cast<int>(ObjectType::Player)].name), Material::Other)));
		corpseModel_[i].second->SetPos({ pos.x,pos.y + height,pos.z });
		corpseModel_[i].second->SetRot(info.rot);
		corpseModel_[i].second->SetScale(info.scale);
		corpseModel_[i].second->SetAnimEndFrame(static_cast<int>(PlayerAnimType::Death));
		//�}�e���A���̐F��ς���
		MV1SetMaterialDifColor(corpseModel_[i].second->GetModelHandle(), 8, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
		height += 15.0f;
	}

	//�_�O���t�̏�������
	for (int i = 0; i < 5;i++) {
		barGraphHeight_.push_back(Game::screen_height / division_height * 12);
	}

	//���̂�u���ꏊ����ʂ𕪊����Č��߂�
	//��ʂ̕������̎擾
	divisionNum_ = file.GetTotalDeathNum().back() / 5 + 1;
	if (file.GetTotalDeathNum().back() % 5 > 0) {
		divisionNum_++;
	}

	//�t�H���g�n���h���̎擾
	pigumo21FontHandle_ = font.GetFontHandle("�s�O�� 0021");
	pigumo42FontHandle_ = font.GetFontHandle("�s�O�� 0042");

	//���C�g�������s��Ȃ�
	SetUseLighting(false);

	sound.Set3DSoundListenerInfo(camera_->GetPos(), camera_->GetTarget());
}

//�f�X�g���N�^
GameEnd::~GameEnd()
{
}

//������
void GameEnd::Init()
{
}

//�I��
void GameEnd::End()
{
}

//�X�V
void GameEnd::Update()
{
	(this->*updateFunc_)();
}

//�`��
void GameEnd::Draw()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	camera_->Init(VGet(265, 314, 1777));

	//�v���C���[���f���̕`��
	playerModel_->Draw();

	//�z���C�g�{�[�h�̕`��
	boardModel_->Draw();

	//���̃��f���̕`��
	for (auto& corpse : corpseModel_) {
		corpse.second->Draw();
	}

	//�I�u�W�F�N�g�̕`��
	objManager_->Draw({ 0,0,0 });

	//�v���C���[�̍��W���X�N���[�����W�ɂ���
	VECTOR screenPos = ConvWorldPosToScreenPos(boardModel_->GetPos());

	//int�^�̃X�N���[�����W
	int castScreenPosX = static_cast<int>(screenPos.x);

	//�F
	int color = 0x0000ff;

	for (int i = 0; i < file.GetTotalDeathNum().size();i++) {

		barGraphHeight_[i] = (std::max)(barGraphHeight_[i] - 1, Game::screen_height / division_height * 12 - file.GetTotalDeathNum()[i] * bar_graph_height);

		if (static_cast<int>(file.GetTotalDeathNum().size() - 1) - i == 0) {
			color = 0xff0000;
		}

		//�_�O���t�̕`��
		DrawBox(Game::screen_width / division_width * (i + 3) - bar_graph_width - (Game::screen_width / 2 - castScreenPosX),
				barGraphHeight_[i],
				Game::screen_width / division_width * (i + 3) + bar_graph_width - (Game::screen_width / 2 - castScreenPosX),
				Game::screen_height / division_height * 12,
				color, true);

		//�t�H���g��K�p����������̃T�C�Y���擾
		int size = GetDrawFormatStringWidthToHandle(pigumo42FontHandle_,"%d��", file.GetTotalDeathNum()[i]);

		//�����̕`��
		DrawFormatStringToHandle(Game::screen_width / division_width * (i + 3) - size / 2 - (Game::screen_width / 2 - castScreenPosX),
								 Game::screen_height / division_height * 12 - file.GetTotalDeathNum()[i] * bar_graph_height - size,
								 0x000000, pigumo42FontHandle_, "%d��", file.GetTotalDeathNum()[i]);

		if (4 - i == 0) {
			DrawVStringToHandle(Game::screen_width / division_width * (i + 3) - 10 - (Game::screen_width / 2 - castScreenPosX),
								Game::screen_height / division_height * 12 + 15,
								"����", 0xff0000, pigumo21FontHandle_);
			continue;
		}

		DrawFormatVStringToHandle(Game::screen_width / division_width * (i + 3)-10 - (Game::screen_width / 2 - castScreenPosX),
								  Game::screen_height / division_height * 12+15,
								  0x000000,
								  pigumo21FontHandle_,
								  "%d��O", 4 - i);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//��ʑS�̂�^�����ɓh��Ԃ�
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//���̂̍X�V
void GameEnd::CorpseUpdate(const VECTOR playerPos)
{
	//���̂̍����̕␳
	float height = 0.0f;

	//���̂̃��f������]������
	if (isTurn_) {
		for (int i = 0; i < corpseModel_.size(); i++) {
			if (corpseModel_[i].first) {
				continue;
			}
			VECTOR rot = {};
			rot.y = 0 * DX_PI_F / 180.0f;
			corpseModel_[i].second->SetRot(rot);
		}
	}

	//�A�j���[�V�������I��������A�����Ɏ��̂�u��
	if (playerModel_->IsAnimEnd()) {
		for (int i = corpseNum_; i < corpseNum_ + 5; i++) {
			//���݂̔ԍ������̂̑����𒴂�����ȍ~�̏������s��Ȃ�
			if (i > static_cast<int>(corpseModel_.size() - 1)) {
				continue;
			}

			//���̂̍��W���v���C���[�̍��W�ɐݒ肷��
			corpseModel_[i].second->SetPos({ playerPos.x,playerPos.y + height, playerPos.z });

			//�t���O��true�ɂ���
			corpseModel_[i].first = true;

			//������ύX����
			height += 15.0f;
		}

		//�܂��u����Ă��Ȃ����̂̃��f������]������
		for (int i = 0; i < corpseModel_.size(); i++) {
			if (corpseModel_[i].first) {
				continue;
			}

			VECTOR rot = {};
			rot.y = -90 * DX_PI_F / 180.0f;
			corpseModel_[i].second->SetRot(rot);
		}

		//���̂�5�����ɒu������A�܂��茳�Ɏ��̂��c���Ă�����
		//���삷�鎀�̂̔ԍ��͈͂�ς���
		if ((static_cast<int>(corpseModel_.size()) - 1 - corpseNum_ + 1) / 5 > 0) {
			corpseNum_ += 5;
		}

		//�t���O��܂�
		isPutting_ = false;
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
}

//�z���C�g�{�[�h�̍X�V
void GameEnd::WhiteBoardUpdate()
{
	//�Z�k��
	auto& sound = SoundManager::GetInstance();

	if (isWentToRightSide_) {
		return;
	}

	if (!isResultDisplaying_) {
		boardModel_->SetPos(VAdd(boardModel_->GetPos(), moveVec_));
		boardIsMoving_ = true;
		if (!sound.CheckSoundFile("pull")) {
			sound.PlaySE("pull");
		}
	}

	//�v���C���[�̍��W���X�N���[�����W�ɂ���
	VECTOR screenPos = ConvWorldPosToScreenPos(boardModel_->GetPos());

	//int�^�̃X�N���[�����W
	int castScreenPosX = static_cast<int>(screenPos.x);

	//��ʂ̔����̃T�C�Y(��)
	int screenWidthHalf = Game::screen_width / 2;

	if (castScreenPosX >= screenWidthHalf - 1 && castScreenPosX <= screenWidthHalf + 1) {
		boardIsMoving_ = false;
		isResultDisplaying_ = true;
		playerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::Idle), true, false, 10);
	}

	//�z���C�g�{�[�h����ʊO�ɏo����t�F�[�h�A�E�g����
	if (castScreenPosX < -Game::screen_width / 2) {
		updateFunc_ = &GameEnd::FadeOutUpdate;
	}
}

//�t�F�[�h�C���̍X�V
void GameEnd::FadeInUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &GameEnd::NormalUpdate;
		fadeValue_ = 0;
	}
}

//�ʏ펞�̍X�V
void GameEnd::NormalUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();
	auto& file = ExternalFile::GetInstance();
	auto& sound = SoundManager::GetInstance();

	//�v���C���[���f���̍��W
	VECTOR playerPos = playerModel_->GetPos();

	//�v���C���[���f���̍X�V
	playerModel_->Update();

	//�v���C���[�̍��W���X�N���[�����W�ɂ���
	VECTOR screenPos = ConvWorldPosToScreenPos(playerModel_->GetPos());

	//�v���C���[����ʊO�ɍs�����瓮���Ă���t���O��false�ɂ���
	if (screenPos.x > Game::screen_width && isWentToRightSide_) {
		isWentToRightSide_ = false;
		moveVec_.x = -1;
		playerModel_->SetPos(file.GetSpecifiedInfo("end", "secondPos").pos);
		playerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::Pull), true, false, 10);
		playerPos = playerModel_->GetPos();
	}

	if (!isPutting_ && !isResultDisplaying_) {
		//�v���C���[���f���̍��W�ύX
		playerModel_->SetPos(VAdd(playerPos, moveVec_));
		if (playerModel_->GetSpecifiedAnimTime(6) || playerModel_->GetSpecifiedAnimTime(45)) {
			sound.Set3DSoundInfo(playerPos, 800.0f, "asphaltStep");
			sound.PlaySE("asphaltStep");
		}
	}

	//�z���C�g�{�[�h�̍X�V
	WhiteBoardUpdate();

	if (input.IsTriggered(InputType::Space) && isResultDisplaying_) {
		isResultDisplaying_ = false;
	}

	if (!isWentToRightSide_) {
		return;
	}

	//��ʂ𕪊�������̈������̉��̒���
	int divisionPosX = Game::screen_width / divisionNum_;

	//�v���C���[�̍��W�����Ɏ擾�����X�N���[�����W��
	//��L�̃T�C�Y�ŗ]������߂�
	int errorRange = static_cast<int>(screenPos.x) % (divisionPosX * currentDivisionNum_);

	if (screenPos.x > 50 && screenPos.x < Game::screen_width - 50) {
		//�덷�͈͓���������A�v���C���[�̃A�j���[�V������ύX����
		if (errorRange >= -1 && errorRange <= 1 && !isPutting_) {
			//�A�j���[�V������ύX����
			playerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::Put), false, true, 10);

			//�t���O�𗧂Ă�
			isPutting_ = true;

			currentDivisionNum_++;
		}
	}

	//�v���C���[������̃A�j���[�V�����t���[���ɂȂ�����t���O�𗧂Ă�
	if (isPutting_) {
		if (playerModel_->GetSpecifiedAnimTime(30)) {
			isTurn_ = true;
		}
		else if (playerModel_->GetSpecifiedAnimTime(40)) {
			isTurn_ = false;
		}
	}

	//���̂̍X�V
	CorpseUpdate(playerPos);

	if (!isPutting_) {
		//�A�j���[�V������ύX����
		playerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::Run), true, false, 10);
	}
}

//�t�F�[�h�A�E�g�̍X�V
void GameEnd::FadeOutUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
		return;
	}
}
