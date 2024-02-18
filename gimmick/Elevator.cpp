#include "Elevator.h"
#include "Switch.h"
#include "Lever.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/Easing.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"

#include "../object/Player.h"

namespace {
	//�G���x�[�^�[���ړ����鑬�x
	constexpr float move_speed = 6.0f;

	//�o�����鎞��
	constexpr int max_departure_time = 60;

	//���B����
	constexpr float total_time = 180.0f;
}

//�R���X�g���N�^
Elevator::Elevator(int handle,Material materialType, LoadObjectInfo objInfo):GimmickBase(handle, materialType, objInfo)
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//�|�W�V�����̏�����
	pos_ = objInfo.pos;

	//�^�[�Q�b�g�|�W�V�����̏�����
	targetPos_ = pos_;

	//�Փ˔���p�t���[���̐ݒ�
	model_->SetUseCollision(true, false);

	//���̃G���x�[�^�[�����Ԗڂ̃G���x�[�^�[���𖼑O�̖�������擾����
	int elevatorNum = StrUtil::GetNumberFromString(objInfo.name, ".");

	//�G���x�[�^�[�̔ԍ��ƕ������A����������������擾����
	std::string switchName = StrUtil::GetConcatenateNumAndStrings("ElevatorSwitch", ".", elevatorNum);

	//��L�ŘA���������O�����f�[�^���擾���ăC���X�^���X������
	switch_ = std::make_shared<Switch>(file.GetSpecifiedGimmickInfo(pos_, switchName.c_str()));

	//�G���x�[�^�[�̔ԍ��ƕ������A����������������擾����
	std::string pointName = StrUtil::GetConcatenateNumAndStrings("ElevatorPoint", ".", elevatorNum);

	//�G���x�[�^�[�̔ԍ��ƕ������A����������������擾����
	std::string leverName = StrUtil::GetConcatenateNumAndStrings("ElevatorLever", ".", elevatorNum);

	//���o�[�̃C���X�^���X��
	for (int i = 0; i < 2; i++)
	{
		std::string specificLeverName = StrUtil::GetConcatenateNumAndStrings(leverName, "-", i);
		levers_.push_back(std::make_shared<Lever>(file.GetSpecifiedGimmickInfo(pos_, specificLeverName.c_str())));
	}

	//��~�|�W�V�����̎擾
	for (int i = 0; i < 2; i++)
	{
		std::string specificPointName = StrUtil::GetConcatenateNumAndStrings(pointName, "-", i);
		stopPos_[static_cast<ElevatorState>(i)] = file.GetSpecifiedGimmickInfo(pos_, specificPointName.c_str()).pos;
	}

	//�A�j���[�V�����̐ݒ�
	model_->SetAnimation(static_cast<int>(ElevatorAnimType::openIdle), true, false);

	//��~�|�W�V�����Ƃ̋������擾
	float upperDistanceSize = MathUtil::GetSizeOfDistanceTwoPoints(pos_, stopPos_[ElevatorState::upper]);
	float lowerDistanceSize = MathUtil::GetSizeOfDistanceTwoPoints(pos_, stopPos_[ElevatorState::lower]);

	//���݂̃|�W�V�����ƒ�~�|�W�V�����Ƃ̋������߂����̏�Ԃ�ۑ�����
	if (upperDistanceSize > lowerDistanceSize) {
		state_ = ElevatorState::lower;
	}
	else
	{
		state_ = ElevatorState::upper;
	}
}

//�f�X�g���N�^
Elevator::~Elevator()
{
}

//�X�V
void Elevator::Update(Player& player)
{
	//�Z�k��
 	auto& sound = SoundManager::GetInstance();

	//�v���C���[�̃|�W�V�����̎擾
	VECTOR playerPos = player.GetStatus().pos;

	//���f���̍X�V
	model_->Update();

	//�X�C�b�`�̍X�V
	switch_->ChangeDuringStartup(elapsedTime_);
	switch_->Update(player);

	//���o�[�̍X�V
	for (auto lever : levers_) {
		lever->Update();
	}

	//���o�[�ƃv���C���[�̏Փ˔�����s��
	//�Փ˂��Ă����ꍇ�v���C���[�Ƀ|�C���^��ݒ肷��
	for (auto lever : levers_) {
		if (lever->CollCheck(playerPos)) {
			player.SetLeverPointer(lever);
		}
	}

	//�^�[�Q�b�g�|�W�V�����̕ύX
	if (targetPos_.y == pos_.y && !isDeparture_ && !isOnSwitch_) {
		TargetPosition();
	}
	else {
		if (!switch_->ElevatorCollResult()) {
			isOnSwitch_ = false;
		}
		switch_->DeleteHitResult();
	}

	//�A�j���[�V�������I������A�ړ����J�n����
	if (model_->IsAnimEnd()) {
		Move();
	}
}

//�`��
void Elevator::Draw()
{
	//���f���̕`��
	model_->Draw();

	//�X�C�b�`�̕`��
	switch_->Draw();

	//���o�[�̕`��
	for (auto lever : levers_) {
		lever->Draw();
	}
}

//�G���x�[�^�[���ړ�������
void Elevator::Move()
{
	//���Ԃ̍X�V
	elapsedTime_ = (std::min)(elapsedTime_ + 1.0f, total_time);

	//���W�̈ړ�
	pos_.y = Easing::InOutCubic(elapsedTime_, total_time, targetPos_.y, pos_.y);

	//�ړ��I����A�j���[�V������ύX����
	if (elapsedTime_ == 180.0f && isDeparture_) {
		model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::open), false, false, 10);
		SoundManager::GetInstance().Set3DSoundInfo(pos_, 1500.0f, "door");
		SoundManager::GetInstance().PlaySE("door");
		isDeparture_ = false;
	}

	if (model_->IsAnimEnd() && model_->GetCurrentAnimNo() == static_cast<int>(ElevatorAnimType::open)) {
		model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::openIdle), true, false, 10);
	}

	//�|�W�V�����̐ݒ�
	model_->SetPos(pos_);

	//�X�C�b�`�̃|�W�V�����̐ݒ�
	switch_->GetModelPointer()->SetPos(VGet(pos_.x, pos_.y + 8.0f, pos_.z));
}

//�ړ���̃|�W�V�������擾����
void Elevator::TargetPosition()
{
	//��ԉ������o�[���������Ă���|�W�V�������擾����
	if (switch_->ElevatorCollResult() && !isOnSwitch_)
	{
		isOnSwitch_ = true;
		isDeparture_ = true;
	}

	//���o�[�������ꂽ��A�j���[�V������ύX����
	//�ړI�n�܂ňړ�����
	if (levers_.front()->IsOn()) {
		isDeparture_ = true;
	}

	//���o�[�������ꂽ��A�j���[�V������ύX����
	//�ړI�n�܂ňړ�����
	if (levers_.back()->IsOn()) {
		isDeparture_ = true;
	}

	if (isDeparture_)
	{
		//�T�E���h��炷
		SoundManager::GetInstance().Set3DSoundInfo(pos_, 1500.0f, "door");
		SoundManager::GetInstance().PlaySE("door");

		//�o�ߎ��Ԃ�0�ɂ���
		elapsedTime_ = 0;

		//�A�j���[�V������ύX
		model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::close), false, false, 10);
	}

	if (isDeparture_)
	{
		//elevator�̏�Ԃɂ����
		//elevator�̒�~�|�W�V������ύX����
		switch (state_)
		{
		case ElevatorState::upper:
			targetPos_ = stopPos_[ElevatorState::lower];
			state_ = ElevatorState::lower;
			break;
		case ElevatorState::lower:
			targetPos_ = stopPos_[ElevatorState::upper];
			state_ = ElevatorState::upper;
			break;
		}
	}
}

//�Փ˔�����s�����f���̒ǉ�
std::shared_ptr<Model> Elevator::AddCollModel()
{
	return switch_->GetModelPointer();
}
