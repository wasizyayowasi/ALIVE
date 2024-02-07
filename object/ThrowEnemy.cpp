#include "ThrowEnemy.h"

#include "../object/Player.h"
#include "../object/ShotManager.h"

#include "../util/Util.h"
#include "../util/Model.h"

namespace {
	//�G���v���C���[�����F�ł���͈�
	constexpr float visible_range = 1300.0f;

	//�E��̃t���[����
	//const char* const hand_framename = "mixamorig:RightHandIndex2";
	const char* const hand_framename = "hand.R_end";

	//���𓊂��Ă���Ƃ��̃A�j���[�V�����t���[��
	constexpr int throw_frame_time = 73;

	//�G�̎���p
	constexpr float viewing_angle = 45.0f;
}

 ThrowEnemy::ThrowEnemy(int handle, Material materialType, LoadObjectInfo objInfo):EnemyBase(handle, materialType, objInfo)
{
	 //�Փ˔���̐ݒ�
	 isCollCheck_ = true;
	 model_->SetUseCollision(isCollCheck_, true);

	 //������̎擾
	 std::string str = StrUtil::GetStringAfterSign(objInfo.name, "-");

	 //������Fake�������瓊����ӂ������t���O�𗧂Ă�
	 if (str == "Fake") {
		 isFakeThrow_ = true;
	 }

	 //�}�e���A���̐F��ς���
	 MV1SetMaterialDifColor(model_->GetModelHandle(), 8, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
}

ThrowEnemy::~ThrowEnemy()
{
}

void ThrowEnemy::Update(Player& player)
{
	//�v���C���[�̍��W
	VECTOR playerPos = player.GetStatus().pos;

	//���f���̍X�V
	model_->Update();

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
}

void ThrowEnemy::Draw()
{
	model_->Draw();

	//DrawLine3D(pos_, VScale(frontVec_, 800.0f), 0x0000ff);
}

void ThrowEnemy::SearchForPlayer(VECTOR playerPos)
{
	//�G����v���C���[�̒�������
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(playerPos, pos_);

	float innerProduct = 0.0f;

	//���ς��擾����(�Ԃ�l�̓R�T�C��)
	innerProduct = VDot(VNorm(frontVec_), VNorm(VSub(playerPos, pos_)));

	//��L�̌��ʂ���x���@�ɕς���
	float radian = acos(innerProduct);
	innerProduct = radian / DX_PI_F * 180.0f;

	//����͈͓̔����������΂𓊂��鋗������
	//�Z��������v���C���[�����m�������Ƃɂ���
	if (innerProduct < viewing_angle) {
		if (distanceSize < visible_range) {
			isDetection_ = true;
		}
		else {
			isDetection_ = false;
		}
	}
	else {
		isDetection_ = false;
	}
}

void ThrowEnemy::Shot(std::shared_ptr<ShotManager> shotManager, VECTOR playerPos, float height)
{
	//������A�j���[�V�������I�������
	//�����Ă���Ƃ����ϐ���false�ɂ���
	if (model_->IsAnimEnd()) {
		model_->ChangeAnimation(static_cast<int>(PlayerAnimType::Idle), false, false, 5);
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
		//model_->ChangeAnimation(static_cast<int>(EnemyAnimType::Throw), false, false, 5);
		model_->ChangeAnimation(static_cast<int>(PlayerAnimType::Throw), false, false, 5);
		isThrow_ = true;
	}

	if (isFakeThrow_) {
		return;
	}

	//�����Ă���A�j���[�V�����̓���t���[����
	//�e�𔭎˂���
	if (model_->GetSpecifiedAnimTime(throw_frame_time)) {
		VECTOR framePos = model_->GetFrameLocalPosition(hand_framename);
		shotManager->Fire(framePos, playerPos, height);
	}
}