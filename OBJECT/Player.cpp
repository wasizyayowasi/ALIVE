#include "Player.h"
#include "../util/InputState.h"
#include "../util/Model.h"
#include "../util/CheckCollisionModel.h"
#include "../util/LoadExternalFile.h"
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

Player::Player():updateFunc_(&Player::idleUpdate)
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
		playerInfo.jumpPower = loadExternalFile.getPlayerInfo().jumpPower;
		playerInfo.runningJumpPower = loadExternalFile.getPlayerInfo().runningJumpPower;
		playerInfo.walkSpeed = loadExternalFile.getPlayerInfo().walkSpeed;
		playerInfo.runningSpeed = loadExternalFile.getPlayerInfo().runningSpeed;
		playerInfo.rotSpeed = loadExternalFile.getPlayerInfo().rotSpeed;
		for (int i = 0; i < static_cast<int>(AnimType::max); i++) {
			playerInfo.anim_[i] = loadExternalFile.getPlayerInfo().animNo_[i];
		}
	}

	for (int i = 0; i < static_cast<int>(AnimType::max); i++)
	{
		animType_[static_cast<AnimType>(i)] = playerInfo.anim_[i];
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

	//�W�����v���̏���
	jump_.isJump = false;
	jump_.jumpVec = 0.0f;
}


/// <summary>
/// �v���C���[�̍X�V���s��
/// </summary>
/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
/// <param name="models">�Փ˔�����s�����f����vector�^�̔z��</param>
void Player::update(const InputState& input, std::vector<std::shared_ptr<Model>> models)
{
	//�ړ��x�N�g���̃��Z�b�g
	moveVec_ = { 0.0f,0.0f,0.0f };

	//�v���C���[�̃A�j���[�V�����X�V
	PModel_->update();
	
	(this->*updateFunc_)(input);

	//���̂�vector�z���������models�z��ɒǉ�����
	for (auto& deadPerson : deadPerson_) {
		models.push_back(deadPerson);
	}

	//�v���C���[�Ƃ��̑��I�u�W�F�N�g�Ƃ̏Փ˔���
	checkCollisionModel_->checkCollision(*this,moveVec_, models, player_hegiht, jump_.isJump, jump_.jumpVec);
}

/// <summary>
/// �v���C���[�֘A�̕`��
/// </summary>
void Player::draw()
{
	PModel_->draw();
	for (auto& deadPlayer : deadPerson_) {
		deadPlayer->draw();
	}

	DrawFormatString(0, 16, 0x448844, "X:%.2f,Y:%.2f,Z:%.2f", pos_.x, pos_.y, pos_.z);
	DrawFormatString(0, 32, 0x448844, "X:%.2f,Y:%.2f,Z:%.2f", temp_.x, temp_.y, temp_.z);
	DrawFormatString(0, 48, 0x448844, "%.2f", tempAngle_);
	DrawFormatString(0, 64, 0x448844, "%d", deathCount_);

	for (const auto person : deadPerson_) {
		DrawSphere3D(person->getPos(), 16, 32, 0xff0000, 0xff0000, true);
	}
}

VECTOR Player::getRot()
{
	return {rot_.x, rot_.y * DX_PI_F / 180.0f, rot_.z};
}

/// <summary>
/// �O������̃|�W�V�������󂯎��
/// </summary>
/// <param name="pos">�|�W�V�������</param>
void Player::setPos(VECTOR pos)
{
	pos_ = pos;
	PModel_->setPos(pos_);
}

/// <summary>
/// �O������̃W�����v�����󂯎��
/// </summary>
/// <param name="isJump">�W�����v�t���O</param>
/// <param name="jumpVec">�W�����v�x�N�g��</param>
void Player::setJumpInfo(bool isJump, float jumpVec)
{
	jump_.isJump = isJump;
	jump_.jumpVec = jumpVec;
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
		updateFunc_ = &Player::sitUpdate;
		return;
	}

	//�����o�֐��|�C���^��runningJumpUpdate�A
	//jumpUpdate�̂ǂ��炩�ɕύX����
	if (input.isPressed(InputType::space)) {
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
		if (jump_.isJump && jump_.jumpVec == 0.0f) {
			pos_.y += gravity * 20;
		}

		if (pos_.y <= -400.0f) {
			pos_ = checkPoint_;
		}
		if (input.isTriggered(InputType::space)) {
			pos_ = checkPoint_;
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
		animNo_ = animType_[AnimType::idle];
		isAnimLoop_ = true;
		PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);
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
			moveVec_.z += movingSpeed;
			targetAngle_ = 180.0f;
		}
		if (pressedDown) {
			moveVec_.z -= movingSpeed;
			targetAngle_ = 0.0f;
		}
		if (pressedLeft) {
			moveVec_.x -= movingSpeed;
			targetAngle_ = 90.0f;
		}
		if (pressedRight) {
			moveVec_.x += movingSpeed;
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
		if (animNo_ != animType_[AnimType::runningJump] && animNo_ != animType_[AnimType::jump]) {
			if (movingSpeed != 0.0f) {
				if (movingSpeed > playerInfo.walkSpeed) {
					animNo_ = animType_[AnimType::run];
					isAnimLoop_ = true;
				}
				else if (movingSpeed <= playerInfo.walkSpeed) {
					animNo_ = animType_[AnimType::walk];
					isAnimLoop_ = true;
				}
			}
		}

		//�ړ��x�N�g����p�ӂ���
		moveVec_ = VScale(VNorm(moveVec_), movingSpeed);

		//��]����
		rotationUpdate();

		//�A�j���[�V�����̕ύX
		PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);

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
		rot_.y -= playerInfo.rotSpeed;
		tempAngle_ -= playerInfo.rotSpeed;
	}
	else if (differenceAngle_ > 0.0f) {
		rot_.y += playerInfo.rotSpeed;
		tempAngle_ += playerInfo.rotSpeed;
	}

	//360�x�A���������0�x�ɖ߂��悤�ɂ��Ă���
	if (tempAngle_ == 360.0f || tempAngle_ == -360.0f) {
		tempAngle_ = 0.0f;
	}
	if (rot_.y == 360.0f || rot_.y == -360.0f) {
		rot_.y = 0.0f;
	}

	//���ʂ����f���̉�]���Ƃ��đ���
	PModel_->setRot({ rot_.x,rot_.y * DX_PI_F / 180.0f,rot_.z });
}

//�I�u�W�F�N�g��o��
void Player::climUpdate(const InputState& input)
{
	animNo_ = animType_[AnimType::clim];
	isAnimLoop_ = false;

	PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);

	VECTOR localPosition;

	if (PModel_->isAnimEnd()) {
		localPosition = PModel_->getAnimFrameLocalPosition(animNo_, "mixamorig:LeftToeBase");
		localPosition = VAdd(localPosition, PModel_->getAnimFrameLocalPosition(animNo_, "mixamorig:RightToeBase"));
		localPosition.x = localPosition.x / 2;
		localPosition.y = localPosition.y / 2;
		localPosition.z = localPosition.z / 2;
		temp_ = localPosition;
		pos_ = localPosition;
		PModel_->setPos(pos_);
		animNo_ = animType_[AnimType::stand];
		isAnimLoop_ = false;
		PModel_->setAnimation(animNo_, isAnimLoop_, true);
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
		if (!jump_.isJump && animNo_ != animType_[AnimType::jump]) {
			animNo_ = animType_[AnimType::jump];
			jump_.jumpVec += playerInfo.jumpPower;
			jump_.isJump = true;
			isAnimLoop_ = false;
			PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);
		}

		//�󒆂ɂ���Ƃ�
		//�d�͂��x�N�g���ɑ����ă|�W�V�����ɑ���
		if (jump_.isJump) {
			jump_.jumpVec += gravity;
			pos_.y += jump_.jumpVec;
		}

		//�W�����v�x�N�g����0�ŃW�����v���ł͂Ȃ�������
		//idle��Ԃ̃A�b�v�f�[�g�ɕύX����A�A�j���[�V�������ύX����
		if (jump_.jumpVec == 0.0f && !jump_.isJump) {
			updateFunc_ = &Player::idleUpdate;
			animNo_ = animType_[AnimType::idle];
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
	if (!jump_.isJump && animNo_ != animType_[AnimType::runningJump]) {
		animNo_ = animType_[AnimType::runningJump];
		jump_.jumpVec += playerInfo.runningJumpPower;
		isAnimLoop_ = false;
		jump_.isJump = true;
		PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);
	}

	//HACK�F�ϐ������̂܂܁@+�@�ǂ����邩�Y��ł���
	//�A�j���[�V�����̑����Ԃɂ���āA�d�͂�ύX����
	temp = PModel_->getAnimTotalTime() + 2;
	tempGravity = -(playerInfo.runningJumpPower * 2 / temp);

	//�󒆂ɂ���Ƃ�
	//�d�͂��x�N�g���ɑ����ă|�W�V�����ɑ���
	if (jump_.isJump) {
		jump_.jumpVec += tempGravity;
		pos_.y += jump_.jumpVec;
	}

	//�W�����v�x�N�g����0�ŃW�����v���ł͂Ȃ�������
	//idle��Ԃ̃A�b�v�f�[�g�ɕύX����A�A�j���[�V�������ύX����
	if (jump_.jumpVec == 0.0f && !jump_.isJump) {
		updateFunc_ = &Player::idleUpdate;
		animNo_ = animType_[AnimType::run];
		isAnimLoop_ = true;
		return;
	}
}

/// <summary>
/// �v���C���[�̎��̂ɗ^����������֐�
/// </summary>
/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
void Player::deathUpdate(const InputState& input)
{
	deathPos = pos_;				//���񂾏ꏊ���c��

	isAnimLoop_ = false;			//�A�j���[�V�����̃��[�v�����邩

	//����A�j���[�V�����ȊO�������玀�ʃA�j���[�V�����ɕς���
	if (animNo_ != animType_[AnimType::sit]) {
		animNo_ = animType_[AnimType::death];
		PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);
	}

	if (PModel_->isAnimEnd()) {
		deathCount_++;					//���S�񐔂��J�E���g����
		deathPersonPostProsessing();
	}

}

//���̂̌㏈��
void Player::deathPersonPostProsessing()
{
	pos_ = checkPoint_;				//�`�F�b�N�|�C���g�Ƀv���C���[���A��

	deadPersonGenerater();			//���̂𐶐�����֐�

	deadPerson_.back()->setAnimEndFrame(animNo_);			//���̂Ɏw��A�j���[�V�����̍ŏI�t���[����ݒ肷��

	setCollitionInfoByDeathPattern();					//���݂̃A�j���[�V�����ɂ���ďՓ˔���p���f���t���[����ݒ肷��

	updateFunc_ = &Player::idleUpdate;
}

/// <summary>
/// �v���C���[�̎��̂�vector�z��ŊǗ�����֐�
/// </summary>
void Player::deadPersonGenerater()
{
	//���̂̐���
	deadPerson_.push_back(make_shared<Model>(PModel_->getModelHandle()));
	//���̂̃|�W�V�����ݒ�
	deadPerson_.back()->setPos(deathPos);
	//���̂̃T�C�Y�ݒ�
	deadPerson_.back()->setScale(player_scale);
	//���̂̉�]�ݒ�
	deadPerson_.back()->setRot({ rot_.x,rot_.y * DX_PI_F / 180.0f,rot_.z });

	//���̂𐔂���
	int deathNum = 0;
	for (const auto person : deadPerson_) {
		deathNum++;
		if (deathNum > 3) {
			deadPerson_.erase(deadPerson_.begin());		//�ő吔�𒴂������ԌÂ����̂�����
		}
	}
}

/// <summary>
/// �v���C���[�ɍ���A�j���[�V������������֐�
/// </summary>
/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
void Player::sitUpdate(const InputState& input)
{
	//����ߒ��̃A�j���[�V�������I�������O�p����ɂ���
	if (animNo_ == animType_[AnimType::idleToSitup] && PModel_->isAnimEnd()) {
		animNo_ = animType_[AnimType::sit];
		PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);
	}

	//���ƒ�̃A�j���[�V�������I�������idleupdate�ɕύX����
	if (animNo_ == animType_[AnimType::situpToIdle] && PModel_->isAnimEnd()) {
		updateFunc_ = &Player::idleUpdate;
		isSitting_ = false;
		return;
	}

	//�A�j���[�V����������ߒ��̃A�j���[�V�����ɕύX
	//�����Ă���t���O�𗧂āA�A�j���[�V�������[�v�ϐ���܂�
	if (!isSitting_) {
		animNo_ = animType_[AnimType::idleToSitup];
		isSitting_ = true;
		isAnimLoop_ = false;
		PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);
	}

	//���ʃR�}���h
	if (input.isTriggered(InputType::death)) {
		deathUpdate(input);
		isSitting_ = false;
		updateFunc_ = &Player::idleUpdate;
		return;
	}

	//�����オ�邽�߂̃R�}���h
	if (input.isTriggered(InputType::ctrl)) {
		animNo_ = animType_[AnimType::situpToIdle];
		isAnimLoop_ = false;
		PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);
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

//���̂̏Փ˔���̐ݒ�
void Player::setCollitionInfoByDeathPattern()
{
	//�A�j���[�V�����ԍ��ɂ���ďՓ˔���p�̃t���[����ύX����
	switch (static_cast<AnimType>(animNo_)) {
	case AnimType::death:
		deadPerson_.back()->setCollFrame(coll_frame_death);
		break;
	case AnimType::sit:
		deadPerson_.back()->setCollFrame(coll_frame_Sit);
		break;
	}
}

//�v���C���[�̑��x�ݒ�
float Player::playerSpeed(bool pressedShift)
{
	if (pressedShift) return playerInfo.runningSpeed;
	
	return playerInfo.walkSpeed;
}