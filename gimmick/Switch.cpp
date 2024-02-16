#include "Switch.h"
#include "../util/Model.h"
#include "../OBJECT/Player.h"
#include "../util/SoundManager.h"
#include "../object/ObjectBase.h"

namespace {
	//���f���̃t�@�C���p�X
	const char* const switch_filename = "data/model/other/mv1/switch.mv1";
}

//�R���X�g���N�^
Switch::Switch(LoadObjectInfo objInfo)
{
	//���f���N���X�̏�����
	model_ = std::make_shared<Model>(switch_filename,Material::Iron);
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);
	model_->SetRot(objInfo.rot);
	model_->SetUseCollision(true,false);

	//�|�W�V�����̏�����
	pos_ = objInfo.pos;

	//�����o�֐��|�C���^��offAnim�ɂ���
	stateFunc_ = &Switch::OffAnim;
}

//�f�X�g���N�^
Switch::~Switch()
{
}

//�X�V
void Switch::Update(Player& player)
{
	//�A�j���[�V�����̍X�V
	model_->Update();

	//�Փ˔���
	HitCollPlayer(player);

	(this->*stateFunc_)();
}

//�`��
void Switch::Draw()
{
	model_->Draw();
}

void Switch::DeleteHitResult()
{
	//�Փˌ��ʂ̍폜
	for (auto& result : hitDim_) {
		MV1CollResultPolyDimTerminate(result);
	}

	//�Փˌ��ʂ�ێ�����z��̍폜
	hitDim_.clear();
}

void Switch::ChangeDuringStartup(float time)
{
	//�Z�k��
	auto& sound = SoundManager::GetInstance();

	if (time == 180.0f) {
		if (!isDuringStartup_) {
			stateFunc_ = &Switch::OffAnim;
		}
		isDuringStartup_ = false;
	}
}

//�Փ˔���
void Switch::HitCollPlayer(Player& player)
{
	MV1RefreshCollInfo(model_->GetModelHandle(), model_->GetColFrameIndex());

	VECTOR playerPos = player.GetStatus().pos;

	//�v���C���[�̈ʒu�������ɂ����J�v�Z���ƃX�C�b�`���f���̔���
	hitDim_.push_back(MV1CollCheck_Capsule(model_->GetModelHandle(), model_->GetColFrameIndex(), playerPos, VAdd(playerPos, VGet(0.0f, player.GetStatus().height, 0.0f)), 20.0f));
}

void Switch::HitColl(std::shared_ptr<ObjectBase> deadPerson)
{
	MV1RefreshCollInfo(deadPerson->GetModelPointer()->GetModelHandle(), deadPerson->GetModelPointer()->GetColFrameIndex());

	//�����^�ђ���������ȍ~�̏������s��Ȃ�
	if (deadPerson->GetIsTransit()) {
		return;
	}

	//�v���C���[�̈ʒu�������ɂ����J�v�Z���ƃX�C�b�`���f���̔���
	hitDim_.push_back(MV1CollCheck_Capsule(deadPerson->GetModelPointer()->GetModelHandle(), deadPerson->GetModelPointer()->GetColFrameIndex(), pos_, VAdd(pos_, VGet(0.0f, 50.0f, 0.0f)), 50.0f));
}

//�Փ˔��茋�ʂ̏�����
bool Switch::ElevatorCollResult()
{
	//�������Ă��鐔�𐔂���
	for (auto& result : hitDim_) {
		if (result.HitNum > 0) {
			isDuringStartup_ = true;
		}
	}

	DeleteHitResult();

	//�������Ă��Ȃ�������
	//�A�j���[�V������ύX���I��
	if (!isDuringStartup_) {
		return false;
	}

	if (stateFunc_ == &Switch::OffAnim) {
		SoundManager::GetInstance().Set3DSoundInfo(pos_, 1500.0f, "switchOn");
		SoundManager::GetInstance().PlaySE("switchOn");
	}

	stateFunc_ = &Switch::OnAnim;

	return true;
}

bool Switch::TransCollResult()
{
	int hitNum = 0;

	//�������Ă��鐔�𐔂���
	for (auto& result : hitDim_) {
		if (result.HitNum > 0) {
			hitNum++;
		}
	}

	DeleteHitResult();

	//�������Ă��Ȃ�������
	//�A�j���[�V������ύX���I��
	if (hitNum < 1) {
		stateFunc_ = &Switch::OffAnim;
		return false;
	}

	if (stateFunc_ == &Switch::OffAnim) {
		SoundManager::GetInstance().Set3DSoundInfo(pos_, 1500.0f, "switchOn");
		SoundManager::GetInstance().PlaySE("switchOn");
	}

	stateFunc_ = &Switch::OnAnim;

	return true;
}

//�Փ˔�����s�����f���̒ǉ�
std::shared_ptr<Model> Switch::GetModelPointer()
{
	return model_;
}

//�X�C�b�`�I���A�j���[�V����
void Switch::OnAnim()
{
	model_->ChangeAnimation(0, false, false, 10);
	MV1SetMaterialDifColor(model_->GetModelHandle(), 1, GetColorF(0.0f, 0.0f, 1.0f, 1.0f));
}

//�X�C�b�`�I�t�A�j���[�V����
void Switch::OffAnim()
{
	model_->ChangeAnimation(1, false, false, 10);
	MV1SetMaterialDifColor(model_->GetModelHandle(), 1, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
}
