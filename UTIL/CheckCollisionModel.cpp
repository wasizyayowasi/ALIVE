#include "CheckCollisionModel.h"
#include "../object/Player.h"
#include "../util/ObjectManager.h"
#include "Model.h"

namespace {
	constexpr float collition_radius = 200.0f;
}

CheckCollisionModel::CheckCollisionModel()
{
}

CheckCollisionModel::~CheckCollisionModel()
{
}

void CheckCollisionModel::CheckCollisionPersonalArea(Player& player, VECTOR moveVec)
{
	//�X�V�O�̃|�W�V�������擾����
	oldPos = player.GetStatus().pos;
	//�X�V��̃|�W�V�������擾����
	nowPos = VAdd(player.GetStatus().pos, moveVec);
	//���f���Ƌ��̓����蔻��
	
	for (auto& model : ObjectManager::GetInstance().GetAllCheckCollModel()) {
		if (static_cast<AnimType>(player.GetStatus().animNo) == AnimType::carryWalking) {
			if (player.GetTempCustodyPointer() == model) {
				continue;
			}
		}
		MV1RefreshCollInfo(model->GetModelHandle(), model->GetColFrameIndex());
		CollModelState state;
		state.hitDim = MV1CollCheck_Sphere(model->GetModelHandle(), model->GetColFrameIndex(), oldPos, collition_radius + VSize(moveVec));
		state.model = model;
		hitDim_.push_back(state);
	}
	

	//�x�N�g���̐�Βl���擾���A�������������Ă��Ȃ����̃t���O���Ƃ�
	if (fabs(moveVec.x) > 0.01f || fabs(moveVec.z) > 0.01f) {
		moveFlag = true;
	}
	else {
		moveFlag = false;
	}

	CheckWallAndFloor();
	
}

void CheckCollisionModel::CheckWallAndFloor()
{
	hitWallNum = 0;
	hitFloorNum = 0;

	//�O�ɂƂ������f���Ƌ��̓����蔻�菈��
	int i = 0;
	for (auto& result : hitDim_) {
		for (i = 0; i < result.hitDim.HitNum; i++) {
			//���f���̖@���x�N�g��
			if (result.hitDim.Dim[i].Normal.y < 0.000001f && result.hitDim.Dim[i].Normal.y > -0.000001f) {
				if (result.hitDim.Dim[i].Position[0].y > oldPos.y + 1.0f ||
					result.hitDim.Dim[i].Position[1].y > oldPos.y + 1.0f ||
					result.hitDim.Dim[i].Position[2].y > oldPos.y + 1.0f)
				{
					//�ǃ|���S�����̎擾�H
					if (hitWallNum < max_hit_coll) {
						wallHitDim_[hitWallNum].hitDim = &result.hitDim.Dim[i];
						hitWallNum++;
					}

				}
			}
			else {
				//���|���S�����̎擾�H
				if (hitFloorNum < max_hit_coll) {
					floorHitDim_[hitFloorNum].hitDim = &result.hitDim.Dim[i];
					hitFloorNum++;
				}
			}
		}
	}
}

void CheckCollisionModel::CheckCollisionWall(VECTOR moveVec,float playerHeight)
{
	//�ǂ̏���
	if (hitWallNum != 0) {
		hitFlag = false;
		//�����Ă�����
		if (moveFlag) {
			int i = 0,j = 0;
			for (i = 0; i < hitWallNum; i++) {
				auto hitPoly = wallHitDim_[i];
				//�v���C���[�����ɂ����J�v�Z���ƕǃ|���S���̔���@�@�������Ă��Ȃ�������continue
				if (!HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2])){
					continue;
				}

				hitFlag = true;
				if (!isGoUpStep_) {
					VECTOR slideVec;
					//�v���C���[�̃x�N�g���ƃ|���S���̖@���x�N�g���̊O�ς��擾
					slideVec = VCross(moveVec, hitPoly.hitDim->Normal);
					//�v���C���[�̃x�N�g���ƃ|���S���̖@���x�N�g���̊O�ςƃ|���S���̊O�ς̖@���x�N�g���̊O�ς��擾
					slideVec = VCross(hitPoly.hitDim->Normal, slideVec);
					//�X�V�O�̃v���C���[�̃|�W�V�����Ə�L�̊O�ς��擾
					nowPos = VAdd(oldPos, slideVec);

					//�܂������蔻��H
					for (j = 0; j < hitWallNum; j++) {
						hitPoly = wallHitDim_[j];
						if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2])) {
							break;
						}
					}
				}

				//�������Ă��Ȃ�������t���O��܂�
				if (j == hitWallNum) {
					hitFlag = false;
					break;
				}
			}
		}
	}
	else {
		int i = 0;
		//����ǂƂ�hit��񂪂Ȃ������ꍇ
		for (i = 0; i < hitWallNum; i++) {
			auto hitPoly = wallHitDim_[i];
			if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2])) {
				hitFlag = false;
				break;
			}
		}
	}

	// �ǂɓ������Ă�����ǂ��牟���o���������s��
	//���������|���S���̖@���x�N�g���̂T�{���v���C���[�̃|�W�V�����ɑ����Ă���
	if (hitFlag && !isGoUpStep_) {
		int i, j, k;
		for (k = 0; k < 16; k++) {
			for (i = 0; i < hitWallNum; i++) {
				auto hitPoly = wallHitDim_[i];
				if (!HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2])) {
					continue;
				}
				nowPos = VAdd(nowPos, VScale(hitPoly.hitDim->Normal, 5.0f));
				for (j = 0; j < hitWallNum; j++) {
					hitPoly = wallHitDim_[j];
					if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2])) {
						break;
					}
				}
				if (j == hitWallNum)break;
			}
			if (i != hitWallNum)break;
		}
	}
}

void CheckCollisionModel::CheckCollisionFloor(Player& player, VECTOR moveVec,bool jumpFlag, float playerHeight)
{

	float jumpVec = player.GetStatus().jump.jumpVec;
	bool isJump = player.GetStatus().jump.isJump;

	//���Ƃ̓����蔻��
	if (hitFloorNum != 0) {

		if (isJump && jumpVec > 0.0f) {
			float minY = 0.0f;
			hitFlag = false;
			for (int i = 0; i < hitFloorNum; i++) {
				auto hitPoly = floorHitDim_[i];
				hitLineResult = HitCheck_Line_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]);
				if (hitLineResult.HitFlag == FALSE)continue;
				if (hitFlag == true && minY < hitLineResult.Position.y)continue;
				hitFlag = true;
				minY = hitLineResult.Position.y;
			}
			if (hitFlag) {
				nowPos.y = minY - playerHeight;
				if (jumpVec != 36.0f) {
					jumpVec = -jumpVec;
				}
			}
		}
		else {
			float maxY = 0.0f;
			hitFlag = false;
			for (int i = 0; i < hitFloorNum; i++) {
				auto hitPoly = floorHitDim_[i];
				if (isJump) {
					hitLineResult = HitCheck_Line_Triangle(VAdd(nowPos, VGet(0, playerHeight, 0)), VAdd(nowPos, VGet(0.0f, -10.0f, 0.0f)), hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]);
				}
				else {
					hitLineResult = HitCheck_Line_Triangle(VAdd(nowPos, VGet(0, playerHeight, 0)), VAdd(nowPos, VGet(0.0f, -playerHeight, 0.0f)), hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]);
				}
				if (hitLineResult.HitFlag == false) {
					continue;
				}
				if (hitFlag && maxY > hitLineResult.Position.y) {
					continue;
				}
				if (player.GetStatus().animNo == 11) {
					continue;
				}
				hitFlag = true;
				maxY = hitLineResult.Position.y;
			}
			if (hitFlag) {
				nowPos.y = maxY;
				jumpVec = 0.0f;
				if (isJump) {
					isJump = false;
				}
			}
			else {
				isJump = true;
			}
		}
	}

	player.SetJumpInfo(isJump, jumpVec);

}

void CheckCollisionModel::CheckCollision(Player& player, VECTOR moveVec, float playerHeight, bool isJump, float jumpVec)
{
	//�v���C���[������͈͂̏Փ˔�����Ƃ�
	CheckCollisionPersonalArea(player,moveVec);

	CheckCollSpecificModel(player);
	//�Փ˂����I�u�W�F�N�g�����z���邱�Ƃ��o����I�u�W�F�N�g�����f����
	CheckStepDifference(player,playerHeight);
	//�擾�����Փˌ��ʂ���ǂɓ��������ꍇ�̏���
	CheckCollisionWall(moveVec,playerHeight);
	//�擾�����Փˌ��ʂ��珰�ɓ��������ꍇ�̏���
	CheckCollisionFloor(player, moveVec,isJump,playerHeight);

	//�|�W�V�����̃Z�b�g
	player.SetPos(nowPos);

	//�Փ˔���̏���
	for (auto& hit : hitDim_) {
		MV1CollResultPolyDimTerminate(hit.hitDim);
	}

	hitDim_.clear();
}

void CheckCollisionModel::CheckStepDifference(Player& player, float playerHeight)
{
	objectHeightY = 0;
	isGoUpStep_ = false;

	//���z������i�������f���邽��
	bool overHeight = false;

	//�v���C���[����ɂ����J�v�Z���Ɠ��������|���S���̓����蔻����s��
	//�v���C���[�̍��������|���S���̍������Ⴉ������i������鏈�������
	if (hitWallNum != 0) {
		for (int i = 0; i < hitWallNum; i++) {
			auto hitPoly = wallHitDim_[i];
			if (!HitCheck_Capsule_Triangle(VGet(nowPos.x, nowPos.y, nowPos.z), VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2])) {
				continue;
			}

			//�Փ˂����|���S���̈��Y���̍������_�������o��
			//���z������i���ȏゾ������overHeight��true�ɂ���
			for (int i = 0; i < 3; i++) {
				if (nowPos.y + 60 < hitPoly.hitDim->Position[i].y) {
					overHeight = true;
					if (nowPos.y + playerHeight > hitPoly.hitDim->Position[i].y) {
						player.SetClim(overHeight);
					}
				}
				else {
					player.SetClim(false);
				}
			}

			//���z���邱�Ƃ��ł��鍂���ň�ԍ���Y���̒l��������
			if (!overHeight) {
				for (int i = 0; i < 3; i++) {
					if (objectHeightY < hitPoly.hitDim->Position[i].y) {
						objectHeightY = hitPoly.hitDim->Position[i].y;
						isGoUpStep_ = true;
					}
				}
			}
		}
	}

	//���
	if (objectHeightY > 0.0f) {
		nowPos.y = objectHeightY;
	}

	if (!overHeight) {
		player.SetClim(false);
	}

}

void CheckCollisionModel::CheckCollSpecificModel(Player& player)
{
	for (auto& obj : ObjectManager::GetInstance().GetSpecificModel(ObjectType::deadPerson)) {
		for (auto& hit : hitDim_)
		{
			if (hit.model != obj) {
				continue;
			}

			auto result = MV1CollCheck_Capsule(hit.model->GetModelHandle(), hit.model->GetColFrameIndex(), player.GetStatus().pos, VAdd(player.GetStatus().pos, VGet(0, player.GetStatus().height, 0)),30);
			if (result.HitNum > 0) {
				player.SetCarryInfo(true,hit.model);
			}

		}
	}
}
