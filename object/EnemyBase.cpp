#include "EnemyBase.h"
#include "Player.h"
#include "ShotManager.h"
#include "../util/Model.h"
#include "../util/ObjectManager.h"
#include "../util/Aster.h"

#include "../util/Util.h"

namespace {
	//���f���̏�����]�x�N�g��
	const VECTOR init_rot = { 0.0f,0.0f,-1.0f };

	//�G�̎���p
	constexpr float viewing_angle = 30.0f;
	//�G���v���C���[�����F�ł���͈�
	constexpr float visible_range = 500.0f;
	//�G�̃X�s�[�h
	constexpr float move_speed = 3.0f;

	//�I�u�W�F�N�g�F�m�͈�
	constexpr float check_collition_radius = 200.0f;

	//�G���f���̍���
	constexpr float model_height = 150.0f;

	//���[�`
	constexpr float within_reach = 80.0f;

	//�E��̃t���[����
	const char* const hand_framename = "mixamorig:RightHandIndex2";

	//���𓊂��Ă���Ƃ��̃A�j���[�V�����t���[��
	constexpr int throw_frame_time = 73;
}

EnemyBase::EnemyBase(int handle, LoadObjectInfo objInfo) : CharacterBase(handle,objInfo)
{
	model_->SetAnimation(0, true, false);
	Aster_ = std::make_shared<Aster>(objInfo.pos);
	frontVec_ = init_rot;
}

void EnemyBase::Update(Player& player, const InputState& input)
{

	//���f���̍X�V
	model_->Update();

	if (isThrow_) {
		return;
	}

	isDetection_ = false;

	//�v���C���[�̍��W
	VECTOR playerPos = player.GetStatus().pos;

	//���G
	if (!IsThereAnObject(playerPos)) {
		if (SearchForPlayer(playerPos)) {
			//�v���C���[��ǐՂ���
			TrackingUpdate(playerPos);
		}
		else {
			model_->ChangeAnimation(static_cast<int>(EnemyAnimType::Idle), true, true, 10);
		}
	}
	else{
		if (DistanceIsWithinRange()) {
			//�ڕW�}�X�̒��S���W���擾
			Aster_->LocationInformation(playerPos, pos_);

			//�o�H�T��
			RoutingUpdate(player);
		}
	}

	if (distanceSize_ < within_reach) {
		pushVec_ = VScale(VNorm(frontVec_), 10);
	}

	float pushVecSize = VSize(pushVec_);

	if (pushVecSize > 1.0f) {
		ThrustAway(player);
	}

}

void EnemyBase::Draw()
{
	model_->Draw();

//	Aster_->Draw();
}

void EnemyBase::TrackingUpdate(VECTOR playerPos)
{

	//�v���C���[�Ǝ����̍����Z�o���A���K�����A�X�s�[�h���|����
	VECTOR distancePlayerAndEnemy = VSub(playerPos, pos_);
	distancePlayerAndEnemy.y = 0;
	VECTOR moveVec = VScale(VNorm(distancePlayerAndEnemy), move_speed);

	//��]�s��̎擾
	MATRIX rotMtx = MGetRotVec2(init_rot, distancePlayerAndEnemy);

	//�g�k�s��̎擾
	MATRIX scaleMtx = MGetScale(scale_);

	//���ʃx�N�g�����擾����
	frontVec_ = VTransformSR(init_rot, rotMtx);

	//��]�s��Ɗg�k�s��̊|���Z
	MATRIX mtx = MMult(rotMtx, scaleMtx);

	//�|�W�V�����̈ړ�
	pos_ = VAdd(pos_, moveVec);

	//��]�s��Ɗg�k�s����|�����s��ɕ��s�ړ��s���������
	mtx = MMult(mtx, MGetTranslate(pos_));

	//�s������f���ɃZ�b�g����
	MV1SetMatrix(model_->GetModelHandle(), mtx);
}

bool EnemyBase::SearchForPlayer(VECTOR playerPos)
{
	//�G����v���C���[�̒�������
	distanceSize_ = MathUtil::GetSizeOfDistanceTwoPoints(playerPos,pos_);

	//���ς��擾����(�Ԃ�l�̓R�T�C��)
	innerProduct = VDot(frontVec_, VNorm(VSub(playerPos, pos_)));

	//��L�̌��ʂ���x���@�ɕς���
	float radian = acos(innerProduct);
	innerProduct = radian / DX_PI_F * 180.0f;

	if (innerProduct < viewing_angle) {
		if (distanceSize_ < 700.0f) {
			isDetection_ = true;
		}
	}

	//�G�̎���p�����O���ɂ���܂���
	//�G����v���C���[�̋������w��͈͂��傫��������return
	if (innerProduct < viewing_angle) {
		if (distanceSize_ > visible_range) {
			model_->ChangeAnimation(static_cast<int>(EnemyAnimType::Idle), true, false, 20);
			return false;
		}
	}

	if (!DistanceIsWithinRange()) {
		return false;
	}

	return true;
}

//�v���C���[��˂���΂�
void EnemyBase::ThrustAway(Player& player)
{
	pushVec_ = VScale(pushVec_, 0.96f);
	float size = VSize(pushVec_);
	VECTOR nockback = VAdd(player.GetStatus().pos,pushVec_);

	if (nockback.z < -250.0f) {
		pushVec_.z = 0.0f;
	}

	player.SetMoveVec(VAdd(player.GetStatus().moveVec, pushVec_));
}

void EnemyBase::RoutingUpdate(Player& player)
{
	//�v���C���[�̍��W
	VECTOR playerPos = player.GetStatus().pos;

	VECTOR targetPos = Aster_->GetDestinationCoordinates(playerPos,pos_);

	//�ڕW�n�_�ƃ|�W�V�����̋������擾
	VECTOR distance = VSub(targetPos, pos_);

	float size = VSize(distance);


	//��]�s��̎擾
	MATRIX rotMtx = MGetRotVec2(init_rot, VSub(targetPos, pos_));

	//���ʃx�N�g�����擾����
	frontVec_ = VTransformSR(init_rot, rotMtx);

	//�g�k�s��̎擾
	MATRIX scaleMtx = MGetScale(scale_);

	//��]�s��Ɗg�k�s��̊|���Z
	MATRIX mtx = MMult(rotMtx, scaleMtx);
	//��]�s��Ɗg�k�s����|�����s��ɕ��s�ړ��s���������
	mtx = MMult(mtx, MGetTranslate(pos_));
	//�s������f���ɃZ�b�g����
	MV1SetMatrix(model_->GetModelHandle(), mtx);

	if (size > 5.0f) {
		//���K��
		VECTOR norm = VNorm(distance);
		//�ړ��x�N�g�����쐬
		VECTOR moveVec = VScale(norm, move_speed);
		//�ړ�
		pos_ = VAdd(pos_, moveVec);
		//�|�W�V�����̍X�V
		model_->SetPos(pos_);
	}
}

bool EnemyBase::IsThereAnObject(VECTOR playerPos)
{
	VECTOR distance = VSub(playerPos, pos_);
	int size = VSize(distance) / 50;
	VECTOR norm = VNorm(distance);
	
	bool noObject = false;

	for (int i = 0; i < size; i++) {
		VECTOR PointPos = VScale(norm, 50.0f * i);
		PointPos = VAdd(pos_, PointPos);
		noObject = Aster_->temp(PointPos);
		if (noObject) {
			break;
		}
	}

	return noObject;
}

bool EnemyBase::DistanceIsWithinRange()
{
	//�v���C���[�ƓG�̍��W�������āA
	if (distanceSize_ < visible_range) {
		model_->ChangeAnimation(static_cast<int>(EnemyAnimType::Walk), true, false, 20);
		return true;
	}

	return false;
}

void EnemyBase::Shot(std::shared_ptr<ShotManager>shotManager, VECTOR playerPos,float height)
{
	//�v���C���[�����m���Ă��邩�ǂ���
	//���m���Ă��Ȃ�������return
	if (!isDetection_) return;

	//�v���C���[�̃|�W�V�����ƃG�l�~�[�̋����̃T�C�Y���擾
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(playerPos, pos_);

	//�����̃T�C�Y���v���C���[�����F�ł��鋗�������傫��������
	//�A�j���[�V�����𓊂���A�j���[�V�����ɕύX����
	if (distanceSize > visible_range) {
		if (!isThrow_) {
			model_->ChangeAnimation(static_cast<int>(EnemyAnimType::Throw), false, false, 5);
			isThrow_ = true;
		}
	}

	//������A�j���[�V�������I�������
	//�����Ă���Ƃ����ϐ���false�ɂ���
	if (model_->IsAnimEnd()) {
		isThrow_ = false;
	}

	//�����Ă���A�j���[�V�����̓���t���[����
	//�e�𔭎˂���
	if (model_->GetSpecifiedAnimTime(throw_frame_time)) {
		VECTOR framePos = model_->GetFrameLocalPosition(hand_framename);
		shotManager->Fire(framePos, playerPos, height);
	}

}

