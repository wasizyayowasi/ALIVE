#include "CheckCollisionModel.h"
#include "../object/Player.h"
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

void CheckCollisionModel::checkCollisionPersonalArea(Player& player, VECTOR moveVec, std::vector<std::shared_ptr<Model>> models)
{
	//�X�V�O�̃|�W�V�������擾����
	oldPos = player.getPos();
	//�X�V��̃|�W�V�������擾����
	nowPos = VAdd(player.getPos(), moveVec);
	//���f���Ƌ��̓����蔻��
	for (int i = 0; i < models.size();i++) {
		MV1RefreshCollInfo(models[i]->getModelHandle(), models[i]->getColFrameIndex());
		hitDim_.push_back(MV1CollCheck_Sphere(models[i]->getModelHandle(), models[i]->getColFrameIndex(), oldPos, collition_radius + VSize(moveVec)));
	}
	

	//�x�N�g���̐�Βl���擾���A�������������Ă��Ȃ����̃t���O���Ƃ�
	if (fabs(moveVec.x) > 0.01f || fabs(moveVec.z) > 0.01f) {
		moveFlag = true;
	}
	else {
		moveFlag = false;
	}

	hitWallNum = 0;
	hitFloorNum = 0;

	//�O�ɂƂ������f���Ƌ��̓����蔻�菈��
	for (auto& result : hitDim_) {
		for (i = 0; i < result.HitNum; i++) {
			//���f���̖@���x�N�g��
			if (result.Dim[i].Normal.y < 0.000001f && result.Dim[i].Normal.y > -0.000001f) {
				if (result.Dim[i].Position[0].y > oldPos.y + 1.0f ||
					result.Dim[i].Position[1].y > oldPos.y + 1.0f ||
					result.Dim[i].Position[2].y > oldPos.y + 1.0f)
				{
					//�ǃ|���S�����̎擾�H
					if (hitWallNum < max_hit_coll) {
						wallHitDim_[hitWallNum] = &result.Dim[i];
						hitWallNum++;
					}

				}
			}
			else {
				//���|���S�����̎擾�H
				if (hitFloorNum < max_hit_coll) {
					floorHitDim_[hitFloorNum] = &result.Dim[i];
					hitFloorNum++;
				}
			}
		}
	}
	
}

void CheckCollisionModel::checkCollisionWall(VECTOR moveVec,float playerHeight)
{
	//�ǂ̏���
	if (hitWallNum != 0) {
		hitFlag = false;
		//�����Ă�����
		if (moveFlag) {
			for (i = 0; i < hitWallNum; i++) {
				hitPoly = wallHitDim_[i];
				//�v���C���[�����ɂ����J�v�Z���ƕǃ|���S���̔���@�@�������Ă��Ȃ�������continue
				if (!HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly->Position[0], hitPoly->Position[1], hitPoly->Position[2])) continue;

				hitFlag = true;
				if (!isGoUpStep_) {
					VECTOR slideVec;
					//�v���C���[�̃x�N�g���ƃ|���S���̖@���x�N�g���̊O�ς��擾
					slideVec = VCross(moveVec, hitPoly->Normal);
					//�v���C���[�̃x�N�g���ƃ|���S���̖@���x�N�g���̊O�ςƃ|���S���̊O�ς̖@���x�N�g���̊O�ς��擾
					slideVec = VCross(hitPoly->Normal, slideVec);
					//�X�V�O�̃v���C���[�̃|�W�V�����Ə�L�̊O�ς��擾
					nowPos = VAdd(oldPos, slideVec);

					//�܂������蔻��H
					for (j = 0; j < hitWallNum; j++) {
						hitPoly = wallHitDim_[j];
						if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly->Position[0], hitPoly->Position[1], hitPoly->Position[2])) break;
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
		//����ǂƂ�hit��񂪂Ȃ������ꍇ
		for (i = 0; i < hitWallNum; i++) {
			hitPoly = wallHitDim_[i];
			if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly->Position[0], hitPoly->Position[1], hitPoly->Position[2])) {
				hitFlag = false;
				break;
			}
		}
	}

	// �ǂɓ������Ă�����ǂ��牟���o���������s��
	//���������|���S���̖@���x�N�g���̂T�{���v���C���[�̃|�W�V�����ɑ����Ă���
	if (hitFlag && !isGoUpStep_) {
		for (k = 0; k < 16; k++) {
			for (i = 0; i < hitWallNum; i++) {
				hitPoly = wallHitDim_[i];
				if (!HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly->Position[0], hitPoly->Position[1], hitPoly->Position[2])) continue;
				nowPos = VAdd(nowPos, VScale(hitPoly->Normal, 5.0f));
				for (int j = 0; j < hitWallNum; j++) {
					hitPoly = wallHitDim_[j];
					if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly->Position[0], hitPoly->Position[1], hitPoly->Position[2])) break;
				}
				if (j == hitWallNum)break;
			}
			if (i != hitWallNum)break;
		}
	}

	

}

void CheckCollisionModel::checkCollisionFloor(Player& player, VECTOR moveVec,bool jumpFlag, float playerHeight)
{

	float jumpVec = player.getJumpInfo().jumpVec;
	bool isJump = player.getJumpInfo().isJump;

	//���Ƃ̓����蔻��
	if (hitFloorNum != 0) {

		if (isJump && jumpVec > 0.0f) {
			float minY = 0.0f;
			hitFlag = false;
			for (i = 0; i < hitFloorNum; i++) {
				hitPoly = floorHitDim_[i];
				hitLineResult = HitCheck_Line_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), hitPoly->Position[0], hitPoly->Position[1], hitPoly->Position[2]);
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
			for (i = 0; i < hitFloorNum; i++) {
				hitPoly = floorHitDim_[i];
				if (isJump) {
					hitLineResult = HitCheck_Line_Triangle(VAdd(nowPos, VGet(0, playerHeight, 0)), VAdd(nowPos, VGet(0.0f, -10.0f, 0.0f)), hitPoly->Position[0], hitPoly->Position[1], hitPoly->Position[2]);
				}
				else {
					hitLineResult = HitCheck_Line_Triangle(VAdd(nowPos, VGet(0, playerHeight, 0)), VAdd(nowPos, VGet(0.0f, -playerHeight, 0.0f)), hitPoly->Position[0], hitPoly->Position[1], hitPoly->Position[2]);
				}
				if (hitLineResult.HitFlag == false) continue;
				if (hitFlag && maxY > hitLineResult.Position.y)continue;
				if (player.animNo_ == 11) continue;
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

	player.setJumpInfo(isJump, jumpVec);

}

void CheckCollisionModel::checkCollision(Player& player, VECTOR moveVec, std::vector<std::shared_ptr<Model>> model, float playerHeight, bool isJump, float jumpVec)
{
	//�v���C���[������͈͂̏Փ˔�����Ƃ�
	checkCollisionPersonalArea(player,moveVec,model);
	//�Փ˂����I�u�W�F�N�g�����z���邱�Ƃ��o����I�u�W�F�N�g�����f����
	checkStepDifference(player,playerHeight);
	//�擾�����Փˌ��ʂ���ǂɓ��������ꍇ�̏���
	checkCollisionWall(moveVec,playerHeight);
	//�擾�����Փˌ��ʂ��珰�ɓ��������ꍇ�̏���
	checkCollisionFloor(player, moveVec,isJump,playerHeight);

	//�|�W�V�����̃Z�b�g
	player.setPos(nowPos);

	//�Փ˔���̏���
	for (auto& hit : hitDim_) {
		MV1CollResultPolyDimTerminate(hit);
	}

	hitDim_.erase(hitDim_.begin(),hitDim_.end());
}

void CheckCollisionModel::checkStepDifference(Player& player, float playerHeight)
{
	objectHeightY = 0;
	isGoUpStep_ = false;

	//���z������i�������f���邽��
	bool overHeight = false;

	//�v���C���[����ɂ����J�v�Z���Ɠ��������|���S���̓����蔻����s��
	//�v���C���[�̍��������|���S���̍������Ⴉ������i������鏈�������
	if (hitWallNum != 0) {
		for (int i = 0; i < hitWallNum; i++) {
			hitPoly = wallHitDim_[i];
			if (!HitCheck_Capsule_Triangle(VGet(nowPos.x, nowPos.y, nowPos.z), VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly->Position[0], hitPoly->Position[1], hitPoly->Position[2])) continue;

			//�Փ˂����|���S���̈��Y���̍������_�������o��
			//���z������i���ȏゾ������overHeight��true�ɂ���
			for (int i = 0; i < 3; i++) {
				if (nowPos.y + 60 < hitPoly->Position[i].y) {
					overHeight = true;
					if (nowPos.y + playerHeight > hitPoly->Position[i].y) {
						player.isClim_ = overHeight;
					}
				}
				else {
					player.isClim_ = false;
				}
			}

			//���z���邱�Ƃ��ł��鍂���ň�ԍ���Y���̒l��������
			if (!overHeight) {
				for (int i = 0; i < 3; i++) {
					if (objectHeightY < hitPoly->Position[i].y) {
						objectHeightY = hitPoly->Position[i].y;
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
		player.isClim_ = false;
	}

}
