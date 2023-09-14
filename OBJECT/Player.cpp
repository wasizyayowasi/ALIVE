#include "Player.h"
#include "../UTIL/InputState.h"
#include "../UTIL/Model.h"
#include "../UTIL/CheckCollisionModel.h"
#include<algorithm>
#include<string>

namespace {
	//�A�j���[�V����
	constexpr int anim_idle_no = 0;			//�ҋ@���
	constexpr int anim_run_no = 1;			//����
	constexpr int anim_stairs_no = 2;		//�K�i�����
	constexpr int anim_clim_no = 3;			//���
	constexpr int anim_jump_no = 4;			//�W�����v
	constexpr int anim_runningJump_no = 5;	//����W�����v
	constexpr int anim_death_no = 6;		//����W�����v
	constexpr int anim_walk_no = 7;			//����
	constexpr int anim_idleToSitup_no = 8;	//idle��Ԃ������
	constexpr int anim_situpToIdle_no = 9;	//�����Ă����Ԃ���idle
	constexpr int anim_sit_no = 10;			//�����Ă���

	//�W�����v
	constexpr float jump_power = 10.0f;
	constexpr float runningJump_power = 8.0f;
	constexpr float gravity = -0.4f;

	//��]�X�s�[�h
	constexpr float rot_speed = 15.0f;

	//�t�@�C���p�X
	const char* const player_Filename = "DATA/player/player11.mv1";
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

	//�ړ��X�s�[�h
	constexpr float walk_speed = 3.0f;
	constexpr float run_speed = 7.0f;
}

using namespace std;

Player::Player():updateFunc_(&Player::idleUpdate)
{
	//�v���C���[���f���̐���
	PModel_ = make_shared<Model>(player_Filename);
	//�A�j���[�V�����̐ݒ�
	PModel_->setAnimation(anim_idle_no, true, false);
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
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
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
	//�ړ��X�s�[�h�̃��Z�b�g
	movingSpeed_ = 0.0f;

	//�v���C���[�̃A�j���[�V�����X�V
	PModel_->update();
	
	(this->*updateFunc_)(input);

	//���̂�vector�z���������models�z��ɒǉ�����
	for (auto& deadPerson : deadPlayer_) {
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
	for (auto& deadPlayer : deadPlayer_) {
		if (deadPlayer->getEnable()) {
			deadPlayer->draw();
		}
	}
	
	DrawFormatString(0, 32, 0x448844, "%d", animNo_);
	DrawFormatString(0, 48, 0x448844, "%d", jump_.isJump);
	DrawFormatString(0, 48, 0x448844, "%f.2", pos_.y);

	DrawSphere3D(pos_, 16, 32, 0x0000ff, 0x0000ff, true);

	for (const auto person : deadPlayer_) {
		if (person->getEnable()) {
			DrawSphere3D(person->getPos(), 16, 32, 0xff0000, 0xff0000, true);
		}
	}
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
		if (input.isPressed(InputType::shift)) {
			updateFunc_ = &Player::runningJumpUpdate;
			return;
		}
		else {
			updateFunc_ = &Player::jumpUpdate;
			return;
		}
	}

	//���S���o���łȂ����
	if (!isDead_) {
		changeAnimIdle();
		movingUpdate(input);
		death(input);
	}
	else {
		if (PModel_->isAnimEnd()) {
			//�`�F�b�N�|�C���g�Ƀv���C���[���A��
			pos_ = checkPoint_;
			//���S���o���I���������Ƃɂ���
			isDead_ = false;

			//���̂𐶐�����֐�
			deadPersonGenerater();

			//���̂Ɏw��A�j���[�V�����̍ŏI�t���[����ݒ肷��
			deadPlayer_.back()->setAnimEndFrame(animNo_);
			
			//�A�j���[�V�����ԍ��ɂ���ďՓ˔���p�̃t���[����ύX����
			switch (animNo_) {
			case anim_death_no:
				deadPlayer_.back()->setCollFrame(coll_frame_death);
				break;
			case anim_sit_no:
				deadPlayer_.back()->setCollFrame(coll_frame_Sit);
				break;
			}
		}
	}

	//TODO�F���Ȃ�������
	{
		if (jump_.isJump && jump_.jumpVec == 0.0f) {
			pos_.y += gravity * 20;
		}

		if (pos_.y <= -400.0f) {
			pos_ = checkPoint_;
		}
		if (input.isTriggered(InputType::next)) {
			pos_ = checkPoint_;
		}
	}
	
}


//HACK:�������A�C�ɐH��Ȃ�
/// <summary>
/// �v���C���[���ړ������邽�߂̊֐�
/// </summary>
/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
void Player::movingUpdate(const InputState& input)
{

	isMoving = false;
	
	//���P���悤
	{
		//HACK�F�����A���t�@�N�^�����O�K�{
		if (input.isPressed(InputType::up)) {
			movingSpeed_ = walk_speed;
			moveVec_.z += movingSpeed_;
			isMoving = true;
			targetAngle_ = 180.0f;
		}
		if (input.isPressed(InputType::down)) {
			movingSpeed_ = walk_speed;
			moveVec_.z -= movingSpeed_;
			isMoving = true;
			targetAngle_ = 0.0f;
		}
		if (input.isPressed(InputType::left)) {
			movingSpeed_ = walk_speed;
			moveVec_.x -= movingSpeed_;
			isMoving = true;
			targetAngle_ = 90.0f;
		}
		if (input.isPressed(InputType::right)) {
			movingSpeed_ = walk_speed;
			moveVec_.x += movingSpeed_;
			isMoving = true;
			targetAngle_ = 270.0f;
		}
		if (input.isPressed(InputType::up) && input.isPressed(InputType::right)) {
			targetAngle_ = 225.0f;
		}
		if (input.isPressed(InputType::up) && input.isPressed(InputType::left)) {
			targetAngle_ = 135.0f;
		}
		if (input.isPressed(InputType::down) && input.isPressed(InputType::left)) {
			targetAngle_ = 45.0f;
		}
		if (input.isPressed(InputType::down) && input.isPressed(InputType::right)) {
			targetAngle_ = 315.0f;
		}
		if ((input.isPressed(InputType::up) || input.isPressed(InputType::down) || input.isPressed(InputType::left) || input.isPressed(InputType::right)) && input.isPressed(InputType::shift)) {
			movingSpeed_ = run_speed;
		}

		//HACK�F�����Ƃ����A�j���[�V�����ԍ��ύX������͂�
		if (animNo_ != anim_runningJump_no && animNo_ != anim_jump_no) {
			if (movingSpeed_ != 0.0f) {
				if (movingSpeed_ > walk_speed) {
					animNo_ = anim_run_no;
					isAnimLoop_ = true;
				}
				else if (movingSpeed_ <= walk_speed) {
					animNo_ = anim_walk_no;
					isAnimLoop_ = true;
				}
			}
		}
		
		//�ړ��x�N�g����p�ӂ���
		moveVec_ = VScale(VNorm(moveVec_),movingSpeed_);

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
		if (!jump_.isJump && animNo_ != anim_jump_no) {
			animNo_ = anim_jump_no;
			jump_.jumpVec += jump_power;
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
			animNo_ = anim_idle_no;
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
	if (!jump_.isJump&& animNo_ != anim_runningJump_no) {
		animNo_ = anim_runningJump_no;
		jump_.jumpVec += runningJump_power;
		isAnimLoop_ = false;
		jump_.isJump = true;
		PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);
	}

	//HACK�F�ϐ������̂܂܁@+�@�ǂ����邩�Y��ł���
	//�A�j���[�V�����̑����Ԃɂ���āA�d�͂�ύX����
	temp = PModel_->getAnimTotalTime() + 2;
	tempGravity = -(runningJump_power * 2 / temp);

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
		animNo_ = anim_run_no;
		isAnimLoop_ = true;
		return;
	}
}

/// <summary>
/// �v���C���[�̎��̂ɗ^����������֐�
/// </summary>
/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
void Player::death(const InputState& input)
{
	//���S
	{
		if (input.isTriggered(InputType::death)) {
			deathPos = pos_;				//���񂾏ꏊ���c��
			
			isAnimLoop_ = false;			
			isDead_ = true;

			//����A�j���[�V�����ȊO�������玀�ʃA�j���[�V�����ɕς���
			if (animNo_ != anim_sit_no) {
				animNo_ = anim_death_no;
				PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);
			}
		}
	}
}

/// <summary>
/// �A�j���[�V������idle�ɖ߂��֐�
/// </summary>
void Player::changeAnimIdle()
{
	//�ҋ@�A�j���[�V�����ɖ߂�
	if (!isMoving) {
		animNo_ = anim_idle_no;
		isAnimLoop_ = true;
		PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);
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
		rot_.y -= rot_speed;
		tempAngle_ -= rot_speed;
	}
	else if(differenceAngle_ > 0.0f){
		rot_.y += rot_speed;
		tempAngle_ += rot_speed;
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

/// <summary>
/// �v���C���[�̎��̂�vector�z��ŊǗ�����֐�
/// </summary>
void Player::deadPersonGenerater()
{
	//���̂̐���
	deadPlayer_.push_back(make_shared<Model>(PModel_->getModelHandle()));
	//���̂̃|�W�V�����ݒ�
	deadPlayer_.back()->setPos(deathPos);
	//���̂̃T�C�Y�ݒ�
	deadPlayer_.back()->setScale(player_scale);
	//���̂̉�]�ݒ�
	deadPlayer_.back()->setRot({ rot_.x,rot_.y * DX_PI_F / 180.0f,rot_.z });
	
	//���̂𐔂���
	int deathNum = 0;
	for (const auto person : deadPlayer_) {
		deathNum++;
		if (deathNum > 10) {
			deadPlayer_.erase(deadPlayer_.begin());		//�ő吔�𒴂������ԌÂ����̂�����
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
	if (animNo_ == anim_idleToSitup_no && PModel_->isAnimEnd()) {
		animNo_ = anim_sit_no;
		PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);
	}

	//���ƒ�̃A�j���[�V�������I�������idleupdate�ɕύX����
	if (animNo_ == anim_situpToIdle_no && PModel_->isAnimEnd()) {
		updateFunc_ = &Player::idleUpdate;
		isSitting_ = false;
		return;
	}

	//�A�j���[�V����������ߒ��̃A�j���[�V�����ɕύX
	//�����Ă���t���O�𗧂āA�A�j���[�V�������[�v�ϐ���܂�
	if (!isSitting_) {
		animNo_ = anim_idleToSitup_no;
		isSitting_ = true;
		isAnimLoop_ = false;
		PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);
	}

	//���ʃR�}���h
	if (input.isTriggered(InputType::death)) {
		death(input);
		isSitting_ = false;
		updateFunc_ = &Player::idleUpdate;
		return;
	}

	//�����オ�邽�߂̃R�}���h
	if(input.isTriggered(InputType::ctrl)){
		animNo_ = anim_situpToIdle_no;
		isAnimLoop_ = false;
		PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);
	}
}
