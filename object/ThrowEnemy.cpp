#include "ThrowEnemy.h"

#include "../object/Player.h"
#include "../object/ShotManager.h"

#include "../util/Util.h"
#include "../util/Model.h"

namespace {
	//�E��̃t���[����
	const char* const hand_framename = "hand.R_end";

	//���𓊂��Ă���Ƃ��̃A�j���[�V�����t���[��
	constexpr int throw_frame_time = 73;
}

 ThrowEnemy::ThrowEnemy(int handle, Material materialType, LoadObjectInfo objInfo):EnemyBase(handle, materialType, objInfo)
{
	 //�Փ˔���̐ݒ�
	 isCollCheck_ = true;
	 model_->SetUseCollision(isCollCheck_, true);

	 //������̎擾
	 std::string str = StrUtil::GetStringAfterSign(objInfo.name, "-");

	 //������Fake�������瓊����ӂ������t���O�𗧂Ă�
	 if (str == "Fake")
	 {
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

	//������A�j���[�V�������I�������
	//�����Ă���Ƃ����t���O��false�ɂ���
	if (model_->IsAnimEnd() && isThrow_)
	{
		model_->ChangeAnimation(static_cast<int>(PlayerAnimType::Idle), true, false, 20);
		isThrow_ = false;
	}

	//�v���C���[�����m������
	//�v���C���[�̕���������
	if (isDetection_)
	{
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
//	else 
//	{
//		//��]�s��Ɗg�k�s��̍���
//		MATRIX mtx = CombiningRotAndScallMat(initFrontVec_);
//
//		//��]�s��Ɗg�k�s����|�����s��ɕ��s�ړ��s���������
//		mtx = MMult(mtx, MGetTranslate(pos_));
//
//		//�s������f���ɃZ�b�g����
//		MV1SetMatrix(model_->GetModelHandle(), mtx);
//	}
}

void ThrowEnemy::Draw()
{
	//���f���̕`��
	model_->Draw();

	//�����e�̕`��
	DrawPolygon3D();
}

void ThrowEnemy::Shot(std::shared_ptr<ShotManager> shotManager, VECTOR playerPos, float height)
{
	//�v���C���[�����m���Ă��邩�ǂ���
	//���m���Ă��Ȃ�������return
	if (!isDetection_)
	{
		return;
	}

	//�A�j���[�V�����𓊂���A�j���[�V�����ɕύX����
	model_->ChangeAnimation(static_cast<int>(PlayerAnimType::Throw), false, false, 5);

	//������ӂ������G�������ꍇ���^�[������
	if (isFakeThrow_)
	{
		return;
	}

	//�����Ă���A�j���[�V�����̓���t���[����
	//�e�𔭎˂���
	if (model_->GetSpecifiedAnimTime(throw_frame_time))
	{
		VECTOR framePos = model_->GetFrameLocalPosition(hand_framename);
		shotManager->Fire(framePos, playerPos, height);
		isThrow_ = true;
	}
}