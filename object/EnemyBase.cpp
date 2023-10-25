#include "EnemyBase.h"
#include "Player.h"
#include "../util/Model.h"


namespace {
	//�ǐՂ��~�߂�͈�
	constexpr float range_to_stop_tracking = 60.0f;

	//���f���̏�����]�x�N�g��
	const VECTOR init_rot = { 0.0f,0.0f,-1.0f };

	//�G�̎���p
	constexpr float viewing_angle = 30.0f;
	constexpr float visible_range = 200.0f;
}


EnemyBase::EnemyBase(const char* fileName, LoadObjectInfo objInfo):CharacterBase(fileName,objInfo)
{
}

EnemyBase::EnemyBase(int handle, LoadObjectInfo objInfo) : CharacterBase(handle,objInfo)
{
	model_->SetAnimation(0, true, false);
}

void EnemyBase::Update(Player& player)
{
	//�v���C���[�̍��W
	VECTOR playerPos = player.GetStatus().pos;

	//���G
	SearchForPlayer(playerPos);

	//���f���̍X�V
	model_->Update();

	//�v���C���[��ǐՂ���
	TrackingUpdate(playerPos);

	if (distance_ < range_to_stop_tracking + 20.0f) {
		ThrustAway(player);
	}

}

void EnemyBase::TrackingUpdate(VECTOR playerPos)
{

	//�v���C���[�ƓG�̍��W�������āA
	if (distance_ < range_to_stop_tracking) {
		model_->ChangeAnimation(0, true, false, 20);
		return;
	}
	//�G�̎���p�����O���ɂ���܂���
	//�G����v���C���[�̋������w��͈͂��傫��������return
	if (innerProduct > viewing_angle || distance_ > visible_range) {
		model_->ChangeAnimation(0, true, false, 20);
		return;
	}

	model_->ChangeAnimation(7, true, false, 20);

	//�v���C���[�Ǝ����̍����Z�o���A���K�����A�X�s�[�h���|����
	VECTOR moveVec = VScale(VNorm(VSub(playerPos, pos_)), 3.0f);

	//��]�s��̎擾
	MATRIX rotMtx = MGetRotVec2(VGet(0, 0, -1), VSub(playerPos, pos_));

	//�g�k�s��̎擾
	MATRIX scaleMtx = MGetScale(scale_);

	//���ʃx�N�g�����擾����
	frontVector_ = VTransformSR(VGet(0, 0, -1), rotMtx);

	//��]�s��Ɗg�k�s��̊|���Z
	MATRIX mtx = MMult(rotMtx, scaleMtx);

	//�|�W�V�����̈ړ�
	pos_ = VAdd(pos_, moveVec);

	//��]�s��Ɗg�k�s����|�����s��ɕ��s�ړ��s���������
	mtx = MMult(mtx, MGetTranslate(pos_));

	//�s������f���ɃZ�b�g����
	MV1SetMatrix(model_->GetModelHandle(), mtx);
}

void EnemyBase::SearchForPlayer(VECTOR playerPos)
{
	//�G����v���C���[�̒�������
	distance_ = VSize(VSub(playerPos, pos_));

	//���ς��擾����(�Ԃ�l�̓R�T�C��)
	innerProduct = VDot(frontVector_, VNorm(VSub(playerPos, pos_)));

	//��L�̌��ʂ���x���@�ɕς���
	float aiu = acos(innerProduct);
	innerProduct = aiu / DX_PI_F * 180.0f;
}

//�v���C���[��˂���΂�
void EnemyBase::ThrustAway(Player& player)
{
	VECTOR push = VScale(VNorm(frontVector_),110);

	VECTOR nockback = VAdd(player.GetStatus().pos, push);

	player.SetPos(nockback);
}

