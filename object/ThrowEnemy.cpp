#include "ThrowEnemy.h"

#include "../object/Player.h"
#include "../object/ShotManager.h"

#include "../util/Util.h"
#include "../util/Model.h"

namespace {
	//���f���̏�����]�x�N�g��
	const VECTOR init_rot = { 0.0f,0.0f,-1.0f };

	//�G���v���C���[�����F�ł���͈�
	constexpr float visible_range = 800.0f;

	//�E��̃t���[����
	const char* const hand_framename = "mixamorig:RightHandIndex2";

	//���𓊂��Ă���Ƃ��̃A�j���[�V�����t���[��
	constexpr int throw_frame_time = 73;

	//�G�̎���p
	constexpr float viewing_angle = 30.0f;

	//�΂𓊂��鋗��
	constexpr float throw_distance = 800.0f;
}

ThrowEnemy::ThrowEnemy(int handle, LoadObjectInfo objInfo):EnemyBase(handle,objInfo)
{
	frontVec_ = init_rot;
}

ThrowEnemy::~ThrowEnemy()
{
}

void ThrowEnemy::Update(Player& player)
{
	//�v���C���[�̍��W
	VECTOR playerPos = player.GetStatus().pos;

	//�v���C���[�����G����
	SearchForPlayer(playerPos);

	if (isDetection_) {
		//�v���C���[�ƓG�̋���
		VECTOR distance = VNorm(VSub(playerPos, pos_));

		//Y���͍l�����Ȃ�
		distance.y = 0.0f;

		//��]�s��Ɗg�k�s��̍���
		MATRIX mtx = CombiningRotAndScallMat(distance);

		//��]�s��Ɗg�k�s����|�����s��ɕ��s�ړ��s���������
		mtx = MMult(mtx, MGetTranslate(pos_));

		//�s������f���ɃZ�b�g����
		MV1SetMatrix(model_->GetModelHandle(), mtx);
	}

	//���f���̍X�V
	model_->Update();
}

void ThrowEnemy::Draw()
{
	model_->Draw();

	DrawLine3D(pos_, VAdd(pos_, VScale(frontVec_, 10.0f)), 0x0000ff);
}

void ThrowEnemy::SearchForPlayer(VECTOR playerPos)
{
	//�G����v���C���[�̒�������
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(playerPos, pos_);

	//���ς��擾����(�Ԃ�l�̓R�T�C��)
	innerProduct = VDot(frontVec_, VNorm(VSub(playerPos, pos_)));

	//��L�̌��ʂ���x���@�ɕς���
	float radian = acos(innerProduct);
	innerProduct = radian / DX_PI_F * 180.0f;

	//����͈͓̔����������΂𓊂��鋗������
	//�Z��������v���C���[�����m�������Ƃɂ���
	if (innerProduct < viewing_angle) {
		if (distanceSize < throw_distance) {
			isDetection_ = true;
		}
	}
}

void ThrowEnemy::Shot(std::shared_ptr<ShotManager> shotManager, VECTOR playerPos, float height)
{
	//������A�j���[�V�������I�������
	//�����Ă���Ƃ����ϐ���false�ɂ���
	if (model_->IsAnimEnd()) {
		model_->ChangeAnimation(static_cast<int>(EnemyAnimType::Idle), false, false, 5);
		isThrow_ = false;
	}

	//�v���C���[�����m���Ă��邩�ǂ���
	//���m���Ă��Ȃ�������return
	if (!isDetection_) {
		return;
	}

	//�v���C���[�̃|�W�V�����ƃG�l�~�[�̋����̃T�C�Y���擾
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(playerPos, pos_);

	//�����Ă���r���ł͂Ȃ�������
	//�A�j���[�V�����𓊂���A�j���[�V�����ɕύX����
	if (!isThrow_) {
		model_->ChangeAnimation(static_cast<int>(EnemyAnimType::Throw), false, false, 5);
		isThrow_ = true;
	}

	//�����Ă���A�j���[�V�����̓���t���[����
	//�e�𔭎˂���
	if (model_->GetSpecifiedAnimTime(throw_frame_time)) {
		VECTOR framePos = model_->GetFrameLocalPosition(hand_framename);
		shotManager->Fire(framePos, playerPos, height);
	}
}