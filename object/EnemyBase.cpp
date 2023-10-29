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
	SearchForPlayer(playerPos);

	//���f���̍X�V
	model_->Update();

	if (aiueo(playerPos)) {
		//�ڕW�}�X�̒��S���W���擾
		Aster_->LocationInformation(playerPos, pos_);

		//�o�H�T��
		RoutingUpdate(player);
	}
	else {
		//�v���C���[��ǐՂ���
		TrackingUpdate(playerPos);
	}


	if (distance_ < range_to_stop_tracking + 20.0f) {
		//ThrustAway(player);
	}

}

void EnemyBase::Draw()
{
	model_->Draw();

	Aster_->Draw();

	for (auto& point : debug_) {
		//DrawSphere3D(point.second, 16, 32, 0x0000ff, 0x0000ff, true);
	}

	DrawSphere3D(pos_, 16, 32, 0x0000ff, 0x0000ff, true);

	/*DrawLine3D(VGet(playerpos_.x, playerpos_.y + 20, playerpos_.z), VGet(pos_.x, pos_.y + 20, pos_.z), 0xffff00);



	VECTOR aiu = VSub(pos_, playerpos_);
	float ty = VSize(aiu);

	int asd = ty / 100;

	VECTOR temp = VSub(playerpos_, pos_);

	for (int i = 0; i < asd; i++) {
		aiu = VSub(playerpos_, pos_);
		aiu = VScale(VNorm(aiu), 100.0f + i * 100.0f);
		aiu = VAdd(pos_, aiu);

		DrawSphere3D(aiu, 16, 32, 0x0000ff, 0x0000ff, true);

		int x = temp.x / (100 * (asd - i));
		int z = temp.z / (100 * (asd - i));

		VECTOR pos = ConvWorldPosToScreenPos(aiu);
		DrawFormatString(pos.x, pos.y + 10, 0xff0000, "x:%d,z:%d", x,z);
	}*/
	

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
		Aster_->Init();
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

void EnemyBase::RoutingUpdate(Player& player)
{
	//�v���C���[�̍��W
	VECTOR playerPos = player.GetStatus().pos;

	VECTOR targetPos = Aster_->GetDestinationCoordinates(playerPos,pos_);

	//�ڕW�n�_�ƃ|�W�V�����̋������擾
	VECTOR distance = VSub(targetPos, pos_);

	float size = VSize(distance);

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

bool EnemyBase::aiueo(VECTOR playerPos)
{
	VECTOR distance = VSub(playerPos, pos_);
	int size = VSize(distance) / 100;
	VECTOR norm = VNorm(distance);
	
	bool noObject = false;

	for (int i = 0; i < size; i++) {
		VECTOR PointPos = VScale(norm, 100.0f * i);
		PointPos = VAdd(pos_, PointPos);
		noObject = Aster_->temp(PointPos);
		debug_[i] = PointPos;
		if (noObject) {
			break;
		}
	}

	return noObject;
}

