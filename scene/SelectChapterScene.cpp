#include "SelectChapterScene.h"
#include "GameMain.h"
#include "SceneManager.h"

#include "../object/ObjectData.h"

#include "../util/game.h"
#include "../util/Model.h"
#include "../UTIL/Easing.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/ModelManager.h"

#include <algorithm>
#include <string>

namespace {
	//������
	constexpr float total_time = 60.0f;
}

SelectChapterScene::SelectChapterScene(SceneManager& manager) : SceneBase(manager),updateFunc_(&SelectChapterScene::SlideInBook)
{
}

SelectChapterScene::~SelectChapterScene()
{
}

void SelectChapterScene::Init()
{
	//�C���X�^���X��
	model_ = std::make_shared<Model>(ModelManager::GetInstance().GetModelHandle(ObjectType::Book),Material::Other);

	//�I�u�W�F�N�g�z�u�f�[�^
	auto& file = ExternalFile::GetInstance();

	//���f���z�u���̎擾
	auto info = file.GetSpecifiedInfo("title", "Book");

	//���f���̏��ݒ�
	model_->SetPos(info.pos);
	model_->SetRot(info.rot);
	model_->SetScale(info.scale);

	//�ڕW�̍��W�̎擾
	targetPosX_ = file.GetSpecifiedInfo("title", "BookPutPos").pos.x;

	//�摜�̃��[�h
	textureHandle_[0] = LoadGraph("data/model/room/texture/Chapter1.png");
	textureHandle_[1] = LoadGraph("data/model/room/texture/Chapter2.png");
	textureHandle_[2] = LoadGraph("data/model/room/texture/Chapter3.png");

	model_->SetAnimation(static_cast<int>(BookAnim::normal), false, false);
}

void SelectChapterScene::End()
{
}

void SelectChapterScene::Update()
{
	(this->*updateFunc_)();
}

void SelectChapterScene::Draw()
{
	//���f���̕`��
	model_->Draw();

	//��ʑS�̂�^�����ɓh��Ԃ�
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SelectChapterScene::ChangeChapter()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//�I�΂�Ă���`���v�^�[�̕�������擾����
	std::string str = "Chapter" + std::to_string(selectNum_);

	//�X�^�[�g�|�W�V�������擾����
	VECTOR startPos = file.GetStartPos(str);

	//�v���C���[�̊J�n�ʒu��ݒ肷��
	file.SetPlayerInfo(startPos);
}

void SelectChapterScene::SlideInBook()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//���f���̍��W
	VECTOR pos = model_->GetPos();

	//���Ԃ𑝉�������
	elapsedTime_ = (std::min)(elapsedTime_ + 1.0f, total_time);

	//�C�[�W���O
	pos.x = Easing::InOutCubic(elapsedTime_, total_time, targetPosX_, model_->GetPos().x);

	//���f���̃|�W�V�������W
	model_->SetPos(pos);

	if (elapsedTime_ == total_time) {
		updateFunc_ = &SelectChapterScene::NormalUpdate;
		targetPosX_ = file.GetSpecifiedInfo("title", "Book").pos.x;
		elapsedTime_ = 0;
	}
}

void SelectChapterScene::NormalUpdate()
{
	//���f���̍X�V
	model_->Update();

	//�Z�k��
	auto& input = InputState::GetInstance();

	if (model_->IsAnimEnd()) {
		model_->ChangeAnimation(static_cast<int>(BookAnim::normal), false, false, 10);
	}
	else {
		return;
	}

	//�`���v�^�[�I��
	if (input.IsTriggered(InputType::Right)) {
		if (selectNum_ < 2) {
			model_->ChangeAnimation(static_cast<int>(BookAnim::open), false, false, 10);
			MV1SetTextureGraphHandle(model_->GetModelHandle(), 0, textureHandle_[(std::min)(selectNum_ + 1, 2)], true);
		}
		selectNum_ = (std::min)(selectNum_ + 1, 2);
	}
	if (input.IsTriggered(InputType::Left)) {
		if (selectNum_ > 0) {
			model_->ChangeAnimation(static_cast<int>(BookAnim::close), false, false, 10);
			MV1SetTextureGraphHandle(model_->GetModelHandle(), 0, textureHandle_[(std::max)(selectNum_ - 1, 0)], true);
		}
		selectNum_ = (std::max)(selectNum_ - 1, 0);
	}

	//�߂�
	if (input.IsTriggered(InputType::Down)) {
		updateFunc_ = &SelectChapterScene::SlideOutBook;
	}

	//����
	if (input.IsTriggered(InputType::Space)) {
		updateFunc_ = &SelectChapterScene::FadeOutUpdate;
	}
}

void SelectChapterScene::SlideOutBook()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//���f���̍��W
	VECTOR pos = model_->GetPos();

	//���Ԃ𑝉�������
	elapsedTime_ = (std::min)(elapsedTime_ + 1.0f, total_time);

	//�C�[�W���O
	pos.x = Easing::InOutCubic(elapsedTime_, total_time, targetPosX_, model_->GetPos().x);

	//���f���̃|�W�V�������W
	model_->SetPos(pos);

	if (elapsedTime_ == total_time) {
		manager_.PopFrontScene();
	}
}

void SelectChapterScene::FadeOutUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		ChangeChapter();
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_)));
		return;
	}
}
