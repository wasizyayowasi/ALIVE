#include "Player.h"
#include "../util/InputState.h"
#include "../util/Model.h"
#include "../util/CheckCollisionModel.h"
#include "../util/LoadExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/ObjectManager.h"
#include<algorithm>
#include<string>

namespace {
	//�d��
	constexpr float gravity = -0.4f;

	//�t�@�C���p�X
	const char* const player_Filename = "data/player/temp.mv1";
	//���f���t���[����
	const char* const coll_frame_death = "CollisionDeath";
	const char* const coll_frame_Sit = "CollisionSit";
	const char* const coll_frame_Stand = "CollisionStand";

	//�t���[���̖��O
	const char* const frame_name = "foot.L";

	//�v���C���[�T�C�Y
	const VECTOR player_scale = { 1.0f,1.0f ,1.0f };

	//�����v���C���[�̉�]�p�x
	const VECTOR start_player_rot = { 0.0f,0.0f ,0.0f };

	//�v���C���[�̍���
	constexpr float player_hegiht = 150.0f;

}

using namespace std;

Player::Player(const char* const filename):updateFunc_(&Player::IdleUpdate),carryUpdateFunc_(&Player::CarryObjectUpdate)
{

	//�W�����v���̏���
	status_.jump.isJump = false;
	status_.jump.jumpVec = 0.0f;
	//�v���C���[�̍����̐ݒ�
	status_.height = player_hegiht;
}

Player::Player(int handle) :updateFunc_(&Player::IdleUpdate)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
}


void Player::Init(LoadObjectInfo info)
{

	auto loadExternalFile = LoadExternalFile::GetInstance();
	//�v���C���[���̏�����
	{
		playerInfo_.jumpPower = loadExternalFile.GetPlayerInfo().jumpPower;
		playerInfo_.runningJumpPower = loadExternalFile.GetPlayerInfo().runningJumpPower;
		playerInfo_.walkSpeed = loadExternalFile.GetPlayerInfo().walkSpeed;
		playerInfo_.runningSpeed = loadExternalFile.GetPlayerInfo().runningSpeed;
		playerInfo_.rotSpeed = loadExternalFile.GetPlayerInfo().rotSpeed;
		for (int i = 0; i < static_cast<int>(AnimType::max); i++) {
			playerInfo_.animNo_[i] = loadExternalFile.GetPlayerInfo().animNo_[i];
		}
	}

	for (int i = 0; i < static_cast<int>(AnimType::max); i++)
	{
		animType_[static_cast<AnimType>(i)] = playerInfo_.animNo_[i];
	}

	//�v���C���[���f���̐���
	PModel_ = make_shared<Model>(player_Filename);
	//�A�j���[�V�����̐ݒ�
	PModel_->SetAnimation(animType_[AnimType::idle], true, false);
	//�v���C���[�̑傫���̒���
	PModel_->SetScale(info.scale);
	//�|�W�V�����̐ݒ�
	PModel_->SetPos(info.pos);
	//��]���̐ݒ�
	PModel_->SetPos(info.rot);
	//�}�b�v��u���b�N�Ȃǂ̓����蔻��̐���
	checkCollisionModel_ = make_shared<CheckCollisionModel>();
	//�R���W�����t���[���̐ݒ�
	PModel_->SetCollFrame("Character");

	scale_ = info.scale;

}



void Player::Update(const InputState& input)
{
	//�ړ��x�N�g���̃��Z�b�g
	status_.moveVec = { 0.0f,0.0f,0.0f };

	//�v���C���[�̃A�j���[�V�����X�V
	PModel_->Update();
	
	(this->*updateFunc_)(input);

	//�v���C���[�Ƃ��̑��I�u�W�F�N�g�Ƃ̏Փ˔���
	checkCollisionModel_->CheckCollision(*this);
}


void Player::Draw()
{
	PModel_->Draw();
//	VECTOR aiu = FramPosition2("hand.R_end");
//	DrawFormatString(0, 48, 0x448844, "%.2f , %.2f , %.2f", aiu.x, aiu.y, aiu.z);
}

void Player::SetPos(VECTOR pos)
{
	status_.pos = pos;
	PModel_->SetPos(status_.pos);
}


void Player::SetJumpInfo(bool isJump, float jumpVec)
{
	status_.jump.isJump = isJump;
	status_.jump.jumpVec = jumpVec;
}


//HACK:�������A�C�ɐH��Ȃ�
/// <summary>
/// �A�j���[�V������idle��Ԃ̎��ɍs����
/// </summary>
/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
void Player::IdleUpdate(const InputState& input)
{
	

	if (input.IsTriggered(InputType::carry)) {
		(this->*carryUpdateFunc_)();
	}

	if (status_.isTransit) {
		deadPersonModelPointer_->SetAnimEndFrame(animType_[AnimType::dead]);
		deadPersonModelPointer_->SetRot(DegreesToRadians(status_.rot));
		deadPersonModelPointer_->SetPos(FramPosition2("hand.R_end"));
	}
	else {
		isCanBeCarried_ = false;
	}

	ChangeAnimIdle();
	MovingUpdate(input);

	//�����^�ђ���������
	//�ȍ~�̏������s��Ȃ�
	if (status_.isTransit) {
		return;
	}

	//�����o�֐��|�C���^��runningJumpUpdate�A
	//jumpUpdate�̂ǂ��炩�ɕύX����
	if (input.IsTriggered(InputType::space)) {

		if (isClim_) {
			//�A�j���[�V�����̕ύX
			//ChangeAnimNo(AnimType::clim, false, 20);
			//updateFunc_ = &Player::ClimUpdate;
			return;
		}
		//�����j���O�W�����v�̍폜�\��(����)
		/*else if (input.IsPressed(InputType::shift)) {
			PlayerJump(playerInfo_.runningJumpPower);
			ChangeAnimNo(AnimType::runningJump, false, 20);
			updateFunc_ = &Player::RunningJumpUpdate;
			return;
		}*/
		else {
			PlayerJump(playerInfo_.jumpPower);
			ChangeAnimNo(AnimType::jump, false, 20);
			updateFunc_ = &Player::JumpUpdate;
			return;
		}
	}

	//�����o�֐��|�C���^��sitUpdate�ɕύX����
	if (input.IsTriggered(InputType::ctrl)) {
		updateFunc_ = &Player::IdleToSitup;
		return;
	}

	//�����o�֐��|�C���^��DeathUpdate�ɕύX����
	if (input.IsTriggered(InputType::death)) {
		deathCount_++;
		updateFunc_ = &Player::DeathUpdate;
		return;
	}

}

/// <summary>
/// �A�j���[�V������idle�ɖ߂��֐�
/// </summary>
void Player::ChangeAnimIdle()
{
	//�ҋ@�A�j���[�V�����ɖ߂�
	if (!isMoving_) {

		if (status_.isTransit) {
			ChangeAnimNo(AnimType::carryIdle, true, 20);
		}
		else {
			ChangeAnimNo(AnimType::idle, true, 20);
		}
	}
}

//HACK:�������A�C�ɐH��Ȃ�
/// <summary>
/// �v���C���[���ړ������邽�߂̊֐�
/// </summary>
/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
void Player::MovingUpdate(const InputState& input)
{
	
	float movingSpeed = Move(input);

	//HACK�F�����Ƃ����A�j���[�V�����ԍ��ύX������͂�
	if (movingSpeed != 0.0f) {
		if (movingSpeed > playerInfo_.walkSpeed) {
			//�A�j���[�V�����̕ύX
			if (status_.isTransit) {
				ChangeAnimNo(AnimType::carryRunning, true, 20);
			}
			else {
				ChangeAnimNo(AnimType::run, true, 20);
			}
		}
		else if (movingSpeed <= playerInfo_.walkSpeed) {
			//�A�j���[�V�����̕ύX
			if (status_.isTransit) {
				ChangeAnimNo(AnimType::carryWalking, true, 20);
			}
			else {
				ChangeAnimNo(AnimType::walk, true, 20);
			}
		}
	}

	if (VSize(status_.moveVec) == 0.0f) {
		isMoving_ = false;
		return;
	}

	//�ړ��x�N�g����p�ӂ���
	status_.moveVec = VScale(VNorm(status_.moveVec), movingSpeed);

}

float Player::Move(const InputState& input) {
	//�L�[�̉������u�[���^�Ɋi�[
	bool pressedUp = input.IsPressed(InputType::up);
	bool pressedDown = input.IsPressed(InputType::down);
	bool pressedLeft = input.IsPressed(InputType::left);
	bool pressedRight = input.IsPressed(InputType::right);
	bool pressedShift = input.IsPressed(InputType::shift);

	isMoving_ = false;
	float movingSpeed = 0.0f;

	if (pressedUp || pressedDown || pressedLeft || pressedRight) {
		movingSpeed = PlayerSpeed(pressedShift);
		isMoving_ = true;
	}

	//HACK�F�����A���t�@�N�^�����O�K�{
	if (pressedUp) {
		status_.moveVec.z += movingSpeed;
		targetAngle_ = 180.0f;
	}
	if (pressedDown) {
		status_.moveVec.z -= movingSpeed;
		targetAngle_ = 0.0f;
	}
	if (pressedLeft) {
		status_.moveVec.x -= movingSpeed;
		targetAngle_ = 90.0f;
	}
	if (pressedRight) {
		status_.moveVec.x += movingSpeed;
		targetAngle_ = 270.0f;
	}
	if (pressedUp && pressedRight) {
		targetAngle_ = 225.0f;
	}
	if (pressedUp && pressedLeft) {
		targetAngle_ = 135.0f;
	}
	if (pressedDown && pressedLeft) {
		targetAngle_ = 45.0f;
	}
	if (pressedDown && pressedRight) {
		targetAngle_ = 315.0f;
	}

	//��]����
	RotationUpdate();

	return movingSpeed;

}

//�����i�����獡�ア����Ȃ��Ă����Ǝv��
/// <summary>
/// �v���C���[�̉�]�������s���֐�
/// </summary>
void Player::RotationUpdate()
{
	//�ڕW�̊p�x���猻�݂̊p�x�������č����o���Ă���
	differenceAngle_ = targetAngle_ - angle_;

	//��Ƀv���C���[���f�������肳�������Ȃ��̂�
	//181�x����-181�x�ȏ�̏ꍇ�A�t���ɂ��Ă�����
	if (differenceAngle_ >= 180.0f) {
		differenceAngle_ = targetAngle_ - angle_ - 360.0f;
	}
	else if (differenceAngle_ <= -180.0f) {
		differenceAngle_ = targetAngle_ - angle_ + 360.0f;
	}

	//���炩�ɉ�]�����邽��
	//���݂̊p�x�ɉ�]�X�s�[�h�𑝌������Ă���
	if (differenceAngle_ < 0.0f) {
		status_.rot.y -= playerInfo_.rotSpeed;
		angle_ -= playerInfo_.rotSpeed;
	}
	else if (differenceAngle_ > 0.0f) {
		status_.rot.y += playerInfo_.rotSpeed;
		angle_ += playerInfo_.rotSpeed;
	}

	//360�x�A���������0�x�ɖ߂��悤�ɂ��Ă���
	if (angle_ == 360.0f || angle_ == -360.0f) {
		angle_ = 0.0f;
	}
	if (status_.rot.y == 360.0f || status_.rot.y == -360.0f) {
		status_.rot.y = 0.0f;
	}

	//���ʂ����f���̉�]���Ƃ��đ���
	PModel_->SetRot(DegreesToRadians(status_.rot));
}

//�I�u�W�F�N�g��o��
void Player::ClimUpdate(const InputState& input)
{
	if (PModel_->IsAnimEnd()) {
		status_.pos = FramPosition("mixamorig:LeftToeBase", "mixamorig:RightToeBase");
		PModel_->SetPos(status_.pos);

		status_.animNo = animType_[AnimType::stand];
		status_.isAnimLoop = false;
		PModel_->SetAnimation(status_.animNo, status_.isAnimLoop, true);
		updateFunc_ = &Player::StandUpdate;
	}
}

//HACK:�������A�C�ɐH��Ȃ�
/// <summary>
/// ����W�����v�ł͂Ȃ��Ƃ��̃W�����v
/// </summary>
/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
void Player::JumpUpdate(const InputState& input)
{
	//�v���C���[�ړ��֐�
	Move(input);

	//�W�����v����
	{
		//�W�����v�x�N�g����0�ŃW�����v���ł͂Ȃ�������
		//idle��Ԃ̃A�b�v�f�[�g�ɕύX����A�A�j���[�V�������ύX����
		if (!status_.jump.isJump) {
			updateFunc_ = &Player::IdleUpdate;
			return;
		}

		//�󒆂ɂ���Ƃ�
		//�d�͂��x�N�g���ɑ����ă|�W�V�����ɑ���
		if (status_.jump.isJump) {
			status_.jump.jumpVec += gravity;
			status_.pos.y += status_.jump.jumpVec;
		}
	}
}


//HACK:�������A�C�ɐH��Ȃ�
/// <summary>
/// �v���C���[�������Ă���Ƃ��̃W�����v
/// </summary>
/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
void Player::RunningJumpUpdate(const InputState& input)
{
	//�v���C���[�ړ��֐�
	Move(input);

	//HACK�F�ϐ������̂܂܁@+�@�ǂ����邩�Y��ł���
	//�A�j���[�V�����̑����Ԃɂ���āA�d�͂�ύX����
	totalAnimFrame_ = PModel_->GetAnimTotalTime() + 2;
	runJumpGravity = -(playerInfo_.runningJumpPower * 2 / totalAnimFrame_);

	//�󒆂ɂ���Ƃ�
	//�d�͂��x�N�g���ɑ����ă|�W�V�����ɑ���
	if (status_.jump.isJump) {
		status_.jump.jumpVec += runJumpGravity;
		status_.pos.y += status_.jump.jumpVec;
	}

	//�W�����v�x�N�g����0�ŃW�����v���ł͂Ȃ�������
	//idle��Ԃ̃A�b�v�f�[�g�ɕύX����A�A�j���[�V�������ύX����
	if (status_.jump.jumpVec == 0.0f && !status_.jump.isJump) {
		updateFunc_ = &Player::IdleUpdate;
		return;
	}
}

/// <summary>
/// �v���C���[�̎��̂ɗ^����������֐�
/// </summary>
/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
void Player::DeathUpdate(const InputState& input)
{
	deathPos_ = status_.pos;				//���񂾏ꏊ���c��

	//����A�j���[�V�����ȊO�������玀�ʃA�j���[�V�����ɕς���
	if (status_.animNo != animType_[AnimType::idleToSitup]) {
		//�A�j���[�V�����̕ύX
		ChangeAnimNo(AnimType::death, false, 20);
	}

	if (PModel_->IsAnimEnd()) {
		DeathPersonPostProsessing();
	}

}

//���̂̌㏈��
void Player::DeathPersonPostProsessing()
{
	status_.pos = checkPoint_;				//�`�F�b�N�|�C���g�Ƀv���C���[���A��

	DeadPersonGenerater();			//���̂𐶐�����֐�

	updateFunc_ = &Player::IdleUpdate;
}

/// <summary>
/// �v���C���[�̎��̂�vector�z��ŊǗ�����֐�
/// </summary>
void Player::DeadPersonGenerater()
{
	auto& objManager = ObjectManager::GetInstance();

	LoadObjectInfo info;
	info.rot = DegreesToRadians(status_.rot);
	info.pos = deathPos_;
	info.scale = scale_;

	objManager.DeadPersonGenerator(PModel_->GetModelHandle(),info, status_.animNo);
}

/// <summary>
/// �v���C���[�ɍ���A�j���[�V������������֐�
/// </summary>
/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
void Player::SitUpdate(const InputState& input)
{
	//�����オ�邽�߂̃R�}���h
	if (input.IsTriggered(InputType::ctrl)) {
		//�A�j���[�V�����̕ύX
		ChangeAnimNo(AnimType::situpToIdle, false, 20);
	}
	
	//���ƒ�̃A�j���[�V�������I�������idleupdate�ɕύX����
	if (status_.animNo == animType_[AnimType::situpToIdle] && PModel_->IsAnimEnd()) {
		updateFunc_ = &Player::IdleUpdate;
		isSitting_ = false;
		return;
	}

	if (status_.animNo == animType_[AnimType::situpToIdle]) return;

	//���ʃR�}���h
	if (input.IsTriggered(InputType::death)) {
		DeathUpdate(input);
		isSitting_ = false;
		updateFunc_ = &Player::IdleUpdate;
		return;
	}

}

void Player::IdleToSitup(const InputState& input)
{
	//�A�j���[�V����������ߒ��̃A�j���[�V�����ɕύX
	//�����Ă���t���O�𗧂āA�A�j���[�V�������[�v�ϐ���܂�
	if (!isSitting_) {
		isSitting_ = true;
		ChangeAnimNo(AnimType::idleToSitup, false, 20);
	}

	//����ߒ��̃A�j���[�V�������I�������O�p����ɂ���
	if (status_.animNo == animType_[AnimType::idleToSitup] && PModel_->IsAnimEnd()) {
		PModel_->SetAnimEndFrame(status_.animNo);
		updateFunc_ = &Player::SitUpdate;
	}

}


//�����オ�鏈��
void Player::StandUpdate(const InputState& input)
{
	if (PModel_->IsAnimEnd()) {
		updateFunc_ = &Player::IdleUpdate;
		isClim_ = false;
	}
}

void Player::SetCarryInfo(bool isCarry, shared_ptr<Model>model) {
	isCanBeCarried_ = isCarry;
	deadPersonModelPointer_ = model;
}

void Player::CarryObjectUpdate()
{
	
	if (!isCanBeCarried_) return;

	status_.isTransit = true;

	carryUpdateFunc_ = &Player::DropOffObjectUpdate;

}

//�^��ł����I�u�W�F�N�g�����낷����
void Player::DropOffObjectUpdate()
{
	bool isCarryWalking = status_.animNo == animType_[AnimType::carryWalking];
	bool isCarry = status_.animNo == animType_[AnimType::carryIdle];
	if ((isCarryWalking || isCarry) && isCanBeCarried_) {
		isCanBeCarried_ = false;
		deadPersonModelPointer_->SetPos(FramPosition("foot.L", "foot.R"));
		deadPersonModelPointer_.reset();
	}

	status_.isTransit = false;
	isCanBeCarried_ = false;

	carryUpdateFunc_ = &Player::CarryObjectUpdate;

}

//�x���@����ʓx�@�ɕϊ������p�x��Ԃ�
VECTOR Player::DegreesToRadians(VECTOR rot)
{
	VECTOR storageRot;

	storageRot.x = rot.x * DX_PI_F / 180.0f;
	storageRot.y = rot.y * DX_PI_F / 180.0f;
	storageRot.z = rot.z * DX_PI_F / 180.0f;

	return storageRot;
}

void Player::ChangeAnimNo(AnimType type, bool isAnimLoop, int changeTime)
{
	status_.animNo = animType_[type];
	status_.isAnimLoop = isAnimLoop;
	PModel_->ChangeAnimation(status_.animNo, status_.isAnimLoop, false, changeTime);
}

//�v���C���[�̑��x�ݒ�
float Player::PlayerSpeed(bool pressedShift)
{
	if (pressedShift) return playerInfo_.runningSpeed;
	
	return playerInfo_.walkSpeed;
}

//�W�����v�̐ݒ�
void Player::PlayerJump(float jumpPower) {
	status_.jump.jumpVec += jumpPower;
	status_.pos.y += status_.jump.jumpVec;
	status_.jump.isJump = true;
}

//��̃t���[�����W�̒��S���擾����
VECTOR Player::FramPosition(const char* const LeftFramename, const char* const RightFramename)
{

	VECTOR framePosition;

	//�w��t���[���̍��W���擾����B
	framePosition = PModel_->GetAnimFrameLocalPosition(LeftFramename);
	framePosition = VAdd(framePosition, PModel_->GetAnimFrameLocalPosition(RightFramename));
	//��̍��W�𑫂��A2�Ŋ��蒆�S���擾����
	framePosition.x = framePosition.x / 2;
	framePosition.y = framePosition.y / 2;
	framePosition.z = framePosition.z / 2;

	return framePosition;
}

VECTOR Player::FramPosition2(const char* const framename)
{

	VECTOR framePosition;

	//�w��t���[���̍��W���擾����B
	framePosition = PModel_->GetAnimFrameLocalPosition(framename);

	return framePosition;
}
