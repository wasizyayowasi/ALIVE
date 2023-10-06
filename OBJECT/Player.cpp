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
	const char* const player_Filename = "DATA/player/player14.mv1";
	//���f���t���[����
	const char* const coll_frame_death = "CollisionDeath";
	const char* const coll_frame_Sit = "CollisionSit";
	const char* const coll_frame_Stand = "CollisionStand";

	//�v���C���[�T�C�Y
	const VECTOR player_scale = { 0.5f,0.5f ,0.5f };

	//�����v���C���[�̉�]�p�x
	const VECTOR start_player_rot = { 0.0f,0.0f ,0.0f };

	//�v���C���[�̍���
	constexpr float player_hegiht = 150.0f;

}

using namespace std;

Player::Player(const char* const filename):updateFunc_(&Player::idleUpdate)
{

	//�W�����v���̏���
	status_.jump.isJump = false;
	status_.jump.jumpVec = 0.0f;
}

Player::Player(int handle) :updateFunc_(&Player::idleUpdate)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
}


void Player::init()
{

	auto loadExternalFile = LoadExternalFile::getInstance(isContinue_);
	//�v���C���[���̏�����
	{
		playerInfo_.jumpPower = loadExternalFile.getPlayerInfo().jumpPower;
		playerInfo_.runningJumpPower = loadExternalFile.getPlayerInfo().runningJumpPower;
		playerInfo_.walkSpeed = loadExternalFile.getPlayerInfo().walkSpeed;
		playerInfo_.runningSpeed = loadExternalFile.getPlayerInfo().runningSpeed;
		playerInfo_.rotSpeed = loadExternalFile.getPlayerInfo().rotSpeed;
		for (int i = 0; i < static_cast<int>(AnimType::max); i++) {
			playerInfo_.anim_[i] = loadExternalFile.getPlayerInfo().animNo_[i];
		}
	}

	for (int i = 0; i < static_cast<int>(AnimType::max); i++)
	{
		animType_[static_cast<AnimType>(i)] = playerInfo_.anim_[i];
	}

	//�v���C���[���f���̐���
	PModel_ = make_shared<Model>(player_Filename);
	//�A�j���[�V�����̐ݒ�
	PModel_->setAnimation(animType_[AnimType::idle], true, false);
	//�v���C���[�̑傫���̒���
	PModel_->setScale(player_scale);
	//�}�b�v��u���b�N�Ȃǂ̓����蔻��̐���
	checkCollisionModel_ = make_shared<CheckCollisionModel>();
	//�R���W�����t���[���̐ݒ�
	PModel_->setCollFrame("Character");
}



void Player::update(const InputState& input)
{
	//�ړ��x�N�g���̃��Z�b�g
	status_.moveVec = { 0.0f,0.0f,0.0f };

	//�v���C���[�̃A�j���[�V�����X�V
	PModel_->update();
	
	(this->*updateFunc_)(input);

	//�v���C���[�Ƃ��̑��I�u�W�F�N�g�Ƃ̏Փ˔���
	checkCollisionModel_->checkCollision(*this, status_.moveVec,player_hegiht, status_.jump.isJump, status_.jump.jumpVec);
}


void Player::draw()
{
	PModel_->draw();

	DrawFormatString(0, 48, 0x448844, "%.2f", tempAngle_);
	DrawFormatString(0, 64, 0x448844, "%d", deathCount_);

}

VECTOR Player::getRot()
{
	return { status_.rot.x, status_.rot.y * DX_PI_F / 180.0f, status_.rot.z};
}


void Player::setPos(VECTOR pos)
{
	status_.pos = pos;
	PModel_->setPos(status_.pos);
}


void Player::setJumpInfo(bool isJump, float jumpVec)
{
	status_.jump.isJump = isJump;
	status_.jump.jumpVec = jumpVec;
}


//HACK:�������A�C�ɐH��Ȃ�
/// <summary>
/// �A�j���[�V������idle��Ԃ̎��ɍs����
/// </summary>
/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
void Player::idleUpdate(const InputState& input)
{

	//�����o�֐��|�C���^��sitUpdate�ɕύX����
	if (input.isTriggered(InputType::ctrl)) {
		updateFunc_ = &Player::idleToSitup;
		return;
	}

	//�����o�֐��|�C���^��runningJumpUpdate�A
	//jumpUpdate�̂ǂ��炩�ɕύX����
	if (input.isTriggered(InputType::space)) {
		if (isClim_) {
			updateFunc_ = &Player::climUpdate;
			return;
		}
		else if (input.isPressed(InputType::shift)) {
			updateFunc_ = &Player::runningJumpUpdate;
			return;
		}
		else {
			updateFunc_ = &Player::jumpUpdate;
			return;
		}
	}

	if (input.isTriggered(InputType::death)) {
		updateFunc_ = &Player::deathUpdate;
		return;
	}

	changeAnimIdle();
	movingUpdate(input);

	//TODO�F���Ȃ�������
	{
		if (status_.jump.isJump && status_.jump.jumpVec == 0.0f) {
			status_.pos.y += gravity * 20;
		}

		if (status_.pos.y <= -400.0f) {
			status_.pos = checkPoint_;
		}
		if (input.isTriggered(InputType::space)) {
			status_.pos = checkPoint_;
		}
	}
	
}

/// <summary>
/// �A�j���[�V������idle�ɖ߂��֐�
/// </summary>
void Player::changeAnimIdle()
{
	//�ҋ@�A�j���[�V�����ɖ߂�
	if (!isMoving_) {
		status_.animNo = animType_[AnimType::idle];
		status_.isAnimLoop = true;
		PModel_->changeAnimation(status_.animNo, status_.isAnimLoop, false, 20);
	}
}

//HACK:�������A�C�ɐH��Ȃ�
/// <summary>
/// �v���C���[���ړ������邽�߂̊֐�
/// </summary>
/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
void Player::movingUpdate(const InputState& input)
{
	//�L�[�̉������u�[���^�Ɋi�[
	bool pressedUp = input.isPressed(InputType::up);
	bool pressedDown = input.isPressed(InputType::down);
	bool pressedLeft = input.isPressed(InputType::left);
	bool pressedRight = input.isPressed(InputType::right);
	bool pressedShift = input.isPressed(InputType::shift);

	isMoving_ = false;
	float movingSpeed = 0.0f;

	if (pressedUp || pressedDown || pressedLeft || pressedRight) {
		movingSpeed = playerSpeed(pressedShift);
		isMoving_ = true;
	}

	//���P���悤
	{
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

		//HACK�F�����Ƃ����A�j���[�V�����ԍ��ύX������͂�
		if (status_.animNo != animType_[AnimType::runningJump] && status_.animNo != animType_[AnimType::jump]) {
			if (movingSpeed != 0.0f) {
				if (movingSpeed > playerInfo_.walkSpeed) {
					status_.animNo = animType_[AnimType::run];
					status_.isAnimLoop = true;
				}
				else if (movingSpeed <= playerInfo_.walkSpeed) {
					status_.animNo = animType_[AnimType::walk];
					status_.isAnimLoop = true;
				}
			}
		}

		//��]����
		rotationUpdate();

		if (VSize(status_.moveVec) == 0.0f) {
			isMoving_ = false;
			return;
		}
		//�ړ��x�N�g����p�ӂ���
		status_.moveVec = VScale(VNorm(status_.moveVec), movingSpeed);

		//�A�j���[�V�����̕ύX
		PModel_->changeAnimation(status_.animNo, status_.isAnimLoop, false, 20);


		//�f�o�b�O�p
		/*{
			if (input.isPressed(InputType::next)) {
				pos_.y += movingSpeed_;
			}
			if (input.isPressed(InputType::prev)) {
				pos_.y -= movingSpeed_;
			}
		}*/
	}
}

//�����i�����獡�ア����Ȃ��Ă����Ǝv��
/// <summary>
/// �v���C���[�̉�]�������s���֐�
/// </summary>
void Player::rotationUpdate()
{
	//�ڕW�̊p�x���猻�݂̊p�x�������č����o���Ă���
	differenceAngle_ = targetAngle_ - tempAngle_;

	//��Ƀv���C���[���f�������肳�������Ȃ��̂�
	//181�x����-181�x�ȏ�̏ꍇ�A�t���ɂ��Ă�����
	if (differenceAngle_ >= 180.0f) {
		differenceAngle_ = targetAngle_ - tempAngle_ - 360.0f;
	}
	else if (differenceAngle_ <= -180.0f) {
		differenceAngle_ = targetAngle_ - tempAngle_ + 360.0f;
	}

	//���炩�ɉ�]�����邽��
	//���݂̊p�x�ɉ�]�X�s�[�h�𑝌������Ă���
	if (differenceAngle_ < 0.0f) {
		status_.rot.y -= playerInfo_.rotSpeed;
		tempAngle_ -= playerInfo_.rotSpeed;
	}
	else if (differenceAngle_ > 0.0f) {
		status_.rot.y += playerInfo_.rotSpeed;
		tempAngle_ += playerInfo_.rotSpeed;
	}

	//360�x�A���������0�x�ɖ߂��悤�ɂ��Ă���
	if (tempAngle_ == 360.0f || tempAngle_ == -360.0f) {
		tempAngle_ = 0.0f;
	}
	if (status_.rot.y == 360.0f || status_.rot.y == -360.0f) {
		status_.rot.y = 0.0f;
	}

	//���ʂ����f���̉�]���Ƃ��đ���
	PModel_->setRot({ status_.rot.x,status_.rot.y * DX_PI_F / 180.0f,status_.rot.z });
}

//�I�u�W�F�N�g��o��
void Player::climUpdate(const InputState& input)
{
	status_.animNo = animType_[AnimType::clim];
	status_.isAnimLoop = false;

	PModel_->changeAnimation(status_.animNo, status_.isAnimLoop, false, 20);

	VECTOR localPosition;

	if (PModel_->isAnimEnd()) {
		localPosition = PModel_->getAnimFrameLocalPosition(status_.animNo, "mixamorig:LeftToeBase");
		localPosition = VAdd(localPosition, PModel_->getAnimFrameLocalPosition(status_.animNo, "mixamorig:RightToeBase"));
		localPosition.x = localPosition.x / 2;
		localPosition.y = localPosition.y / 2;
		localPosition.z = localPosition.z / 2;
		temp_ = localPosition;
		status_.pos = localPosition;
		PModel_->setPos(status_.pos);
		status_.animNo = animType_[AnimType::stand];
		status_.isAnimLoop = false;
		PModel_->setAnimation(status_.animNo, status_.isAnimLoop, true);
		updateFunc_ = &Player::standUpdate;
	}

}

//HACK:�������A�C�ɐH��Ȃ�
/// <summary>
/// ����W�����v�ł͂Ȃ��Ƃ��̃W�����v
/// </summary>
/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
void Player::jumpUpdate(const InputState& input)
{
	//�v���C���[�ړ��֐�
	movingUpdate(input);

	//�W�����v����
	{
		//�A�j���[�V�����ύX�Ƌr�͂��W�����v�x�N�g���ɑ���
		if (!status_.jump.isJump && status_.animNo != animType_[AnimType::jump]) {
			status_.animNo = animType_[AnimType::jump];
			status_.jump.jumpVec += playerInfo_.jumpPower;
			status_.jump.isJump = true;
			status_.isAnimLoop = false;
			PModel_->changeAnimation(status_.animNo, status_.isAnimLoop, false, 20);
		}

		//�󒆂ɂ���Ƃ�
		//�d�͂��x�N�g���ɑ����ă|�W�V�����ɑ���
		if (status_.jump.isJump) {
			status_.jump.jumpVec += gravity;
			status_.pos.y += status_.jump.jumpVec;
		}

		//�W�����v�x�N�g����0�ŃW�����v���ł͂Ȃ�������
		//idle��Ԃ̃A�b�v�f�[�g�ɕύX����A�A�j���[�V�������ύX����
		if (status_.jump.jumpVec == 0.0f && !status_.jump.isJump) {
			updateFunc_ = &Player::idleUpdate;
			status_.animNo = animType_[AnimType::idle];
			return;
		}

	}
}


//HACK:�������A�C�ɐH��Ȃ�
/// <summary>
/// �v���C���[�������Ă���Ƃ��̃W�����v
/// </summary>
/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
void Player::runningJumpUpdate(const InputState& input)
{
	//�v���C���[�ړ��֐�
	movingUpdate(input);

	//�A�j���[�V�����ύX�Ƌr�͂��W�����v�x�N�g���ɑ���
	if (!status_.jump.isJump && status_.animNo != animType_[AnimType::runningJump]) {
		status_.animNo = animType_[AnimType::runningJump];
		status_.jump.jumpVec += playerInfo_.runningJumpPower;
		status_.isAnimLoop = false;
		status_.jump.isJump = true;
		PModel_->changeAnimation(status_.animNo, status_.isAnimLoop, false, 20);
	}

	//HACK�F�ϐ������̂܂܁@+�@�ǂ����邩�Y��ł���
	//�A�j���[�V�����̑����Ԃɂ���āA�d�͂�ύX����
	temp = PModel_->getAnimTotalTime() + 2;
	tempGravity = -(playerInfo_.runningJumpPower * 2 / temp);

	//�󒆂ɂ���Ƃ�
	//�d�͂��x�N�g���ɑ����ă|�W�V�����ɑ���
	if (status_.jump.isJump) {
		status_.jump.jumpVec += tempGravity;
		status_.pos.y += status_.jump.jumpVec;
	}

	//�W�����v�x�N�g����0�ŃW�����v���ł͂Ȃ�������
	//idle��Ԃ̃A�b�v�f�[�g�ɕύX����A�A�j���[�V�������ύX����
	if (status_.jump.jumpVec == 0.0f && !status_.jump.isJump) {
		updateFunc_ = &Player::idleUpdate;
		status_.animNo = animType_[AnimType::run];
		status_.isAnimLoop = true;
		return;
	}
}

/// <summary>
/// �v���C���[�̎��̂ɗ^����������֐�
/// </summary>
/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
void Player::deathUpdate(const InputState& input)
{
	deathPos = status_.pos;				//���񂾏ꏊ���c��

	status_.isAnimLoop = false;			//�A�j���[�V�����̃��[�v�����邩

	//����A�j���[�V�����ȊO�������玀�ʃA�j���[�V�����ɕς���
	if (status_.animNo != animType_[AnimType::sit]) {
		status_.animNo = animType_[AnimType::death];
		PModel_->changeAnimation(status_.animNo, status_.isAnimLoop, false, 20);
	}

	if (PModel_->isAnimEnd()) {
		deathCount_++;					//���S�񐔂��J�E���g����
		deathPersonPostProsessing();
	}

}

//���̂̌㏈��
void Player::deathPersonPostProsessing()
{
	status_.pos = checkPoint_;				//�`�F�b�N�|�C���g�Ƀv���C���[���A��

	deadPersonGenerater();			//���̂𐶐�����֐�

	updateFunc_ = &Player::idleUpdate;
}

/// <summary>
/// �v���C���[�̎��̂�vector�z��ŊǗ�����֐�
/// </summary>
void Player::deadPersonGenerater()
{
	auto& objManager = ObjectManager::getInstance();

	VECTOR rot = { status_.rot.x,status_.rot.y * DX_PI_F / 180.0f,status_.rot.z };

	objManager.deadPersonGenerator(ObjectType::deadPerson, PModel_->getModelHandle(), deathPos, rot, status_.animNo);
}

/// <summary>
/// �v���C���[�ɍ���A�j���[�V������������֐�
/// </summary>
/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
void Player::sitUpdate(const InputState& input)
{
	//�����オ�邽�߂̃R�}���h
	if (input.isTriggered(InputType::ctrl)) {
		status_.animNo = animType_[AnimType::situpToIdle];
		status_.isAnimLoop = false;
		PModel_->changeAnimation(status_.animNo, status_.isAnimLoop, false, 20);
	}
	
	//���ƒ�̃A�j���[�V�������I�������idleupdate�ɕύX����
	if (status_.animNo == animType_[AnimType::situpToIdle] && PModel_->isAnimEnd()) {
		updateFunc_ = &Player::idleUpdate;
		isSitting_ = false;
		return;
	}

	if (status_.animNo == animType_[AnimType::situpToIdle]) return;

	//���ʃR�}���h
	if (input.isTriggered(InputType::death)) {
		deathUpdate(input);
		isSitting_ = false;
		updateFunc_ = &Player::idleUpdate;
		return;
	}

}

void Player::idleToSitup(const InputState& input)
{
	//�A�j���[�V����������ߒ��̃A�j���[�V�����ɕύX
	//�����Ă���t���O�𗧂āA�A�j���[�V�������[�v�ϐ���܂�
	if (!isSitting_) {
		status_.animNo = animType_[AnimType::idleToSitup];
		isSitting_ = true;
		status_.isAnimLoop = false;
		PModel_->changeAnimation(status_.animNo, status_.isAnimLoop, false, 20);
	}

	//����ߒ��̃A�j���[�V�������I�������O�p����ɂ���
	if (status_.animNo == animType_[AnimType::idleToSitup] && PModel_->isAnimEnd()) {
		status_.animNo = animType_[AnimType::sit];
		PModel_->changeAnimation(status_.animNo, status_.isAnimLoop, false, 20);
		updateFunc_ = &Player::sitUpdate;
	}

}

//�����オ�鏈��
void Player::standUpdate(const InputState& input)
{
	if (PModel_->isAnimEnd()) {
		updateFunc_ = &Player::idleUpdate;
		isClim_ = false;
	}
}

//�Z�[�u�f�[�^
void Player::setSaveData(VECTOR pos,int num, bool isContinue)
{
	checkPoint_ = pos;
	deathCount_ = num;
	isContinue_ = isContinue;
}

//�v���C���[�̑��x�ݒ�
float Player::playerSpeed(bool pressedShift)
{
	if (pressedShift) return playerInfo_.runningSpeed;
	
	return playerInfo_.walkSpeed;
}