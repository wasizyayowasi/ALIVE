#include "EnemyBase.h"
#include "Player.h"
#include "../util/Model.h"
#include "../util/ObjectManager.h"
#include "../util/Aster.h"


namespace {
	//�ǐՂ��~�߂�͈�
	constexpr float range_to_stop_tracking = 500.0f;

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
	//ray�̒���
	constexpr float ray_radius = 200.0f;

	//�G���f���̍���
	constexpr float model_height = 150.0f;
}


EnemyBase::EnemyBase(const char* fileName, LoadObjectInfo objInfo):CharacterBase(fileName,objInfo)
{
}

EnemyBase::EnemyBase(int handle, LoadObjectInfo objInfo) : CharacterBase(handle,objInfo)
{
	model_->SetAnimation(0, true, false);
	Aster_ = std::make_shared<Aster>();
}

void EnemyBase::Update(Player& player)
{
	//�v���C���[�̍��W
	VECTOR playerPos = player.GetStatus().pos;

	//���G
	if (SearchForPlayer(playerPos)) {
		if (!IsThereAnObject(playerPos)) {
			//�v���C���[��ǐՂ���
			TrackingUpdate(playerPos);
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

	//���f���̍X�V
	model_->Update();

	if (distance_ < range_to_stop_tracking + 20.0f) {
		//ThrustAway(player);
	}

}

void EnemyBase::Draw()
{
	model_->Draw();

	Aster_->Draw();
}

void EnemyBase::TrackingUpdate(VECTOR playerPos)
{
	model_->ChangeAnimation(7, true, false, 20);

	//�v���C���[�Ǝ����̍����Z�o���A���K�����A�X�s�[�h���|����
	VECTOR distancePlayerAndEnemy = VSub(playerPos, pos_);
	VECTOR moveVec = VScale(VNorm(distancePlayerAndEnemy), move_speed);

	//��]�s��̎擾
	MATRIX rotMtx = MGetRotVec2(VGet(0, 0, -1), VSub(playerPos, pos_));

	//�g�k�s��̎擾
	MATRIX scaleMtx = MGetScale(scale_);

	//���ʃx�N�g�����擾����
	frontVec_ = VTransformSR(VGet(0, 0, -1), rotMtx);

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
	distance_ = VSize(VSub(playerPos, pos_));

	//���ς��擾����(�Ԃ�l�̓R�T�C��)
	innerProduct = VDot(frontVec_, VNorm(VSub(playerPos, pos_)));

	//��L�̌��ʂ���x���@�ɕς���
	float aiu = acos(innerProduct);
	innerProduct = aiu / DX_PI_F * 180.0f;

	if (DistanceIsWithinRange()) {
		return false;
	}

	//�G�̎���p�����O���ɂ���܂���
	//�G����v���C���[�̋������w��͈͂��傫��������return
	if (innerProduct > viewing_angle || distance_ > visible_range) {
		model_->ChangeAnimation(0, true, false, 20);
		return false;
	}

	return true;
}

//�v���C���[��˂���΂�
void EnemyBase::ThrustAway(Player& player)
{
	VECTOR push = VScale(VNorm(frontVec_),110);

	VECTOR nockback = VAdd(player.GetStatus().pos, push);

	player.SetPos(nockback);
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
	MATRIX rotMtx = MGetRotVec2(VGet(0, 0, -1), VSub(targetPos, pos_));

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
		debug_[i] = PointPos;
		if (noObject) {
			break;
		}
	}

	return noObject;
}

bool EnemyBase::DistanceIsWithinRange()
{
	//�v���C���[�ƓG�̍��W�������āA
	if (distance_ < range_to_stop_tracking) {
		model_->ChangeAnimation(0, true, false, 20);
		return true;
	}

	return false;
}

