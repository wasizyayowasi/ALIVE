#include "EnemyBase.h"
#include "Player.h"
#include "ShotManager.h"
#include "../util/Model.h"
#include "../object/ObjectManager.h"
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

	//�G���f���̍���
	constexpr float model_height = 150.0f;

	//���[�`
	constexpr float within_reach = 80.0f;

	//�E��̃t���[����
	const char* const hand_framename = "mixamorig:RightHandIndex2";

	//���𓊂��Ă���Ƃ��̃A�j���[�V�����t���[��
	constexpr int throw_frame_time = 73;

	//�΂𓊂��鋗��
	constexpr float throw_distance = 800.0f;
}

EnemyBase::EnemyBase(int handle, Material materialType, LoadObjectInfo objInfo) : CharacterBase(handle, materialType, objInfo)
{
	model_->SetAnimation(0, true, false);
	Aster_ = std::make_shared<Aster>(objInfo.pos);

	//���ʃx�N�g���̎擾
	MATRIX matRotY = MGetRotY(objInfo.rot.y);
	frontVec_ = VTransform(init_rot, matRotY);

	initFrontVec_ = frontVec_;

	//�}�e���A���̐F��ς���
	MV1SetMaterialDifColor(model_->GetModelHandle(), 8, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
}

void EnemyBase::Update(Player& player)
{
	//���f���̍X�V
	model_->Update();

	if (isThrow_) {
		return;
	}

	isDetection_ = false;

	//�v���C���[�̍��W
	VECTOR playerPos = player.GetStatus().pos;

	//�v���C���[�����G����
	SearchForPlayer(playerPos);

	//���G
	if (!IsThereAnObject(playerPos)) {
		if (isDetection_) {
			//�v���C���[��ǐՂ���
			TrackingUpdate(playerPos);
		}
	}
	else{
		if (isDetection_) {
			//�ڕW�}�X�̒��S���W���擾
			Aster_->LocationInformation(playerPos, pos_);

			//�o�H�T��
			RoutingUpdate(player);
		}
	}

	//�G����v���C���[�̒�������
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(playerPos, pos_);

	if (distanceSize < within_reach) {
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
	//�v���C���[�Ǝ����̍����Z�o����
	VECTOR distancePlayerAndEnemy = VSub(playerPos, pos_);

	//Y���W�̒ǐՂ͍l�����Ȃ�
	distancePlayerAndEnemy.y = 0;

	//�ړ��x�N�g���̐���
	VECTOR moveVec = VScale(VNorm(distancePlayerAndEnemy), move_speed);

	//��]�s��Ɗg�k�s��̍����s��
	MATRIX mtx = CombiningRotAndScallMat(distancePlayerAndEnemy);

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
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(playerPos,pos_);
	float innerProduct = 0.0f;

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

	//�G�̎���p�����O���ɂ���܂���
	//�G����v���C���[�̋������w��͈͂��傫��������
	//idle�A�j���[�V�����A����ȊO������������A�j���[�V�����ɕύX����
	if (innerProduct > viewing_angle || distanceSize > visible_range) {
		model_->ChangeAnimation(static_cast<int>(EnemyAnimType::Idle), true, false, 20);
	}
	else {
		model_->ChangeAnimation(static_cast<int>(EnemyAnimType::Walk), true, false, 20);
	}
}

//�v���C���[��˂���΂�
void EnemyBase::ThrustAway(Player& player)
{
	pushVec_ = VScale(pushVec_, 0.96f);
	float size = VSize(pushVec_);
	VECTOR nockback = VAdd(player.GetStatus().pos,pushVec_);

	//�J�������̈�胉�C�����߂����
	//Z���W�̃x�N�g����0�ɂ���
	if (nockback.z < -250.0f) {
		pushVec_.z = 0.0f;
	}

	player.SetMoveVec(VAdd(player.GetStatus().moveVec, pushVec_));
}

void EnemyBase::RoutingUpdate(Player& player)
{
	//�v���C���[�̍��W
	VECTOR playerPos = player.GetStatus().pos;

	//�G�l�~�[�����ɖڎw�����̒��S���W
	VECTOR targetPos = Aster_->GetDestinationCoordinates(playerPos,pos_);

	//�ڕW�n�_�ƃ|�W�V�����̋������擾
	VECTOR distance = VSub(targetPos, pos_);

	//�G�l�~�[�Ɩڎw�����̒��S���W�Ƃ̋����̃x�N�g���T�C�Y
	float size = VSize(distance);

	//��]�s��Ɗg�k�s��̍����s��
	MATRIX mtx = CombiningRotAndScallMat(distance);

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
	}
}

bool EnemyBase::IsThereAnObject(VECTOR playerPos)
{
	//�G�l�~�[�ƃv���C���[�̋���
	VECTOR distance = VSub(playerPos, pos_);

	//�G�l�~�[�ƃv���C���[�̋����̃x�N�g���T�C�Y
	int size = static_cast<int>(VSize(distance) / 50);

	//�G�l�~�[�ƃv���C���[�̋����̐��K��
	VECTOR norm = VNorm(distance);
	
	bool noObject = false;

	for (int i = 0; i < size; i++) {
		VECTOR PointPos = VScale(norm, 50.0f * i);
		PointPos = VAdd(pos_, PointPos);
		noObject = Aster_->SearchBlockadeMode(PointPos);
		if (noObject) {
			break;
		}
	}

	return noObject;
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

MATRIX EnemyBase::CombiningRotAndScallMat(VECTOR distance)
{
	//��]�s��̎擾
	MATRIX rotMtx = MGetRotVec2(init_rot, distance);

	//���K���������ʃx�N�g�����擾����
	frontVec_ = VTransform(init_rot, rotMtx);

	//�g�k�s��̎擾
	MATRIX scaleMtx = MGetScale(scale_);

	//��]�s��Ɗg�k�s��̊|���Z
	MATRIX mtx = MMult(rotMtx, scaleMtx);

	return mtx;
}

//�v���C���[�̗����e�Ɏg�p���钸�_���擾
VECTOR EnemyBase::VertexPosition(float angle)
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
	pos = VAdd(pos_, pos);

	//Y���W�͐��ƃ|���S���̓����蔻��Ŏ擾����
	//��ԋ߂��|���S����Y���W��������
	pos.y = pos_.y;

	return pos;
}

//�����e���쐬�A�`��
void EnemyBase::DrawPolygon3D()
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
	vertex[0].pos = VGet(pos_.x, pos_.y, pos_.z);
	vertex[0].norm = norm;
	vertex[0].dif = difColor;
	vertex[0].spc = GetColorU8(0, 0, 0, 0);
	vertex[0].u = 0.0f;
	vertex[0].v = 0.0f;
	vertex[0].su = 0.0f;
	vertex[0].sv = 0.0f;

	//�p�x���Ƃ̒��_���擾
	for (int i = 1; i < 7; i++) {
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