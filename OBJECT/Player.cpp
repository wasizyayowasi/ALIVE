#include "Player.h"
#include "../gimmick/ManualCrank.h"
#include "../util/InputState.h"
#include "../util/Model.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/ObjectManager.h"
#include<algorithm>
#include<string>

namespace {
	//�d��
	constexpr float gravity = -0.4f;

	//�t�@�C���p�X
	const char* const player_Filename = "data/player/temp.mv1";

	//�t���[���̖��O
	const char* const frame_name = "foot.L";

	//�v���C���[�̍���
	constexpr float player_hegiht = 130.0f;

}

using namespace std;

Player::Player(const char* const filename):updateFunc_(&Player::NormalUpdate),carryUpdateFunc_(&Player::CarryObjectUpdate)
{

	//�W�����v���̏���
	status_.jump.isJump = false;
	status_.jump.jumpVec = 0.0f;
	//�v���C���[�̍����̐ݒ�
	status_.height = player_hegiht;
}

Player::Player(int handle) :updateFunc_(&Player::NormalUpdate)
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

	//�v���C���[���̏�����
	playerInfo_ = ExternalFile::GetInstance().GetPlayerInfo();
	
	for (int i = 0; i < static_cast<int>(AnimType::max); i++)
	{
		animType_[static_cast<AnimType>(i)] = playerInfo_.animNo_[i];
	}

	//�v���C���[���f���̐���
	model_ = make_shared<Model>(player_Filename);
	//�A�j���[�V�����̐ݒ�
	model_->SetAnimation(animType_[AnimType::idle], true, false);
	//�v���C���[�̑傫���̒���
	model_->SetScale(info.scale);
	//�|�W�V�����̐ݒ�
	model_->SetPos(info.pos);
	status_.pos = info.pos;
	//��]���̐ݒ�
	model_->SetRot(info.rot);
	//�R���W�����t���[���̐ݒ�
	model_->SetCollFrame("Character");

	scale_ = info.scale;

}

void Player::Update(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	//�ړ��x�N�g���̃��Z�b�g
	//status_.moveVec = { 0.0f,0.0f,0.0f };

	//�v���C���[�̃A�j���[�V�����X�V
	model_->Update();
	
	MV1RefreshCollInfo(model_->GetModelHandle(), model_->GetColFrameIndex());

	(this->*updateFunc_)(input,objManager);

}


void Player::Draw()
{
	model_->Draw();

	DrawPolygon3D();

//	float au = 20.0f;

//	DrawLine3D(status_.pos, VAdd(status_.pos, VGet(0, status_.height, 0)), 0xff0000);
//	DrawLine3D(VAdd(status_.pos, VGet( au, 0,   0)), VAdd(status_.pos, VGet( au, status_.height,   0)), 0xff0000);
//	DrawLine3D(VAdd(status_.pos, VGet(-au, 0,   0)), VAdd(status_.pos, VGet(-au, status_.height,   0)), 0xff0000);
//	DrawLine3D(VAdd(status_.pos, VGet(  0, 0,  au)), VAdd(status_.pos, VGet(  0, status_.height,  au)), 0xff0000);
//	DrawLine3D(VAdd(status_.pos, VGet(  0, 0, -au)), VAdd(status_.pos, VGet(  0, status_.height, -au)), 0xff0000);

	//DrawSphere3D(VGet(status_.pos.x + 1000, status_.pos.y, status_.pos.z), 16, 32, 0xff0000, 0xff0000, true);

	DrawFormatString(0, 64, 0x448844, "%.2f,%.2f,%.2f", status_.pos.x,status_.pos.y,status_.pos.z);

}

void Player::SetPos(VECTOR pos)
{
	status_.pos = pos;
	model_->SetPos(status_.pos);
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
void Player::NormalUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{

	if (input.IsTriggered(InputType::creative)) {
		debugCreativeMode = !debugCreativeMode;
	}

	if (input.IsTriggered(InputType::activate)) {
		if (status_.situation.isCanBeCarried) {
			(this->*carryUpdateFunc_)();
		}
		else if (status_.situation.isGimmickCanBeOperated) {
			updateFunc_ = &Player::CrankUpdate;
			return;
		}
		else {
			updateFunc_ = &Player::IdleToSitup;
			return;
		}
	}

	status_.situation.isGimmickCanBeOperated = false;

	if (status_.situation.isInTransit) {
		deadPersonModelPointer_->GetModelPointer()->SetAnimEndFrame(animType_[AnimType::dead]);
		deadPersonModelPointer_->GetModelPointer()->SetRot(DegreesToRadians(status_.rot));
		deadPersonModelPointer_->GetModelPointer()->SetPos(FramPosition("hand.R_end"));
	}
	else {
		status_.situation.isCanBeCarried = false;
	}

	ChangeAnimIdle();
	MovingUpdate(input,objManager);

	//�󒆂ɂ���Ƃ�
	//�d�͂��x�N�g���ɑ����ă|�W�V�����ɑ���
	if (!debugCreativeMode) {
		if (status_.jump.isJump) {
			status_.jump.jumpVec += gravity;
			status_.pos.y += status_.jump.jumpVec;
			model_->SetPos(status_.pos);
		}
		else {
			status_.jump.jumpVec = 0.0f;
		}
	}
	

	//�����^�ђ���������
	//�ȍ~�̏������s��Ȃ�
	if (status_.situation.isInTransit) {
		return;
	}

	//�����o�֐��|�C���^��runningJumpUpdate�A
	//jumpUpdate�̂ǂ��炩�ɕύX����
	if (!debugCreativeMode) {
		if (input.IsTriggered(InputType::space)) {
			if (!status_.jump.isJump) {
				PlayerJump(playerInfo_.jumpPower);
			}
			ChangeAnimNo(AnimType::jump, false, 20);
			updateFunc_ = &Player::JumpUpdate;
			return;
		}
	}
	else {
		if (input.IsPressed(InputType::space)) {
			status_.moveVec.y = 3.0;
		}
	}

	//�����o�֐��|�C���^��DeathUpdate�ɕύX����
	if (input.IsTriggered(InputType::death)) {
		deathCount_++;
		updateFunc_ = &Player::DeathUpdate;
		status_.moveVec = VGet(0, 0, 0);
		return;
	}

}

/// <summary>
/// �A�j���[�V������idle�ɖ߂��֐�
/// </summary>
void Player::ChangeAnimIdle()
{
	//�ҋ@�A�j���[�V�����ɖ߂�
	if (!status_.situation.isMoving) {

		if (status_.situation.isInTransit) {
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
void Player::MovingUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	
	float movingSpeed = Move(input);

	//HACK�F�����Ƃ����A�j���[�V�����ԍ��ύX������͂�
	if (movingSpeed != 0.0f) {
		if (movingSpeed > playerInfo_.walkSpeed) {
			//�A�j���[�V�����̕ύX
			if (status_.situation.isInTransit) {
				ChangeAnimNo(AnimType::carryRunning, true, 20);
			}
			else {
				ChangeAnimNo(AnimType::run, true, 20);
			}
		}
		else if (movingSpeed <= playerInfo_.walkSpeed) {
			//�A�j���[�V�����̕ύX
			if (status_.situation.isInTransit) {
				ChangeAnimNo(AnimType::carryWalking, true, 20);
			}
			else {
				ChangeAnimNo(AnimType::walk, true, 20);
			}
		}
	}

	if (VSize(status_.moveVec) == 0.0f) {
		status_.situation.isMoving = false;
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

	status_.moveVec = { 0.0f,0.0f,0.0f };

	status_.situation.isMoving = false;
	float movingSpeed = 0.0f;

	if (pressedUp || pressedDown || pressedLeft || pressedRight) {
		movingSpeed = PlayerSpeed(pressedShift);
		status_.situation.isMoving = true;
	}

	//HACK�F�����A���t�@�N�^�����O�K�{
	if (pressedUp) {
		status_.moveVec.z = movingSpeed;
		targetAngle_ = 180.0f;
	}
	if (pressedDown) {
		status_.moveVec.z = -movingSpeed;
		targetAngle_ = 0.0f;
	}
	if (pressedLeft) {
		status_.moveVec.x = -movingSpeed;
		targetAngle_ = 90.0f;
	}
	if (pressedRight) {
		status_.moveVec.x = movingSpeed;
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
	model_->SetRot(DegreesToRadians(status_.rot));
}

//�I�u�W�F�N�g��o��
void Player::ClimUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	if (model_->IsAnimEnd()) {
		status_.pos = CenterFramPosition("mixamorig:LeftToeBase", "mixamorig:RightToeBase");
		model_->SetPos(status_.pos);

		status_.animNo = animType_[AnimType::stand];
		status_.isAnimLoop = false;
		model_->SetAnimation(status_.animNo, status_.isAnimLoop, true);
		updateFunc_ = &Player::StandUpdate;
	}
}

//HACK:�������A�C�ɐH��Ȃ�
/// <summary>
/// ����W�����v�ł͂Ȃ��Ƃ��̃W�����v
/// </summary>
/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
void Player::JumpUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	//�v���C���[�ړ��֐�
	Move(input);

	//�W�����v����
	{
		//�W�����v�x�N�g����0�ŃW�����v���ł͂Ȃ�������
		//idle��Ԃ̃A�b�v�f�[�g�ɕύX����A�A�j���[�V�������ύX����
		if (!status_.jump.isJump) {
			updateFunc_ = &Player::NormalUpdate;
			return;
		}

		status_.jump.jumpVec += gravity;
		status_.pos.y += status_.jump.jumpVec;
		model_->SetPos(status_.pos);

	}
}


//HACK:�������A�C�ɐH��Ȃ�
/// <summary>
/// �v���C���[�������Ă���Ƃ��̃W�����v
/// </summary>
/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
void Player::RunningJumpUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	//�v���C���[�ړ��֐�
	Move(input);

	//HACK�F�ϐ������̂܂܁@+�@�ǂ����邩�Y��ł���
	//�A�j���[�V�����̑����Ԃɂ���āA�d�͂�ύX����
	totalAnimFrame_ = model_->GetAnimTotalTime() + 2;
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
		updateFunc_ = &Player::NormalUpdate;
		return;
	}
}

/// <summary>
/// �v���C���[�̎��̂ɗ^����������֐�
/// </summary>
/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
void Player::DeathUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	deathPos_ = status_.pos;				//���񂾏ꏊ���c��

	//����A�j���[�V�����ȊO�������玀�ʃA�j���[�V�����ɕς���
	if (status_.animNo != animType_[AnimType::idleToSitup]) {
		//�A�j���[�V�����̕ύX
		ChangeAnimNo(AnimType::death, false, 20);
	}

	if (model_->IsAnimEnd()) {
		DeathPersonPostProsessing(objManager);
	}

}

//���̂̌㏈��
void Player::DeathPersonPostProsessing(std::shared_ptr<ObjectManager> objManager)
{
//	status_.pos = checkPoint_;				//�`�F�b�N�|�C���g�Ƀv���C���[���A��

	DeadPersonGenerater(objManager);			//���̂𐶐�����֐�

	updateFunc_ = &Player::NormalUpdate;
}

/// <summary>
/// �v���C���[�̎��̂�vector�z��ŊǗ�����֐�
/// </summary>
void Player::DeadPersonGenerater(std::shared_ptr<ObjectManager> objManager)
{
	LoadObjectInfo info;
	info.rot = DegreesToRadians(status_.rot);
	info.pos = deathPos_;
	info.scale = scale_;

	objManager->DeadPersonGenerator(model_->GetModelHandle(),info, status_.animNo);
}

/// <summary>
/// �v���C���[�ɍ���A�j���[�V������������֐�
/// </summary>
/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
void Player::SitUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	//�����オ�邽�߂̃R�}���h
	if (input.IsTriggered(InputType::activate)) {
		//�A�j���[�V�����̕ύX
		ChangeAnimNo(AnimType::situpToIdle, false, 20);
	}
	
	//���ƒ�̃A�j���[�V�������I�������idleupdate�ɕύX����
	if (status_.animNo == animType_[AnimType::situpToIdle] && model_->IsAnimEnd()) {
		updateFunc_ = &Player::NormalUpdate;
		status_.situation.isSitting = false;
		return;
	}

	if (status_.animNo == animType_[AnimType::situpToIdle]) return;

	//���ʃR�}���h
	if (input.IsTriggered(InputType::death)) {
		DeathUpdate(input,objManager);
		status_.situation.isSitting = false;
		updateFunc_ = &Player::NormalUpdate;
		return;
	}

}

void Player::IdleToSitup(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{

	status_.moveVec = VGet(0.0f, 0.0f, 0.0f);

	//�A�j���[�V����������ߒ��̃A�j���[�V�����ɕύX
	//�����Ă���t���O�𗧂āA�A�j���[�V�������[�v�ϐ���܂�
	if (!status_.situation.isSitting) {
		status_.situation.isSitting = true;
		ChangeAnimNo(AnimType::idleToSitup, false, 20);
	}

	//����ߒ��̃A�j���[�V�������I�������O�p����ɂ���
	if (status_.animNo == animType_[AnimType::idleToSitup] && model_->IsAnimEnd()) {
		model_->SetAnimEndFrame(status_.animNo);
		updateFunc_ = &Player::SitUpdate;
	}

}

//�����オ�鏈��
void Player::StandUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	if (model_->IsAnimEnd()) {
		updateFunc_ = &Player::NormalUpdate;
		status_.situation.isClim = false;
	}
}

void Player::SetCarryInfo(bool isCarry, shared_ptr<ObjectBase>model) {
	status_.situation.isCanBeCarried = isCarry;
	deadPersonModelPointer_ = model;
}

void Player::SetGimmickModelPointer(std::shared_ptr<ManualCrank> crank) {
	crank_ = crank;
	status_.situation.isGimmickCanBeOperated = true;
}

void Player::CarryObjectUpdate()
{
	
	if (!status_.situation.isCanBeCarried) return;

	status_.situation.isInTransit = true;

	deadPersonModelPointer_->SetIsTransit(true);

	carryUpdateFunc_ = &Player::DropOffObjectUpdate;

}

//�^��ł����I�u�W�F�N�g�����낷����
void Player::DropOffObjectUpdate()
{
	bool isCarryWalking = status_.animNo == animType_[AnimType::carryWalking];
	bool isCarry = status_.animNo == animType_[AnimType::carryIdle];
	if ((isCarryWalking || isCarry) && status_.situation.isCanBeCarried) {
		status_.situation.isCanBeCarried = false;
		deadPersonModelPointer_->SetIsTransit(false);
		deadPersonModelPointer_->GetModelPointer()->SetPos(CenterFramPosition("foot.L", "foot.R"));
		deadPersonModelPointer_.reset();
	}

	status_.situation.isInTransit = false;
	status_.situation.isCanBeCarried = false;

	carryUpdateFunc_ = &Player::CarryObjectUpdate;

}

void Player::CrankUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager) 
{

	status_.moveVec = VGet(0, 0, 0);

	//VECTOR pos = crank_->GetModelPointer()->GetPos();
	float rotZ = crank_->GetRotZ();

	if (input.IsPressed(InputType::down)) {
		rotZ = (std::max)(rotZ - 3.0f, -630.0f);
		CrankRotatinUpdate(rotZ);
	}
	else if (input.IsPressed(InputType::up)) {
		rotZ = (std::min)(rotZ + 3.0f, 0.0f);
		CrankRotatinUpdate(rotZ);
	}

	if (input.IsTriggered(InputType::activate)) {
		status_.situation.isGimmickCanBeOperated = false;
		crank_.reset();
		updateFunc_ = &Player::NormalUpdate;
	}

}

void Player::CrankRotatinUpdate(float rotZ) {

	float radian = rotZ * DX_PI_F / 180.0f;

	int frameNo = MV1SearchFrame(crank_->GetModelPointer()->GetModelHandle(), "Crank");

	VECTOR pos = MV1GetFramePosition(crank_->GetModelPointer()->GetModelHandle(), frameNo);
	//�N�����N�̃|�W�V��������I�u�W�F�N�g�S�̂̃|�W�V����������������
	VECTOR distance = { 0,2.0f,0 };

	MATRIX mat = {};

	float x = 90 * DX_PI_F / 180.0f;

	//���s�ړ��s��
	MATRIX posMat = MGetTranslate(distance);
	//��]�s��
	MATRIX rotMatZ = MGetRotZ(radian);
	MATRIX rotMatX = MGetRotX(x);

	mat = MMult(rotMatX, rotMatZ);
	mat = MMult(mat, posMat);

	MV1SetFrameUserLocalMatrix(crank_->GetModelPointer()->GetModelHandle(), frameNo, mat);

	crank_->SetRotZ(rotZ);
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

void Player::BulletHitMeUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	//�d��
	status_.jump.jumpVec += gravity;
	status_.moveVec.y = status_.jump.jumpVec;

	//�m�b�N�o�b�N
	status_.moveVec = VScale(status_.moveVec, 0.96f);

	status_.pos = VAdd(status_.pos, status_.moveVec);

	model_->SetPos(status_.pos);

	float moveVecSize = VSize(status_.moveVec);

	if (moveVecSize < 2.0f) {
		updateFunc_ = &Player::NormalUpdate;
	}

}

void Player::BulletHitMe(VECTOR moveVec)
{
	status_.moveVec = moveVec;

	updateFunc_ = &Player::BulletHitMeUpdate;
}

void Player::ChangeAnimNo(AnimType type, bool isAnimLoop, int changeTime)
{
	status_.animNo = animType_[type];
	status_.isAnimLoop = isAnimLoop;
	model_->ChangeAnimation(status_.animNo, status_.isAnimLoop, false, changeTime);
}

//�v���C���[�̑��x�ݒ�
float Player::PlayerSpeed(bool pressedShift)
{
	if (pressedShift) {
		if (debugCreativeMode) {
			return playerInfo_.runningSpeed * 2;
		}
		return playerInfo_.runningSpeed;
	} 
	
	return playerInfo_.walkSpeed;
}

//�W�����v�̐ݒ�
void Player::PlayerJump(float jumpPower) {
	status_.jump.jumpVec = jumpPower;
	status_.pos.y += status_.jump.jumpVec;
	status_.jump.isJump = true;
}

//��̃t���[�����W�̒��S���擾����
VECTOR Player::CenterFramPosition(const char* const LeftFramename, const char* const RightFramename)
{

	VECTOR framePosition;

	//�w��t���[���̍��W���擾����B
	framePosition = model_->GetFrameLocalPosition(LeftFramename);
	framePosition = VAdd(framePosition, model_->GetFrameLocalPosition(RightFramename));
	//��̍��W�𑫂��A2�Ŋ��蒆�S���擾����
	framePosition.x = framePosition.x / 2;
	framePosition.y = framePosition.y / 2;
	framePosition.z = framePosition.z / 2;

	return framePosition;
}

VECTOR Player::FramPosition(const char* const framename)
{

	VECTOR framePosition;

	//�w��t���[���̍��W���擾����B
	framePosition = model_->GetFrameLocalPosition(framename);

	return framePosition;
}

void Player::DrawPolygon3D()
{
	VERTEX3D vertex[7];
	WORD index[18];

	float angle = 0.0f;

	vertex[0].pos = VGet(status_.pos.x, roundShadowHeight_, status_.pos.z);
	vertex[0].norm = VGet(0.0f, 1.0f, 0.0f);
	vertex[0].dif = GetColorU8(51, 51, 51, 125);
	vertex[0].spc = GetColorU8(0, 0, 0, 0);
	vertex[0].u = 0.0f;
	vertex[0].v = 0.0f;
	vertex[0].su = 0.0f;
	vertex[0].sv = 0.0f;

	for (int i = 1; i < 7; i++) {
		vertex[i].pos = VertexPosition(angle);
		vertex[i].norm = VGet(0.0f, 1.0f, 0.0f);
		vertex[i].dif = GetColorU8(51, 51, 51, 125);
		vertex[i].spc = GetColorU8(0, 0, 0, 0);
		vertex[i].u = 0.0f;
		vertex[i].v = 0.0f;
		vertex[i].su = 0.0f;
		vertex[i].sv = 0.0f;
		angle += 60.0f;

	}

	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 0;
	index[4] = 2;
	index[5] = 3;
	index[6] = 0;
	index[7] = 3;
	index[8] = 4;
	index[9] = 0;
	index[10] = 4;
	index[11] = 5;
	index[12] = 0;
	index[13] = 5;
	index[14] = 6;
	index[15] = 0;
	index[16] = 6;
	index[17] = 1;

	DrawPolygonIndexed3D(vertex, 7, index, 6, DX_NONE_GRAPH, true);
}

VECTOR Player::VertexPosition(float angle)
{

	VECTOR pos = {};

	float radian = angle * DX_PI_F / 180.0f;

	pos.x = sin(radian);
	pos.z = cos(radian);
	pos.y = 0.0f;

	pos = VScale(pos, 25.0f);

	pos = VAdd(status_.pos, pos);

	pos.y = roundShadowHeight_;

	return pos;
}
