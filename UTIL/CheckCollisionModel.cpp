#include "CheckCollisionModel.h"
#include "../OBJECT/Player.h"
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
		MV1RefreshCollInfo(models[i]->getModelHandle(), -1);
		hitDim_.push_back(MV1CollCheck_Sphere(models[i]->getModelHandle(), -1, oldPos, collition_radius + VSize(moveVec)));
	}
	

	//�x�N�g���̐�Βl���擾���A�������������Ă��Ȃ����̃t���O���Ƃ�
	if (fabs(moveVec.x) > 0.01f || fabs(moveVec.z) > 0.01f) {
		moveFlag = true;
	}
	else {
		moveFlag = false;
	}

	kabeNum = 0;
	yukaNum = 0;

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
					if (kabeNum < max_hit_coll) {
						kabe[kabeNum] = &result.Dim[i];
						kabeNum++;
					}

				}
			}
			else {
				//���|���S�����̎擾�H
				if (yukaNum < max_hit_coll) {
					yuka[yukaNum] = &result.Dim[i];
					yukaNum++;
				}
			}
		}
	}
	
}

void CheckCollisionModel::checkCollisionWall(VECTOR moveVec,float playerHeight)
{
	//�ǂ̏���
	if (kabeNum != 0) {
		hitFlag = false;
		//�����Ă�����
		if (moveFlag == true) {
			for (i = 0; i < kabeNum; i++) {
				poly = kabe[i];
				//�v���C���[�����ɂ����J�v�Z���ƕǃ|���S���̔���@�@�������Ă��Ȃ�������continue
				if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, poly->Position[0], poly->Position[1], poly->Position[2]) == FALSE) continue;

				hitFlag = true;

				VECTOR slideVec;
				//�v���C���[�̃x�N�g���ƃ|���S���̖@���x�N�g���̊O�ς��擾
				slideVec = VCross(moveVec, poly->Normal);
				//�v���C���[�̃x�N�g���ƃ|���S���̖@���x�N�g���̊O�ςƃ|���S���̊O�ς̖@���x�N�g���̊O�ς��擾
				slideVec = VCross(poly->Normal, slideVec);
				//�X�V�O�̃v���C���[�̃|�W�V�����Ə�L�̊O�ς��擾
				nowPos = VAdd(oldPos, slideVec);

				//�܂������蔻��H
				for (j = 0; j < kabeNum; j++) {
					poly = kabe[j];
					if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE) break;
				}

				//�������Ă��Ȃ�������t���O��܂�
				if (j == kabeNum) {
					hitFlag = false;
					break;
				}
			}
		}
	}
	else {
		//����ǂƂ�hit��񂪂Ȃ������ꍇ
		for (i = 0; i < kabeNum; i++) {
			poly = kabe[i];
			if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE) {
				hitFlag = false;
				break;
			}
		}

	}

	// �ǂɓ������Ă�����ǂ��牟���o���������s��
	if (hitFlag) {
		for (k = 0; k < 16; k++) {
			for (i = 0; i < kabeNum; i++) {
				poly = kabe[i];
				if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, poly->Position[0], poly->Position[1], poly->Position[2]) == FALSE) continue;
				nowPos = VAdd(nowPos, VScale(poly->Normal, 5.0f));
				for (int j = 0; j < kabeNum; j++) {
					poly = kabe[j];
					if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE) break;
				}
				if (j == kabeNum)break;
			}
			if (i != kabeNum)break;
		}
	}
}

void CheckCollisionModel::checkCollisionFloor(Player& player, VECTOR moveVec,bool jumpFlag, float playerHeight)
{

	float jumpVec = player.getJumpInfo().jumpVec;
	bool isJump = player.getJumpInfo().isJump;

	//���Ƃ̓����蔻��
	if (yukaNum != 0) {

		if (isJump && jumpVec > 0.0f) {
			float minY = 0.0f;
			hitFlag = false;
			for (i = 0; i < yukaNum; i++) {
				poly = yuka[i];
				lineRes = HitCheck_Line_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), poly->Position[0], poly->Position[1], poly->Position[2]);
				if (lineRes.HitFlag == FALSE)continue;
				if (hitFlag == 1 && minY < lineRes.Position.y)continue;
				hitFlag = true;
				minY = lineRes.Position.y;
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
			for (i = 0; i < yukaNum; i++) {
				poly = yuka[i];
				if (isJump) {
					lineRes = HitCheck_Line_Triangle(VAdd(nowPos, VGet(0, playerHeight, 0)), VAdd(nowPos, VGet(0.0f, -10.0f, 0.0f)), poly->Position[0], poly->Position[1], poly->Position[2]);
				}
				else {
					lineRes = HitCheck_Line_Triangle(VAdd(nowPos, VGet(0, playerHeight, 0)), VAdd(nowPos, VGet(0.0f, -playerHeight, 0.0f)), poly->Position[0], poly->Position[1], poly->Position[2]);
				}
				if (lineRes.HitFlag == false) continue;
				if (hitFlag && maxY > lineRes.Position.y)continue;
				hitFlag = true;
				maxY = lineRes.Position.y;
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
	//�擾�����Փˌ��ʂ���ǂɓ��������ꍇ�̏���
	checkCollisionWall(moveVec,playerHeight);
	//�擾�����Փˌ��ʂ��珰�ɓ��������ꍇ�̏���
	checkCollisionFloor(player, moveVec,isJump,playerHeight);

	//�|�W�V�����̃Z�b�g
	player.setPos(nowPos);

	//�Փ˔���̏���

	for (auto& hit : hitDim_) {
		hitDim_.erase(hitDim_.begin());
	}

	/*for (auto& result : hitDim_) {
		hitDim_.pop_back();
		hitDim_.erase(hitDim_.back());
	}*/
}
