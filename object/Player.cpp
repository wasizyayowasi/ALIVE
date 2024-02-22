#include "Player.h"

#include "../object/Corpse.h"
#include "../object/ObjectManager.h"

#include "../gimmick/Lever.h"
#include "../gimmick/ManualCrank.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/ModelManager.h"
#include "../util/EffectManager.h"

#include <cmath>
#include <string>
#include <algorithm>

namespace {
	//�d��
	constexpr float gravity = -0.4f;

	//�t���[���̖��O
	const char* const frame_name = "hand.R_end";

	//�v���C���[�̍���
	constexpr float player_hegiht = 130.0f;

	//���̂̐����J�E���g����ő吔
	constexpr int max_death_count = 99;

	//�������ʃx�N�g��
	constexpr VECTOR front_vec = { 0,0,-1 };
}

//�R���X�g���N�^
Player::Player(const LoadObjectInfo& info):updateFunc_(&Player::NormalUpdate),carryUpdateFunc_(&Player::CarryObject)
{
	//�Z�k��
	auto& model = ModelManager::GetInstance();
	auto& file = ExternalFile::GetInstance();

	//�v���C���[���̏�����
	playerInfo_ = ExternalFile::GetInstance().GetPlayerInfo();

	//�v���C���[���f���̐���
	model_ = std::make_shared<Model>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Player)].name), Material::Other);

	//�A�j���[�V�����̐ݒ�
	model_->SetAnimation(static_cast<int>(PlayerAnimType::Idle), true, false);

	//�����|�W�V�����̎擾
	VECTOR startPos = file.GetStartPos(file.GetStartName());

	//�|�W�V�����̐ݒ�
	model_->SetPos(startPos);

	//��]���̐ݒ�
	model_->SetRot(info.rot);

	//�v���C���[�̑傫���̒���
	model_->SetScale(info.scale);

	//�R���W�����t���[���̐ݒ�
	model_->SetUseCollision(true, true, "Character");

	//�|�W�V�����̏�����
	status_.pos = startPos;

	//�g�k���̏�����
	scale_ = info.scale;

	//�W�����v���̏���
	status_.jump.isJump = false;
	status_.jump.jumpVec = 0.0f;
	//�v���C���[�̍����̐ݒ�
	status_.height = player_hegiht;

	tentativeRot_ = front_vec;
}

// �f�X�g���N�^
Player::~Player()
{
	ExternalFile::GetInstance().SetStartName("");
}

//�X�V
void Player::Update(const std::shared_ptr<ObjectManager>& objManager)
{
	//�v���C���[�̃A�j���[�V�����X�V
	model_->Update();
	
	(this->*updateFunc_)(objManager);
}

//�`��
void Player::Draw()
{
	//���f���̕`��
	model_->Draw();

	//�e���ǂ��̕`��
	DrawPolygon3D();
}

//�e�ɓ���������m�b�N�o�b�N��ǉ�����
void Player::BulletHitMe(const VECTOR& moveVec)
{
	//�v���C���[�����S���������炽�܂ɂ��m�b�N�o�b�N�𖳌��ɂ���
	if (updateFunc_ == &Player::DeathUpdate) 
	{
		return;
	}

	//�m�b�N�o�b�N��ǉ�����
	status_.moveVec = moveVec;

	//�����o�֐���ύX����
	updateFunc_ = &Player::BulletHitMeUpdate;
}

//�|�W�V�����̐ݒ�
void Player::SetPos(const VECTOR& pos)
{
	//�X�e�[�^�X�Ƀ|�W�V������������
	status_.pos = pos;

	//���f���Ƀ|�W�V������ݒ肷��
	model_->SetPos(status_.pos);
}

//�W�����v�̐ݒ�
void Player::SetJumpInfo(bool isJump, float jumpVec)
{
	//�W�����v���Ă��邩�̃t���O��������
	status_.jump.isJump = isJump;

	//�W�����v�x�N�g����ݒ肷��
	status_.jump.jumpVec = jumpVec;

	//�|�W�V�����ɃW�����v�x�N�g���𑫂�
	status_.pos.y += status_.jump.jumpVec;
}

//�����^�Ԏ����o����t���O�Ǝ����^�ԃ��f���̃|�C���^���󂯎��
void Player::SetCarryInfo(const bool isCarry, const std::shared_ptr<ObjectBase>& model)
{
	//�����^�ђ��t���O��ݒ肷��
	status_.situation.isCanBeCarried = isCarry;

	//���̂̃��f���|�C���^���擾
	corpseModelPointer_ = model;
}

//ManualCrank�̃|�C���^��ݒ肷��
void Player::SetCrankPointer(const std::shared_ptr<ManualCrank>& crank)
{
	//���쒆�t���O��true�ɐݒ肷��
	status_.situation.isGimmickCanBeOperated = true;

	//�N�����N�̃|�C���^��ݒ肷��
	crank_ = crank;
}

//���o�[�̃|�C���^��ݒ肷��
void Player::SetLeverPointer(const std::shared_ptr<Lever>& lever)
{
	//���쒆�t���O��true�ɐݒ肷��
	status_.situation.isGimmickCanBeOperated = true;

	//���o�[�̃|�C���^��ݒ肷��
	lever_ = lever;
}

//�n�ʂɕ`�悷��e�̍����Ɠ���ł���I�u�W�F�N�g�̑f�ނ�ݒ肷��
void Player::SetRoundShadowHeightAndMaterial(const float height, const  Material materialType)
{
	//�e�̍�����ݒ肷��
	roundShadowHeight_ = height;

	//�����̃I�u�W�F�N�g�̑f�ނ�ݒ肷��
	materialSteppedOn_ = materialType;
}

//HACK:�������A�C�ɐH��Ȃ�
//�ʏ펞�̍X�V
void Player::NormalUpdate(const std::shared_ptr<ObjectManager>& objManager)
{
	//�Z�k��
	auto& input = InputState::GetInstance();

#ifdef _DEBUG
	//�폜�\��
	if (input.IsTriggered(InputType::Creative))
	{
		debugCreativeMode = !debugCreativeMode;
	}
#endif
	
	//�����^�щ\�ȃI�u�W�F�N�g�ɑ΂��ăA�N�V�������N����
	if (input.IsTriggered(InputType::Activate))
	{
		if (status_.situation.isCanBeCarried) 
		{
			(this->*carryUpdateFunc_)();
		}
	}

	//�����K�v�Ƃ���M�~�b�N�ɑ΂��ăA�N�V�������N����
	if (input.IsTriggered(InputType::Activate))
	{
		//�x�N�g����0�ɂ���
		status_.moveVec = VGet(0, 0, 0);

		//�M�~�b�N�𑀍�o����t���O��true�Ȃ��
		//�����o�֐���ύX����
		if (status_.situation.isGimmickCanBeOperated)
		{
			if (crank_ != nullptr)
			{
				//�N�����N�𓮂�������������
				updateFunc_ = &Player::GoCrankRotationPosition;
			}
			else if (lever_ != nullptr)
			{
				//���o�[�𓮂�������������
				updateFunc_ = &Player::GoLeverPullPosition;
			}

			return;
		}
	}
	else {
		//�N�����N�𑀍삵�Ȃ��ꍇ�|�C���^�[�����Z�b�g����
		crank_.reset();

		//���o�[�𑀍삵�Ȃ��ꍇ�|�C���^�[�����Z�b�g����
		lever_.reset();
	}

	status_.situation.isGimmickCanBeOperated = false;

	//���̂������^�ђ���
	if (status_.situation.isInTransit)
	{
		//�v���C���[�̉�]�ƂƂ��Ɏ��̂̃��f������]������
		corpseModelPointer_->GetModelPointer()->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));

		//���̂̃|�W�V��������Ƀv���C���[�̎�̍��W�ɂ���
		VECTOR framePos = model_->GetFrameLocalPosition(frame_name);
		corpseModelPointer_->GetModelPointer()->SetPos(framePos);
	}
	else
	{
		status_.situation.isCanBeCarried = false;
	}

	//�A�j���[�V������ҋ@�ɂ���
	ChangeAnimIdle();

	//�ړ�
	MovingUpdate();

	//������炷
	FootStepSound();

#ifdef _DEBUG
	//�󒆂ɂ���Ƃ�
	//�d�͂��x�N�g���ɑ����ă|�W�V�����ɑ���
	if (!debugCreativeMode)
	{
		if (status_.jump.isJump)
		{
			status_.jump.jumpVec += gravity;
			status_.pos.y += status_.jump.jumpVec;
			model_->SetPos(status_.pos);
		}
		else 
		{
			status_.jump.jumpVec = 0.0f;
		}
	}
#else
	//�󒆂ɂ���Ƃ�
	//�d�͂��x�N�g���ɑ����ă|�W�V�����ɑ���
	if (status_.jump.isJump) {
		status_.jump.jumpVec += gravity;
		status_.pos.y += status_.jump.jumpVec;
		model_->SetPos(status_.pos);
	}
	else {
		status_.jump.jumpVec = 0.0f;
	}
#endif // _DEBUG

	//�����^�ђ���������
	//�ȍ~�̏������s��Ȃ�
	if (status_.situation.isInTransit)
	{
		return;
	}

#ifdef _DEBUG
	//�����o�֐��|�C���^��jumpUpdate�ɕύX����
	if (!debugCreativeMode)
	{
		if (input.IsTriggered(InputType::Space))
		{
			if (!status_.jump.isJump)
			{
				SetJumpInfo(true,playerInfo_.jumpPower);
			}
			ChangeAnimNo(PlayerAnimType::Jump, false, 20);
			updateFunc_ = &Player::JumpUpdate;
			return;
		}
	}
	else {
		if (input.IsPressed(InputType::Space))
		{
			status_.moveVec.y = 10.0;
		}
	}
#else
	//�����o�֐��|�C���^��jumpUpdate�ɕύX����
	if (input.IsTriggered(InputType::Space))
	{
		//�W�����v���Ă���t���O��false��������
		//�W�����v�������s��
		if (!status_.jump.isJump) {
			SetJumpInfo(true, playerInfo_.jumpPower);
		}

		//�A�j���[�V�������W�����v�ɕύX����
		ChangeAnimNo(PlayerAnimType::Jump, false, 20);

		//�����o�֐���ύX����
		updateFunc_ = &Player::JumpUpdate;
		return;
	}
#endif // _DEBUG

	//�����o�֐��|�C���^��DeathUpdate�ɕύX����
	if (input.IsTriggered(InputType::Death))
	{
		//���̂̐����J�E���g����
		deathCount_ = (std::min)(deathCount_ + 1, max_death_count);

		//�ړ��x�N�g����0�ɂ���
		status_.moveVec = VGet(0, 0, 0);

		//�����o�֐���ύX����
		updateFunc_ = &Player::DeathUpdate;
		return;
	}
}

//HACK:�������A�C�ɐH��Ȃ�
//�ړ��̍X�V
void Player::MovingUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�ړ��X�s�[�h�̎擾
	float movingSpeed = Move();
	
	//tempRotationUpdate();
	
	//model_->SetPos(VAdd(status_.pos,status_.moveVec));

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

//�ړ�����
float Player::Move()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�L�[�̉������u�[���^�Ɋi�[
	bool pressedUp = input.IsPressed(InputType::Up);
	bool pressedDown = input.IsPressed(InputType::Down);
	bool pressedLeft = input.IsPressed(InputType::Left);
	bool pressedRight = input.IsPressed(InputType::Right);
	bool pressedShift = input.IsPressed(InputType::Dush);

	status_.moveVec = VGet(0,0,0);

	status_.situation.isMoving = false;
	float movingSpeed = 0.0f;

	if (pressedUp || pressedDown || pressedLeft || pressedRight) {
		movingSpeed = PlayerSpeed(pressedShift);
		status_.situation.isMoving = true;
	}

	//HACK�F�����A���t�@�N�^�����O�K�{
	if (pressedUp) {
		status_.moveVec.z += movingSpeed;
		targetAngle_ = 180.0f;
//		tentativeRot_ = { -0.01f,0,0.99f };
	}
	if (pressedDown) {
		status_.moveVec.z -= movingSpeed;
		targetAngle_ = 0.0f;
//		tentativeRot_ = { 0,0,-1 };
	}
	if (pressedLeft) {
		status_.moveVec.x -= movingSpeed;
		targetAngle_ = 90.0f;
//		tentativeRot_ = { -1,0,0 };
	}
	if (pressedRight) {
		status_.moveVec.x += movingSpeed;
		targetAngle_ = 270.0f;
//		tentativeRot_ = { 1,0,0 };
	}
	if (pressedUp && pressedRight) {
		targetAngle_ = 225.0f;
//		tentativeRot_ = { 1,0,1 };
	}
	if (pressedUp && pressedLeft) {
		targetAngle_ = 135.0f;
//		tentativeRot_ = { -1,0,1 };
	}
	if (pressedDown && pressedLeft) {
		targetAngle_ = 45.0f;
//		tentativeRot_ = { -1,0,-1 };
	}
	if (pressedDown && pressedRight) {
		targetAngle_ = 315.0f;
//		tentativeRot_ = { 1,0,-1 };
	}

	//��]����
	RotationUpdate();

	VECTOR destination = VAdd(status_.pos, status_.moveVec);
	if (destination.z < -250.0f) {
		status_.moveVec.z = 0.0f;
	}

	return movingSpeed;

}

//��]�̏���
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

void Player::tempRotationUpdate()
{
	//�s��
	MATRIX mtx = {};

	//��]�s��̎擾
	MATRIX rotMtx = MGetRotVec2(front_vec, VNorm(tentativeRot_));

	//�g�k�s��̎擾
	MATRIX scaleMtx = MGetScale(scale_);

	//���s�ړ��s��
	MATRIX moveMtx = MGetTranslate(status_.pos);

	//��]�s��Ɗg�k�s��̏�Z
	mtx = MMult(rotMtx, scaleMtx);

	//�i��]�s��Ɗg�k�s��j�ƕ��s�ړ��s��̏�Z
	mtx = MMult(mtx, moveMtx);

	MV1SetMatrix(model_->GetModelHandle(),mtx);
}

//HACK:�������A�C�ɐH��Ȃ�
void Player::JumpUpdate(const std::shared_ptr<ObjectManager>& objManager)
{
	//�v���C���[�ړ��֐�
	Move();

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
void Player::DeathUpdate(const std::shared_ptr<ObjectManager>& objManager)
{
	//�A�j���[�V�����̕ύX
	ChangeAnimNo(PlayerAnimType::Death, false, 20);

	if (model_->IsAnimEnd()) {
		CorpsePostProsessing(objManager);
	}
}

//���̂̌㏈��
void Player::CorpsePostProsessing(const std::shared_ptr<ObjectManager>& objManager)
{
	//���̂𐶐�����֐�
	CorpseGenerater(objManager);			

	updateFunc_ = &Player::NormalUpdate;
}

// �v���C���[�̎��̂�vector�z��ŊǗ�����֐�
void Player::CorpseGenerater(const std::shared_ptr<ObjectManager>& objManager)
{
	//�z�u�f�[�^�̍쐬
	LoadObjectInfo info = {};

	//���f���̒�����w��̃t���[���̔ԍ����擾����
	int frameNo = MV1SearchFrame(model_->GetModelHandle(), "PlaceToPutTheCorpse");

	//�w��̃t���[���̍��W���擾����
	VECTOR putPos = MV1GetFramePosition(model_->GetModelHandle(), frameNo);

	//�擾�������W�����̂̃|�W�V�����Ƃ���
	info.pos = putPos;

	//�v���C���[�̉�]�l�����̂̉�]�l�Ƃ���
	info.rot = MathUtil::VECTORDegreeToRadian(status_.rot);

	//�g�k���𓯂��ɂ���
	info.scale = scale_;

	//���̂𐶐�����
	objManager->CorpseGenerator(model_->GetModelHandle(),info, status_.animNo);
}

//�ו����^��
void Player::CarryObject()
{
	
	if (!status_.situation.isCanBeCarried) return;

	status_.situation.isInTransit = true;

	corpseModelPointer_->SetIsTransit(true);

	carryUpdateFunc_ = &Player::DropOffObject;

}

//�ו������낷
void Player::DropOffObject()
{
	if (status_.situation.isCanBeCarried) {
		status_.situation.isCanBeCarried = false;
		corpseModelPointer_->SetIsTransit(false);
		int frameNo = MV1SearchFrame(model_->GetModelHandle(), "PlaceToPutTheCorpse");
		VECTOR putPos = MV1GetFramePosition(model_->GetModelHandle(), frameNo);
		corpseModelPointer_->GetModelPointer()->SetPos(putPos);
		corpseModelPointer_.reset();
	}

	status_.situation.isInTransit = false;
	status_.situation.isCanBeCarried = false;

	carryUpdateFunc_ = &Player::CarryObject;

}

//�N�����N���񂷂��߂ɃN�����N���񂷃|�W�V�����ւƈړ�����
void Player::GoCrankRotationPosition(const std::shared_ptr<ObjectManager>& objManager)
{
	//�N�����N�̗����Ăق����|�W�V�������擾����
	VECTOR standPos = crank_->GetStandingPosition();

	//�����Ăق����|�W�V�����ƃv���C���[�̋����̃T�C�Y���擾����
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(status_.pos, standPos);

	//distanceSize�����͈̔͊O��������
	//���̑��x�ŗ����Ăق����|�W�V�����Ɍ�����
	if (distanceSize > 30.0f) {
		VECTOR distance = VNorm(VSub(standPos, status_.pos));
		VECTOR moveVec = VScale(distance, playerInfo_.walkSpeed);
		status_.pos = VAdd(status_.pos, moveVec);
		model_->SetPos(status_.pos);
	}
	//distanceSize�����͈͓̔��ɓ�������
	//�����Ăق����|�W�V�������v���C���[�̃|�W�V�����Ƃ���
	else {
		status_.pos = standPos;
		model_->SetPos(status_.pos);
		angle_ = -90.0f;
		status_.rot = VGet(0, angle_, 0);
		model_->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));
		ChangeAnimNo(PlayerAnimType::Crank, false, 20);
		updateFunc_ = &Player::CrankUpdate;
	}
}

//�N�����N����]������A�b�v�f�[�g
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

//�N�����N�̍X�V
void Player::CrankUpdate(const std::shared_ptr<ObjectManager>& objManager)
{
	//�Z�k��
	auto& input = InputState::GetInstance();
	auto& sound = SoundManager::GetInstance();

	//�ړ��x�N�g����0�ɂ���
	status_.moveVec = VGet(0, 0, 0);

	//�N�����N�̉�]���擾����(�ω����Ă��邩�m�F�p)
	float oldRotZ = crank_->GetRotZ();

	//�N�����N�̉�]���擾����(���ۂɒl��ς���悤)
	float rotZ = crank_->GetRotZ();

	if (input.IsPressed(InputType::Down))
	{
		rotZ = (std::max)(rotZ - 3.0f, crank_->GetMaxRotZ());
		CrankRotationUpdate(rotZ);
	}
	else if (input.IsPressed(InputType::Up))
	{
		rotZ = (std::min)(rotZ + 3.0f, 0.0f);
		CrankRotationUpdate(rotZ);
	}

	if (oldRotZ != rotZ && !sound.CheckSoundFile("crank"))
	{
		sound.Set3DSoundInfo(crank_->GetStandingPosition(), 1500.0f, "crank");
		sound.PlaySE("crank");
	}

	int naturalNumber = static_cast<int>((std::max)(rotZ, -rotZ));
	float animTime = static_cast<float>(naturalNumber % 360) / 3;

	model_->SetAnimationFrame(animTime);

	if (input.IsTriggered(InputType::Activate))
	{
		status_.situation.isGimmickCanBeOperated = false;
		crank_.reset();
		updateFunc_ = &Player::NormalUpdate;
	}
}

//���o�[��|���|�W�V�����֍s��
void Player::GoLeverPullPosition(const std::shared_ptr<ObjectManager>& objManager)
{
	//�N�����N�̗����Ăق����|�W�V�������擾����
	VECTOR standPos = lever_->GetStandingPosition();
	//�����Ăق����|�W�V�����ƃv���C���[�̋����̃T�C�Y���擾����
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(status_.pos, standPos);

	//distanceSize�����͈̔͊O��������
	//���̑��x�ŗ����Ăق����|�W�V�����Ɍ�����
	if (distanceSize > 30.0f)
	{
		VECTOR distance = VNorm(VSub(standPos, status_.pos));
		VECTOR moveVec = VScale(distance, playerInfo_.walkSpeed);
		status_.pos = VAdd(status_.pos, moveVec);
		model_->SetPos(status_.pos);
	}
	//distanceSize�����͈͓̔��ɓ�������
	//�����Ăق����|�W�V�������v���C���[�̃|�W�V�����Ƃ���
	else
	{
		status_.pos = standPos;
		model_->SetPos(status_.pos);
		angle_ = 0.0f;
		lever_->OnAnimation();
		status_.rot = VGet(0, angle_, 0);
		model_->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));
		ChangeAnimNo(PlayerAnimType::LeverOn, false, 10);
		updateFunc_ = &Player::LeverUpdate;
	}
}

//���o�[�̍X�V
void Player::LeverUpdate(const std::shared_ptr<ObjectManager>& objManager)
{
	if (!lever_->IsOn())
	{
		updateFunc_ = &Player::NormalUpdate;
	}
}

//�������Ƃ̏Փ˃A�b�v�f�[�g
void Player::BulletHitMeUpdate(const std::shared_ptr<ObjectManager>& objManager)
{
	//�d��
	status_.jump.jumpVec += gravity;
	status_.moveVec.y = status_.jump.jumpVec;

	//�m�b�N�o�b�N
	status_.moveVec = VScale(status_.moveVec, 0.96f);

	//�ړ��x�N�g���𑫂����s������擾����
	VECTOR destinationPos = VAdd(status_.pos, status_.moveVec);

	//�ړ��x�N�g���𑫂����s���悪-250�ȉ���������
	//�ړ��x�N�g����Z�l��0�ɂ���
	if (destinationPos.z < -250.0f)
	{
		status_.moveVec.z = 0.0f;
	}

	//�v���C���[�̃|�W�V�����Ɉړ��x�N�g���𑫂�
	status_.pos = VAdd(status_.pos, status_.moveVec);

	//���f���Ƀ|�W�V������ݒ肷��
	model_->SetPos(status_.pos);


	//�ȉ��A�s���v�f
	float moveVecSize = VSize(status_.moveVec);

	if (moveVecSize < 2.0f)
	{
		updateFunc_ = &Player::NormalUpdate;
	}
}

//�A�j���[�V������ҋ@�ɕύX����
void Player::ChangeAnimIdle()
{
	//�ҋ@�A�j���[�V�����ɖ߂�
	if (!status_.situation.isMoving)
	{
		ChangeAnimNo(PlayerAnimType::Idle, true, 20);
	}
}

//�A�j���[�V�����̕ύX���s��
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
	if (pressedShift)
	{
#ifdef _DEBUG
		if (debugCreativeMode) {
			return playerInfo_.runningSpeed * 3;
		}
		return playerInfo_.runningSpeed;
#else
		return playerInfo_.runningSpeed;
#endif // _DEBUG
	} 
	
	return playerInfo_.walkSpeed;
}

//�����̍Đ�
void Player::FootStepSound()
{
	//�������Ă��邩
	bool playSound = false;

	//�v���C���[�̃A�j���[�V�����ɂ����
	//�A�j���[�V�����t���[���̓���t���[����
	//�������Ă��邩�̃t���O��true�ɂ���
	switch (static_cast<PlayerAnimType>(status_.animNo))
	{
	case PlayerAnimType::Walk:

		if (model_->GetSpecifiedAnimTime(80) || model_->GetSpecifiedAnimTime(160)) {
			playSound = true;
		}

		break;
	case PlayerAnimType::Run:

		if (model_->GetSpecifiedAnimTime(6) || model_->GetSpecifiedAnimTime(45)) {
			playSound = true;
		}

		break;
	}

	//�������Ă��Ȃ��ꍇ���^�[��
	if (!playSound)
	{
		return;
	}

	//�v���C���[�̑����̃I�u�W�F�N�g�̑f�ނɂ����
	//�炷����ς���
	switch (materialSteppedOn_)
	{
	case Material::Iron:
		SoundManager::GetInstance().Set3DSoundInfo(status_.pos, 1500.0f, "ironStep");
		SoundManager::GetInstance().PlaySE("ironStep");
		break;
	case Material::Stone:
		SoundManager::GetInstance().Set3DSoundInfo(status_.pos, 1500.0f, "asphaltStep");
		SoundManager::GetInstance().PlaySE("asphaltStep");
		break;
	}
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
	for (int i = 1; i < 7; i++)
	{
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
