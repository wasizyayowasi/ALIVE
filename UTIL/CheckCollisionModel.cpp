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
	//更新前のポジションを取得する
	oldPos = player.getPos();
	//更新後のポジションを取得する
	nowPos = VAdd(player.getPos(), moveVec);
	//モデルと球の当たり判定
	for (int i = 0; i < models.size();i++) {
		MV1RefreshCollInfo(models[i]->getModelHandle(), models[i]->getColFrameIndex());
		hitDim_.push_back(MV1CollCheck_Sphere(models[i]->getModelHandle(), models[i]->getColFrameIndex(), oldPos, collition_radius + VSize(moveVec)));
	}
	

	//ベクトルの絶対値を取得し、動いたか動いていないかのフラグをとる
	if (fabs(moveVec.x) > 0.01f || fabs(moveVec.z) > 0.01f) {
		moveFlag = true;
	}
	else {
		moveFlag = false;
	}

	hitWallNum = 0;
	hitFloorNum = 0;

	//前にとったモデルと球の当たり判定処理
	for (auto& result : hitDim_) {
		for (i = 0; i < result.HitNum; i++) {
			//モデルの法線ベクトル
			if (result.Dim[i].Normal.y < 0.000001f && result.Dim[i].Normal.y > -0.000001f) {
				if (result.Dim[i].Position[0].y > oldPos.y + 1.0f ||
					result.Dim[i].Position[1].y > oldPos.y + 1.0f ||
					result.Dim[i].Position[2].y > oldPos.y + 1.0f)
				{
					//壁ポリゴン情報の取得？
					if (hitWallNum < max_hit_coll) {
						wallHitDim_[hitWallNum] = &result.Dim[i];
						hitWallNum++;
					}

				}
			}
			else {
				//床ポリゴン情報の取得？
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
	//壁の処理
	if (hitWallNum != 0) {
		hitFlag = false;
		//動いていたら
		if (moveFlag == true) {
			for (i = 0; i < hitWallNum; i++) {
				hitPoly = wallHitDim_[i];
				//プレイヤーを元にしたカプセルと壁ポリゴンの判定　　当たっていなかったらcontinue
				if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly->Position[0], hitPoly->Position[1], hitPoly->Position[2]) == FALSE) continue;

				hitFlag = true;

				if (hitPoly->HitPosition.y < oldPos.y + 50.0f) {
					nowPos = VAdd(oldPos, VGet(0, 30.0f, 0));
				}
				else {
					VECTOR slideVec;
					//プレイヤーのベクトルとポリゴンの法線ベクトルの外積を取得
					slideVec = VCross(moveVec, hitPoly->Normal);
					//プレイヤーのベクトルとポリゴンの法線ベクトルの外積とポリゴンの外積の法線ベクトルの外積を取得
					slideVec = VCross(hitPoly->Normal, slideVec);
					//更新前のプレイヤーのポジションと上記の外積を取得
					nowPos = VAdd(oldPos, slideVec);
				}

				//また当たり判定？
				for (j = 0; j < hitWallNum; j++) {
					hitPoly = wallHitDim_[j];
					if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly->Position[0], hitPoly->Position[1], hitPoly->Position[2]) == TRUE) break;
				}

				//当たっていなかったらフラグを折る
				if (j == hitWallNum) {
					hitFlag = false;
					break;
				}
			}
		}
	}
	else {
		//一つも壁とのhit情報がなかった場合
		for (i = 0; i < hitWallNum; i++) {
			hitPoly = wallHitDim_[i];
			if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly->Position[0], hitPoly->Position[1], hitPoly->Position[2]) == TRUE) {
				hitFlag = false;
				break;
			}
		}

	}

	// 壁に当たっていたら壁から押し出す処理を行う
	if (hitFlag) {
		for (k = 0; k < 16; k++) {
			for (i = 0; i < hitWallNum; i++) {
				hitPoly = wallHitDim_[i];
				if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly->Position[0], hitPoly->Position[1], hitPoly->Position[2]) == FALSE) continue;
				nowPos = VAdd(nowPos, VScale(hitPoly->Normal, 5.0f));
				for (int j = 0; j < hitWallNum; j++) {
					hitPoly = wallHitDim_[j];
					if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly->Position[0], hitPoly->Position[1], hitPoly->Position[2]) == TRUE) break;
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

	//床との当たり判定
	if (hitFloorNum != 0) {

		if (isJump && jumpVec > 0.0f) {
			float minY = 0.0f;
			hitFlag = false;
			for (i = 0; i < hitFloorNum; i++) {
				hitPoly = floorHitDim_[i];
				hitLineResult = HitCheck_Line_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), hitPoly->Position[0], hitPoly->Position[1], hitPoly->Position[2]);
				if (hitLineResult.HitFlag == FALSE)continue;
				if (hitFlag == 1 && minY < hitLineResult.Position.y)continue;
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
	//プレイヤーから一定範囲の衝突判定をとる
	checkCollisionPersonalArea(player,moveVec,model);
	//取得した衝突結果から壁に当たった場合の処理
	checkCollisionWall(moveVec,playerHeight);
	//取得した衝突結果から床に当たった場合の処理
	checkCollisionFloor(player, moveVec,isJump,playerHeight);

	//ポジションのセット
	player.setPos(nowPos);

	//衝突判定の消去
	for (auto& hit : hitDim_) {
		MV1CollResultPolyDimTerminate(hit);
	}

	hitDim_.erase(hitDim_.begin(),hitDim_.end());
}
