#include "EnemyBase.h"
#include "Player.h"
#include "../util/Model.h"
#include "../util/ObjectManager.h"
#include "../util/Aster.h"


namespace {
	//�ǐՂ��~�߂�͈�
	constexpr float range_to_stop_tracking = 60.0f;

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
//	SearchForPlayer(playerPos);

	//���f���̍X�V
	model_->Update();

	//AvoidAndTrackObjectsUpdate(playerPos);

	//�v���C���[��ǐՂ���
//	TrackingUpdate(playerPos);

	VECTOR pos = Aster_->LocationInformation(playerPos,pos_);
	tempa = pos;
	tempdistance = VSub(pos_,pos);
	VECTOR moveVec = VScale(VNorm(tempdistance), move_speed);
//	pos_ = VAdd(pos_,moveVec);

	model_->SetPos(pos_);

	if (distance_ < range_to_stop_tracking + 20.0f) {
		//ThrustAway(player);
	}

}

void EnemyBase::Draw()
{
	model_->Draw();

	Aster_->Draw();

	DrawSphere3D(pos_, 8, 32, 0xff0000, 0xff0000, true);
	DrawSphere3D(tempdistance, 32, 32, 0xff0000, 0xff0000, true);

	DrawFormatString(0, 64, 0x448844, "%.2f,%.2f,%.2f", tempdistance.x, tempdistance.y, tempdistance.z);
	DrawFormatString(0, 80, 0x448844, "%.2f,%.2f,%.2f", tempa.x, tempa.y, tempa.z);

	/*VECTOR stomachPosition = VAdd(pos_,frontVec_);
	stomachPosition.y = stomachPosition.y + model_height / 2;

	VECTOR ray = VScale(frontVec_, ray_radius);
	ray = VAdd(pos_, ray);
	ray.y = ray.y += model_height / 2;
	DrawLine3D(stomachPosition,ray,0xff0000);
	if (isHit_) {
		DrawString(0, 64, "hit,hit,hit", 0x448844);
	}

	if (temp.HitNum > 0) {
		DrawTriangle3D(temp.Dim->Position[0], temp.Dim->Position[1], temp.Dim->Position[2], 0xff0000, true);
	}*/
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

void EnemyBase::SearchForPlayer(VECTOR playerPos)
{
	//�G����v���C���[�̒�������
	distance_ = VSize(VSub(playerPos, pos_));

	//���ς��擾����(�Ԃ�l�̓R�T�C��)
	innerProduct = VDot(frontVec_, VNorm(VSub(playerPos, pos_)));

	//��L�̌��ʂ���x���@�ɕς���
	float aiu = acos(innerProduct);
	innerProduct = aiu / DX_PI_F * 180.0f;
}

//�v���C���[��˂���΂�
void EnemyBase::ThrustAway(Player& player)
{
	VECTOR push = VScale(VNorm(frontVec_),110);

	VECTOR nockback = VAdd(player.GetStatus().pos, push);

	player.SetPos(nockback);
}

//void EnemyBase::AvoidAndTrackObjectsUpdate(VECTOR playerPos)
//{
//	VECTOR distancePlayerAndEnemy = VSub(playerPos, pos_);
//	VECTOR moveVec = VScale(VNorm(distancePlayerAndEnemy), move_speed);
//
//	VECTOR destination = VAdd(frontVec_, moveVec);
//
//	for (auto& model : ObjectManager::GetInstance().GetAllCheckCollModel()) {
//		//�Փ˔���̍X�V
//		MV1RefreshCollInfo(model->GetModelHandle(), model->GetColFrameIndex());
//
//		//�G���W����̌��o�͈�
//		auto rangeOfDetection = check_collition_radius + VSize(moveVec);
//		//�Փ˔���̌���
//		auto result = MV1CollCheck_Sphere(model->GetModelHandle(), model->GetColFrameIndex(), pos_, rangeOfDetection);
//		//list�ɒǉ�
//		hitDim_.push_back(result);
//	}
//
//	//moveVec�̐�Βl�œ������������ĂȂ����𔻒f
//	if (fabs(moveVec.x) > 0.000001f || fabs(moveVec.z) > 0.000001f) {
//		isMove_ = true;
//	}
//	else {
//		isMove_ = false;
//	}
//
//	CheckHeadOnCollision();
//
//	for (auto& hit : hitDim_) {
//		MV1CollResultPolyDimTerminate(hit);
//	}
//
//	temp = hitDim_.front();
//
//	hitDim_.clear();
//
//}
//
//void EnemyBase::CheckHeadOnCollision()
//{
//	//��̂̕��̍����𓾂�
//	VECTOR stomachPosition = VAdd(pos_, frontVec_);
//	stomachPosition.y = stomachPosition.y + model_height / 2;
//
//	VECTOR ray = VScale(frontVec_, ray_radius);
//	ray = VAdd(pos_, ray);
//	ray.y = ray.y += model_height / 2;
//
//	for (auto& dim : hitDim_) {
//		//dim.HitNum
//		auto result = HitCheck_Line_Triangle(stomachPosition, ray, dim.Dim->Position[0], dim.Dim->Position[1], dim.Dim->Position[2]);
//		if (result.HitFlag) {
//			isHit_ = result.HitFlag;
//			break;
//		}
//		isHit_ = result.HitFlag;
//	}
//	
//
//}

