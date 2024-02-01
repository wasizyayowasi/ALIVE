#include "CorpseRecoveryEnemy.h"

#include "../util/Model.h"
#include "../util/util.h"

namespace {
	//���f���̏�����]�x�N�g��
	const VECTOR init_rot = { 0.0f,0.0f,-1.0f };

	//�G���v���C���[�����F�ł���͈�
	constexpr float visible_range = 500.0f;

	//�G�̎���p
	constexpr float viewing_angle = 30.0f;

	//�G�̃X�s�[�h
	constexpr float move_speed = 3.0f;

	//�E��̃t���[����
	const char* const hand_framename = "mixamorig:RightHandIndex2";
}

CorpseRecoveryEnemy::CorpseRecoveryEnemy(int handle,Material materialType, LoadObjectInfo objInfo):EnemyBase(handle, materialType, objInfo)
{
	initPos_ = objInfo.pos;
}

CorpseRecoveryEnemy::~CorpseRecoveryEnemy()
{
}

void CorpseRecoveryEnemy::Update(Player& player)
{

}

void CorpseRecoveryEnemy::Draw()
{
	model_->Draw();
}

void CorpseRecoveryEnemy::SearchForCorpse(VECTOR CorpsePos)
{
	//�G���玀�̂̒�������
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(CorpsePos, pos_);

	//���ς��擾����(�Ԃ�l�̓R�T�C��)
	innerProduct = VDot(frontVec_, VNorm(VSub(CorpsePos, pos_)));

	//��L�̌��ʂ���x���@�ɕς���
	float radian = acos(innerProduct);
	innerProduct = radian / DX_PI_F * 180.0f;

	//����͈͓̔��������鋗���������猟�m�������Ƃɂ���
	if (innerProduct < viewing_angle) {
		if (distanceSize < visible_range) {
			isDetection_ = true;
		}
	}
}

void CorpseRecoveryEnemy::UpdateForCorpse(std::shared_ptr<ObjectBase> pointer)
{
	//�G�l�~�[�̃|�W�V����
	VECTOR corpsePos = pointer->GetPos();

	SearchForCorpse(corpsePos);

	if (isDetection_) {
		VECTOR distance = VSub(corpsePos, pos_);
		VECTOR norm = VNorm(distance);
		VECTOR moveVec = VScale(moveVec, move_speed);

		pos_ = VAdd(pos_, moveVec);

		model_->SetPos(pos_);

		float distanceSize = VSize(distance);
		if (distanceSize < 3.0f) {
			VECTOR framePos = model_->GetFrameLocalPosition(hand_framename);
			pointer->SetPos(framePos);
			isTransit_ = true;
		}
	}

	if (isTransit_) {
		float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(initPos_, pos_);
		if (distanceSize < 3.0f) {
			pointer->SetPos(initPos_);
		}
	}
}
