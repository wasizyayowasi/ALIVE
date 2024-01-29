#include "Player.h"

#include "../object/DeadPerson.h"
#include "../object/ObjectManager.h"

#include "../gimmick/ManualCrank.h"
#include "../gimmick/Lever.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/ModelManager.h"

#include<algorithm>
#include<string>

namespace {
	//�d��
	constexpr float gravity = -0.4f;

	//�t���[���̖��O
	const char* const frame_name = "hand.R_end";

	//�v���C���[�̍���
	constexpr float player_hegiht = 130.0f;
}

using namespace std;

Player::Player():updateFunc_(&Player::NormalUpdate),carryUpdateFunc_(&Player::CarryObjectUpdate)
{
	//�W�����v���̏���
	status_.jump.isJump = false;
	status_.jump.jumpVec = 0.0f;
	//�v���C���[�̍����̐ݒ�
	status_.height = player_hegiht;
	deathCount_ = 3;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
}

void Player::Init(LoadObjectInfo info)
{
	//�Z�k��
	auto& model = ModelManager::GetInstance();

	//�v���C���[���̏�����
	playerInfo_ = ExternalFile::GetInstance().GetPlayerInfo();
	
	//�v���C���[���f���̐���
	model_ = make_shared<Model>(model.GetModelHandle(ObjectType::Player));

	//�A�j���[�V�����̐ݒ�
	model_->SetAnimation(static_cast<int>(PlayerAnimType::Idle), true, false);

	//�v���C���[�̑傫���̒���
	model_->SetScale(info.scale);

	//�|�W�V�����̐ݒ�
	model_->SetPos(info.pos);
	status_.pos = info.pos;
//	status_.pos = VGet(0,41,0);

	//��]���̐ݒ�
	model_->SetRot(info.rot);

	//�R���W�����t���[���̐ݒ�
	model_->SetUseCollision(true,true,"Character");

	scale_ = info.scale;
}

void Player::Update(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	//�v���C���[�̃A�j���[�V�����X�V
	model_->Update();
	
	(this->*updateFunc_)(input,objManager);
}


void Player::Draw()
{
	//���f���̕`��
	model_->Draw();

	//�e���ǂ��̕`��
	DrawPolygon3D();

	DrawFormatString(0, 64, 0xffffff, "normal %.2f,%.2f,%.2f", status_.pos.x,status_.pos.y,status_.pos.z);
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
	//�폜�\��
	if (input.IsTriggered(InputType::creative)) {
		debugCreativeMode = !debugCreativeMode;
	}
	
	//�I�u�W�F�N�g�ɑ΂��ăA�N�V�������N����
	if (input.IsTriggered(InputType::activate)) {
		status_.moveVec = VGet(0, 0, 0);
		if (status_.situation.isCanBeCarried) {
			(this->*carryUpdateFunc_)();
		}
		else if (status_.situation.isGimmickCanBeOperated) {
			if (crank_ != nullptr) {
				//�N�����N�𓮂�������������
				updateFunc_ = &Player::GoCrankRotationPosition;
			}
			else if (lever_ != nullptr) {
				//���o�[�𓮂�������������
				updateFunc_ = &Player::GoLeverPullPosition;
			}
			return;
		}
	}
	else {
		crank_.reset();
		lever_.reset();
	}

	if (input.IsTriggered(InputType::sit)) {
		updateFunc_ = &Player::IdleToSitup;
		return;
	}

	status_.situation.isGimmickCanBeOperated = false;

	if (status_.situation.isInTransit) {
		//dynamic_pointer_cast<DeadPerson>(deadPersonModelPointer_)->Init();
		deadPersonModelPointer_->GetModelPointer()->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));
		VECTOR framePos = model_->GetFrameLocalPosition(frame_name);
		deadPersonModelPointer_->GetModelPointer()->SetPos(framePos);
	}
	else {
		status_.situation.isCanBeCarried = false;
	}

	ChangeAnimIdle();
	MovingUpdate(input);

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
			ChangeAnimNo(PlayerAnimType::jump, false, 20);
			updateFunc_ = &Player::JumpUpdate;
			return;
		}
	}
	else {
		if (input.IsPressed(InputType::space)) {
			status_.moveVec.y = 10.0;
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
		ChangeAnimNo(PlayerAnimType::Idle, true, 20);
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
			ChangeAnimNo(PlayerAnimType::Run, true, 20);
		}
		else if (movingSpeed <= playerInfo_.walkSpeed) {
			//�A�j���[�V�����̕ύX
			ChangeAnimNo(PlayerAnimType::Walk, true, 20);
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
	bool pressedShift = input.IsPressed(InputType::dush);

	status_.moveVec = VGet(0,0,0);

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

	VECTOR destination = VAdd(status_.pos, status_.moveVec);
	if (destination.z < -250.0f) {
		status_.moveVec.z = 0.0f;
	}

	return movingSpeed;

}

//�����i�����獡�ア����Ȃ��Ă����Ǝv��
/// <summary>
/// �v���C���[�̉�]�������s���֐�
/// </summary>
void Player::RotationUpdate()
{
	//���]�̊p�x
	float oneRevolution = 360.0f;

	//�ڕW�̊p�x���猻�݂̊p�x�������č����o���Ă���
	differenceAngle_ = targetAngle_ - angle_;

	//��Ƀv���C���[���f�������肳�������Ȃ��̂�
	//181�x����-181�x�ȏ�̏ꍇ�A�t���ɂ��Ă�����
	if (differenceAngle_ >= oneRevolution / 2) {
		differenceAngle_ = targetAngle_ - angle_ - oneRevolution;
	}
	else if (differenceAngle_ <= -oneRevolution / 2) {
		differenceAngle_ = targetAngle_ - angle_ + oneRevolution;
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
	if (angle_ == oneRevolution || angle_ == -oneRevolution) {
		angle_ = 0.0f;
	}
	if (status_.rot.y == oneRevolution || status_.rot.y == -oneRevolution) {
		status_.rot.y = 0.0f;
	}

	//���ʂ����f���̉�]���Ƃ��đ���
	model_->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));
}

//HACK:�������A�C�ɐH��Ȃ�
/// ����W�����v�ł͂Ȃ��Ƃ��̃W�����v
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

// �v���C���[�̎��̂ɗ^����������֐�
void Player::DeathUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	//����A�j���[�V�����ȊO�������玀�ʃA�j���[�V�����ɕς���
	if (status_.animNo != static_cast<int>(PlayerAnimType::idleToSitup)) {
		//�A�j���[�V�����̕ύX
		ChangeAnimNo(PlayerAnimType::Death, false, 20);
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

// �v���C���[�̎��̂�vector�z��ŊǗ�����֐�
void Player::DeadPersonGenerater(std::shared_ptr<ObjectManager> objManager)
{
	LoadObjectInfo info;
	info.rot = MathUtil::VECTORDegreeToRadian(status_.rot);
	info.scale = scale_;

	int frameNo = MV1SearchFrame(model_->GetModelHandle(), "PlaceToPutTheCorpse");
	VECTOR putPos = MV1GetFramePosition(model_->GetModelHandle(), frameNo);
	info.pos = putPos;

	objManager->DeadPersonGenerator(model_->GetModelHandle(),info, status_.animNo);
}

// �v���C���[�ɍ���A�j���[�V������������֐�
void Player::SitUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	//�����オ�邽�߂̃R�}���h
	if (input.IsTriggered(InputType::activate)) {
		//�A�j���[�V�����̕ύX
		ChangeAnimNo(PlayerAnimType::situpToIdle, false, 20);
	}
	
	//���ƒ�̃A�j���[�V�������I�������idleupdate�ɕύX����
	if (status_.animNo == static_cast<int>(PlayerAnimType::situpToIdle) && model_->IsAnimEnd()) {
		updateFunc_ = &Player::NormalUpdate;
		status_.situation.isSitting = false;
		return;
	}

	if (status_.animNo == static_cast<int>(PlayerAnimType::situpToIdle)) return;

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
		ChangeAnimNo(PlayerAnimType::idleToSitup, false, 20);
	}

	//����ߒ��̃A�j���[�V�������I�������O�p����ɂ���
	if (status_.animNo == static_cast<int>(PlayerAnimType::idleToSitup) && model_->IsAnimEnd()) {
		model_->SetAnimEndFrame(status_.animNo);
		updateFunc_ = &Player::SitUpdate;
	}

}

void Player::SetCarryInfo(bool isCarry, shared_ptr<ObjectBase>model) {
	status_.situation.isCanBeCarried = isCarry;
	deadPersonModelPointer_ = model;
}

void Player::SetCrankPointer(std::shared_ptr<ManualCrank> crank) {
	crank_ = crank;
	status_.situation.isGimmickCanBeOperated = true;
}

void Player::SetLeverPointer(std::shared_ptr<Lever> lever)
{
	lever_ = lever;
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
	if (status_.situation.isCanBeCarried) {
		status_.situation.isCanBeCarried = false;
		deadPersonModelPointer_->SetIsTransit(false);
		int frameNo = MV1SearchFrame(model_->GetModelHandle(), "PlaceToPutTheCorpse");
		VECTOR putPos = MV1GetFramePosition(model_->GetModelHandle(), frameNo);
		deadPersonModelPointer_->GetModelPointer()->SetPos(putPos);
		deadPersonModelPointer_.reset();
	}

	status_.situation.isInTransit = false;
	status_.situation.isCanBeCarried = false;

	carryUpdateFunc_ = &Player::CarryObjectUpdate;

}

void Player::CrankUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager) 
{

	status_.moveVec = VGet(0, 0, 0);

	float rotZ = crank_->GetRotZ();

	if (input.IsPressed(InputType::down)) {
		rotZ = (std::max)(rotZ - 3.0f, crank_->GetMaxRotZ());
		CrankRotationUpdate(rotZ);
	}
	else if (input.IsPressed(InputType::up)) {
		rotZ = (std::min)(rotZ + 3.0f, 0.0f);
		CrankRotationUpdate(rotZ);
	}

	int naturalNumber = (std::max)(rotZ, -rotZ);
	float animTime = static_cast<float>(naturalNumber % 360) / 3;

	model_->SetAnimationFrame(animTime);

	if (input.IsTriggered(InputType::activate)) {
		status_.situation.isGimmickCanBeOperated = false;
		crank_.reset();
		updateFunc_ = &Player::NormalUpdate;
	}

}

void Player::CrankRotationUpdate(float rotZ) {

	float radian = MathUtil::DegreeToRadian(rotZ);

	int frameNo = MV1SearchFrame(crank_->GetModelPointer()->GetModelHandle(), "Crank");

	VECTOR pos = MV1GetFramePosition(crank_->GetModelPointer()->GetModelHandle(), frameNo);
	//�N�����N�̃|�W�V��������I�u�W�F�N�g�S�̂̃|�W�V����������������
	VECTOR distance = { 0,1.8f,0 };

	MATRIX mat = {};

	float x = MathUtil::DegreeToRadian(90.0f);

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

void Player::GoLeverPullPosition(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	//�N�����N�̗����Ăق����|�W�V�������擾����
	VECTOR standPos = lever_->GetStandingPosition();
	//�����Ăق����|�W�V�����ƃv���C���[�̋����̃T�C�Y���擾����
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(status_.pos, standPos);

	//distanceSize�����͈̔͊O��������
	//���̑��x�ŗ����Ăق����|�W�V�����Ɍ�����
	if (distanceSize > 3.0f) {
		VECTOR distance = VNorm(VSub(standPos, status_.pos));
		VECTOR moveVec = VScale(distance, playerInfo_.walkSpeed);
		status_.pos = VAdd(status_.pos, moveVec);
		model_->SetPos(status_.pos);
	}
	//distanceSize�����͈͓̔��ɓ�������
	//�����Ăق����|�W�V�������v���C���[�̃|�W�V�����Ƃ���
	else {
		model_->SetPos(standPos);
		angle_ = 0.0f;
		status_.rot = VGet(0, angle_, 0);
		model_->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));
		ChangeAnimNo(PlayerAnimType::LeverOn, false, 10);
		lever_->OnAnimation();
		updateFunc_ = &Player::LeverUpdate;
	}
}

void Player::LeverUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	if (!lever_->IsOn()) {
		updateFunc_ = &Player::NormalUpdate;
	}
}

//�N�����N���񂷂��߂ɃN�����N���񂷃|�W�V�����ւƈړ�����
void Player::GoCrankRotationPosition(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	//�N�����N�̗����Ăق����|�W�V�������擾����
	VECTOR standPos = crank_->GetStandingPosition();
	//�����Ăق����|�W�V�����ƃv���C���[�̋����̃T�C�Y���擾����
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(status_.pos, standPos);

	//distanceSize�����͈̔͊O��������
	//���̑��x�ŗ����Ăق����|�W�V�����Ɍ�����
	if (distanceSize > 3.0f) {
		VECTOR distance = VNorm(VSub(standPos, status_.pos));
		VECTOR moveVec = VScale(distance, playerInfo_.walkSpeed);
		status_.pos = VAdd(status_.pos, moveVec);
		model_->SetPos(status_.pos);
	}
	//distanceSize�����͈͓̔��ɓ�������
	//�����Ăق����|�W�V�������v���C���[�̃|�W�V�����Ƃ���
	else {
		model_->SetPos(standPos);
		angle_ = -90.0f;
		status_.rot = VGet(0, angle_, 0);
		model_->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));
		ChangeAnimNo(PlayerAnimType::crank, false, 20);
		updateFunc_ = &Player::CrankUpdate;
	}

}

void Player::BulletHitMe(VECTOR moveVec)
{
	status_.moveVec = moveVec;

	updateFunc_ = &Player::BulletHitMeUpdate;
}

void Player::ChangeAnimNo(PlayerAnimType type, bool isAnimLoop, int changeTime)
{
	status_.animNo = static_cast<int>(type);
	status_.isAnimLoop = isAnimLoop;
	model_->ChangeAnimation(status_.animNo, status_.isAnimLoop, false, changeTime);
}

//�v���C���[�̑��x�ݒ�
float Player::PlayerSpeed(bool pressedShift)
{
	//�V�t�g��������Ă��邩�ǂ���
	if (pressedShift) {
		if (debugCreativeMode) {
			return playerInfo_.runningSpeed * 3;
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

//�����e���쐬�A�`��
void Player::DrawPolygon3D()
{
	//���_�̐����z������
	VERTEX3D vertex[7] = {};
	//�O�p�`���쐬���鏇�Ԃ�ۑ�����z��
	WORD index[18] = {};

	//�J���[
	COLOR_U8 difColor = GetColorU8(51, 51, 51, 125);
	COLOR_U8 spcColor = GetColorU8(0, 0, 0, 0);

	//�@���x�N�g��
	VECTOR norm = VGet(0.0f, 1.0f, 0.0f);

	//�p�x�Ɋ���Ē��_�̈ʒu��ύX����
	float angle = 0.0f;

	//�Z�p�`�̒��S���W���擾
	vertex[0].pos = VGet(status_.pos.x, roundShadowHeight_, status_.pos.z);
	vertex[0].norm = norm;
	vertex[0].dif = difColor;
	vertex[0].spc = GetColorU8(0, 0, 0, 0);
	vertex[0].u = 0.0f;
	vertex[0].v = 0.0f;
	vertex[0].su = 0.0f;
	vertex[0].sv = 0.0f;

	//�p�x���Ƃ̒��_���擾
	for (int i = 1; i < 7; i++) {
		vertex[i].pos = VertexPosition(angle);
		vertex[i].norm = norm;
		vertex[i].dif = difColor;
		vertex[i].spc = spcColor;
		vertex[i].u = 0.0f;
		vertex[i].v = 0.0f;
		vertex[i].su = 0.0f;
		vertex[i].sv = 0.0f;
		angle += 60.0f;
	}

	//�O�p�`���쐬���鏇��
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

//�v���C���[�̗����e�Ɏg�p���钸�_���擾
VECTOR Player::VertexPosition(float angle)
{

	VECTOR pos = {};

	//�x���@���ʓx�@�ɕϊ�����
	float radian = MathUtil::DegreeToRadian(angle);
	
	//�p�x�ɂ����W���擾����
	pos.x = sin(radian);
	pos.z = cos(radian);
	pos.y = 0.0f;

	//�|�W�V������25�{����(�T�C�Y����)
	pos = VScale(pos, 25.0f);

	//�v���C���[�̃|�W�V�����Ə�L�Ŏ擾�����|�W�V�����𑫂�
	pos = VAdd(status_.pos, pos);

	//Y���W�͐��ƃ|���S���̓����蔻��Ŏ擾����
	//��ԋ߂��|���S����Y���W��������
	pos.y = roundShadowHeight_;

	return pos;
}
