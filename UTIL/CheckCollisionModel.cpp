#include "CheckCollisionModel.h"

#include "../object/Player.h"
#include "../object/ObjectBase.h"
#include "../object/ObjectManager.h"

#include "Model.h"
#include "Util.h"
#include "SoundManager.h"

#include <algorithm>

namespace {
	constexpr float collition_radius = 200.0f;
}

CheckCollisionModel::CheckCollisionModel()
{
}

CheckCollisionModel::~CheckCollisionModel()
{
}

void CheckCollisionModel::CheckCollisionPersonalArea(std::shared_ptr<Player> player, std::shared_ptr<ObjectManager> objManager)
{
	//�Z�k��
	auto playerState = player->GetStatus();

	//�X�V�O�̃|�W�V�������擾����
	oldPos = playerState.pos;
	//�X�V��̃|�W�V�������擾����
	nowPos = VAdd(playerState.pos, playerState.moveVec);
	//���f���Ƌ��̓����蔻��
	
	objManager->AddCheckCollModel();
	for (auto& model : objManager->GetAllCheckCollModel()) {
		if (player->GetStatus().situation.isInTransit) {
			if (player->GetDeadPersonModelPointer()->GetModelPointer() == model) {
				continue;
			}
		}

		MV1RefreshCollInfo(model->GetModelHandle(), model->GetColFrameIndex());
		CollModelState state;
		state.hitDim = MV1CollCheck_Sphere(model->GetModelHandle(), model->GetColFrameIndex(), oldPos, collition_radius + VSize(playerState.moveVec));
		state.model = model;
		hitDim_.push_back(state);
	}
	

	//�x�N�g���̐�Βl���擾���A�������������Ă��Ȃ����̃t���O���Ƃ�
	if (fabs(playerState.moveVec.x) > 0.01f || fabs(playerState.moveVec.z) > 0.01f) {
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

void CheckCollisionModel::CheckCollisionWall(std::shared_ptr<Player> player)
{
	//�Z�k��
	auto playerState = player->GetStatus();

	//�ǂ̏���
	if (hitWallNum != 0) {
		hitFlag = false;
		//�����Ă�����
		if (moveFlag) {
			int i = 0,j = 0;
			for (i = 0; i < hitWallNum; i++) {
				auto hitPoly = wallHitDim_[i];
				//�v���C���[�����ɂ����J�v�Z���ƕǃ|���S���̔���@�@�������Ă��Ȃ�������continue
				if (!HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerState.height, 0.0f)), 20.0f, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2])){
					continue;
				}

				hitFlag = true;
				if (!isGoUpStep_) {
					VECTOR slideVec;
					//�v���C���[�̃x�N�g���ƃ|���S���̖@���x�N�g���̊O�ς��擾
					slideVec = VCross(playerState.moveVec, hitPoly.hitDim->Normal);
					//�v���C���[�̃x�N�g���ƃ|���S���̖@���x�N�g���̊O�ςƃ|���S���̊O�ς̖@���x�N�g���̊O�ς��擾
					slideVec = VCross(hitPoly.hitDim->Normal, slideVec);
					//�X�V�O�̃v���C���[�̃|�W�V�����Ə�L�̊O�ς��擾
					nowPos = VAdd(oldPos, slideVec);

					//�܂������蔻��H
					for (j = 0; j < hitWallNum; j++) {
						hitPoly = wallHitDim_[j];
						if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerState.height, 0.0f)), 20.0f, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2])) {
							player->SetMoveVec(VGet(0, 0, 0));
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
			if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerState.height, 0.0f)), 20.0f, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2])) {
				hitFlag = false;
				break;
			}
		}
	}


	//TODO�F�ȉ��̏�����ʂ̊֐��ɂ���
	// �ǂɓ������Ă�����ǂ��牟���o���������s��
	//���������|���S���̖@���x�N�g���̂T�{���v���C���[�̃|�W�V�����ɑ����Ă���
	if (hitFlag && !isGoUpStep_) {
		int i, j, k;
		for (k = 0; k < 16; k++) {
			for (i = 0; i < hitWallNum; i++) {
				auto hitPoly = wallHitDim_[i];
				if (!HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerState.height, 0.0f)), 20.0f, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2])) {
					continue;
				}
				nowPos = VAdd(nowPos, VScale(hitPoly.hitDim->Normal, 5.0f));
				for (j = 0; j < hitWallNum; j++) {
					hitPoly = wallHitDim_[j];
					if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerState.height, 0.0f)), 20.0f, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2])) {
						break;
					}
				}
				if (j == hitWallNum)break;
			}
			if (i != hitWallNum)break;
		}
	}
}

void CheckCollisionModel::CheckCollisionFloor(std::shared_ptr<Player> player)
{
	//�Z�k��
	auto playerState = player->GetStatus();

	float jumpVec = playerState.jump.jumpVec;
	bool isJump = playerState.jump.isJump;

	//���Ƃ̓����蔻��
	if (hitFloorNum != 0) {
		//�W�����v�����x�N�g�����}�C�i�X�łȂ����
		if (isJump && jumpVec > 0.0f) {
			float minY = 0.0f;
			hitFlag = false;
			for (int i = 0; i < hitFloorNum; i++) {
				//�f�[�^�̃R�s�[
				auto hitPoly = floorHitDim_[i];
				//�Փ˔��茋��
				hitLineResult_.push_back(HitCheck_Line_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerState.height, 0.0f)), hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));

				//�Փ˔��茋�ʂŏՓ˂��ĂȂ�������ȍ~�̏������s��Ȃ�
				if (hitLineResult_.back().HitFlag == false) {
					continue;
				}
				//�Փ˔��茋�ʂŏՓ˂��ĂȂ�������ȍ~�̏������s��Ȃ�
				if (hitFlag == true && minY < hitLineResult_.back().Position.y) {
					continue;
				}
				hitFlag = true;
				minY = hitLineResult_.back().Position.y;

				hitLineResult_.clear();

			}
			if (hitFlag) {
				nowPos.y = minY - playerState.height;
				if (jumpVec != 36.0f) {
					jumpVec = -jumpVec;
				}
			}
		}
		else {
			float maxY = nowPos.y + playerState.height;
			float correction = 20.0f;
			hitFlag = false;
			for (int i = 0; i < hitFloorNum; i++) {
				auto hitPoly = floorHitDim_[i];
				if (isJump) {
					hitLineResult_.push_back(HitCheck_Line_Triangle(VAdd(nowPos, VGet(  0, playerState.height,   0)), nowPos, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));
					hitLineResult_.push_back(HitCheck_Line_Triangle(VAdd(nowPos, VGet( correction, playerState.height,   0)), VAdd(nowPos, VGet( correction, 0,   0)), hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));
					hitLineResult_.push_back(HitCheck_Line_Triangle(VAdd(nowPos, VGet(-correction, playerState.height,   0)), VAdd(nowPos, VGet(-correction, 0,   0)), hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));
					hitLineResult_.push_back(HitCheck_Line_Triangle(VAdd(nowPos, VGet(  0, playerState.height,  correction)), VAdd(nowPos, VGet(  0, 0,  correction)), hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));
					hitLineResult_.push_back(HitCheck_Line_Triangle(VAdd(nowPos, VGet(  0, playerState.height, -correction)), VAdd(nowPos, VGet(  0, 0, -correction)), hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));
				}
				else {
					hitLineResult_.push_back(HitCheck_Line_Triangle(VAdd(nowPos, VGet(0, playerState.height, 0)), nowPos, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));
					hitLineResult_.push_back(HitCheck_Line_Triangle(VAdd(nowPos, VGet(correction, playerState.height, 0)), VAdd(nowPos, VGet(correction, 0, 0)), hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));
					hitLineResult_.push_back(HitCheck_Line_Triangle(VAdd(nowPos, VGet(-correction, playerState.height, 0)), VAdd(nowPos, VGet(-correction, 0, 0)), hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));
					hitLineResult_.push_back(HitCheck_Line_Triangle(VAdd(nowPos, VGet(0, playerState.height, correction)), VAdd(nowPos, VGet(0, 0, correction)), hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));
					hitLineResult_.push_back(HitCheck_Line_Triangle(VAdd(nowPos, VGet(0, playerState.height, -correction)), VAdd(nowPos, VGet(0, 0, -correction)), hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));
				}

				int hitCount = 0;
				HITRESULT_LINE hitLine = {};

				for (auto& result : hitLineResult_) {
					if (result.HitFlag == 1) {
						hitCount++;
						hitLine = result;
					}
				}
				
				if (hitCount < 1) {
					hitLineResult_.clear();
					continue;
				}

				int overCount = 0;

				for (auto& result : hitLineResult_) {
					if (maxY < result.Position.y) {
						overCount++;
					}
				}
				
				if (overCount > 0) {
					hitLineResult_.clear();
					continue;
				}

				hitFlag = true;
				maxY = hitLine.Position.y;
				result.hitDim = hitPoly.hitDim;
				hitLineResult_.clear();
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

	player->SetJumpInfo(isJump, jumpVec);

}

void CheckCollisionModel::CheckCollision(std::shared_ptr<Player> player, std::shared_ptr<ObjectManager> objManager)
{

	//�v���C���[������͈͂̏Փ˔�����Ƃ�
	CheckCollisionPersonalArea(player, objManager);

	CheckCollSpecificModel(player, objManager);
	//�Փ˂����I�u�W�F�N�g�����z���邱�Ƃ��o����I�u�W�F�N�g�����f����
	CheckStepDifference(player);
	//�擾�����Փˌ��ʂ���ǂɓ��������ꍇ�̏���
	CheckCollisionWall(player);
	//�擾�����Փˌ��ʂ��珰�ɓ��������ꍇ�̏���
	CheckCollisionFloor(player);

	FindThePolygonBelowThePlayer(player, objManager);

	//�|�W�V�����̃Z�b�g
	player->SetPos(nowPos);

	//�Փ˔���̏���
	for (auto& hit : hitDim_) {
		MV1CollResultPolyDimTerminate(hit.hitDim);
	}

	hitDim_.clear();
}

void CheckCollisionModel::CheckStepDifference(std::shared_ptr<Player> player)
{
	//�Z�k��
	auto playerState = player->GetStatus();

	objectHeightY = 0;
	isGoUpStep_ = false;

	//���z������i�������f���邽��
	bool overHeight = false;

	//�v���C���[����ɂ����J�v�Z���Ɠ��������|���S���̓����蔻����s��
	//�v���C���[�̍��������|���S���̍������Ⴉ������i������鏈�������
	if (hitWallNum != 0) {
		for (int i = 0; i < hitWallNum; i++) {
			auto hitPoly = wallHitDim_[i];
			if (!HitCheck_Capsule_Triangle(VGet(nowPos.x, nowPos.y, nowPos.z), VAdd(nowPos, VGet(0.0f, playerState.height, 0.0f)), 20.0f, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2])) {
				continue;
			}

			//�Փ˂����|���S���̈��Y���̍������_�������o��
			//���z������i���ȏゾ������overHeight��true�ɂ���
			for (int i = 0; i < 3; i++) {
				if (nowPos.y + 60 < hitPoly.hitDim->Position[i].y) {
					overHeight = true;
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
}

void CheckCollisionModel::FindThePolygonBelowThePlayer(std::shared_ptr<Player> player, std::shared_ptr<ObjectManager> objManager)
{
	std::unordered_map<Material, std::list<MV1_COLL_RESULT_POLY>> hitLine;

	//�I�u�W�F�N�g�̃|���S�����擾����
	for (auto& model : objManager->GetAllCheckCollModel()) {
		//�v���C���[�������������^��ł���ꍇ
		//�����^��ł���I�u�W�F�N�g�̃|���S�����擾���Ȃ��悤�ɂ��Ă���
		if (player->GetStatus().situation.isInTransit) {
			if (player->GetDeadPersonModelPointer()->GetModelPointer() == model) {
				continue;
			}
		}

		//�I�u�W�F�N�g�ƃv���C���[�̍����̍����擾����
		float distance = nowPos.y - model->GetPos().y;
		distance = (std::max)(distance, -distance);

		//�����̍���1000�ȏ゠������continue
		//if (distance > 1000.0f) {
		//	continue;
		//}

		//���f���Ɛ��̓����蔻������
		MV1RefreshCollInfo(model->GetModelHandle(), model->GetColFrameIndex());
		VECTOR playerHeadPos = VGet(nowPos.x, nowPos.y + player->GetStatus().height, nowPos.z);
		VECTOR lowPos = VGet(nowPos.x, nowPos.y - distance, nowPos.z);
		hitLine[model->GetMaterialType()].push_back(MV1CollCheck_Line(model->GetModelHandle(), model->GetColFrameIndex(), playerHeadPos, lowPos));
	}

	float nearPosY = 5000.0f;
	float resultY = 0.0f;
	float distanceY = 0.0f;
	Material materialType = Material::max;

	//�����蔻��̌��ʂ����ԋ߂��|���S����Y���W���擾����
	for (auto& list : hitLine) {
		for(auto& result : list.second)

		if (!result.HitFlag) {
			continue;
		}
		
		distanceY = nowPos.y - result.hitDim->Position->y;
		if (nearPosY > distanceY) {
			nearPosY = distanceY;
			resultY = result.hitDim->Position->y;
			materialType = list.first;
		}
	}

	//��ԋ߂�Y���W���ۉe��Y���W�Ƃ��Ďg��
	//�����蔻��̌��ʉ��Ƃ��������Ă��Ȃ������ꍇ
	//Y���W��0�ɂ���
	player->SetRoundShadowHeightAndMaterial(resultY,materialType);

	hitLine.clear();
}

void CheckCollisionModel::CheckCollSpecificModel(std::shared_ptr<Player> player, std::shared_ptr<ObjectManager> objManager)
{

	//�Z�k��
	auto playerState = player->GetStatus();

	//�v���C���[������ʂ̎��̂������^��ł�����擾���Ȃ�
	if (player->GetStatus().situation.isInTransit) {
		return;
	}

	//�����^�Ԏ��̂��擾����
	for (auto& obj : objManager->GetSpecificObject(ObjectType::DeadPerson)) {
		for (auto& hit : hitDim_)
		{
			//�Փˌ��ʂ����̈ȊO��������continue
			if (hit.model != obj->GetModelPointer()) {
				continue;
			}

			//���̂̃��f���ƃv���C���[�̍��W����ɍ쐬���ꂽ�J�v�Z���Ƃ̏Փ˔���
			auto result = MV1CollCheck_Capsule(hit.model->GetModelHandle(), hit.model->GetColFrameIndex(), playerState.pos, VAdd(playerState.pos, VGet(0, playerState.height, 0)),30);

			//��L�̏Փ˔���̌��ʂ�1�ł�����΃v���C���[��
			//���̎��̂̃|�C���^�[�Ǝ����^�ׂ�t���O�𑗂�
			if (result.HitNum > 0) {
				player->SetCarryInfo(true,obj);
			}

			MV1CollResultPolyDimTerminate(result);

		}
	}

}
