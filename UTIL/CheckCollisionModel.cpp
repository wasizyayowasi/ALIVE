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

void CheckCollisionModel::checkCollisionPersonalArea(Player& player, VECTOR moveVec)
{
	//更新前のポジションを取得する
	oldPos = player.getPos();
	//更新後のポジションを取得する
	nowPos = VAdd(player.getPos(), moveVec);
	//モデルと球の当たり判定
	
	for (auto& model : ObjectManager::getInstance().getCheckCollModel()) {
		MV1RefreshCollInfo(model->getModelHandle(), model->getColFrameIndex());
		hitDim_.push_back(MV1CollCheck_Sphere(model->getModelHandle(), model->getColFrameIndex(), oldPos, collition_radius + VSize(moveVec)));
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
		if (moveFlag) {
			for (i = 0; i < hitWallNum; i++) {
				hitPoly = wallHitDim_[i];
				//プレイヤーを元にしたカプセルと壁ポリゴンの判定　　当たっていなかったらcontinue
				if (!HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly->Position[0], hitPoly->Position[1], hitPoly->Position[2])) continue;

				hitFlag = true;
				if (!isGoUpStep_) {
					VECTOR slideVec;
					//プレイヤーのベクトルとポリゴンの法線ベクトルの外積を取得
					slideVec = VCross(moveVec, hitPoly->Normal);
					//プレイヤーのベクトルとポリゴンの法線ベクトルの外積とポリゴンの外積の法線ベクトルの外積を取得
					slideVec = VCross(hitPoly->Normal, slideVec);
					//更新前のプレイヤーのポジションと上記の外積を取得
					nowPos = VAdd(oldPos, slideVec);

					//また当たり判定？
					for (j = 0; j < hitWallNum; j++) {
						hitPoly = wallHitDim_[j];
						if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly->Position[0], hitPoly->Position[1], hitPoly->Position[2])) break;
					}
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
			if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly->Position[0], hitPoly->Position[1], hitPoly->Position[2])) {
				hitFlag = false;
				break;
			}
		}
	}

	// 壁に当たっていたら壁から押し出す処理を行う
	//当たったポリゴンの法線ベクトルの５倍をプレイヤーのポジションに足している
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

	//床との当たり判定
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

void CheckCollisionModel::checkCollision(Player& player, VECTOR moveVec, float playerHeight, bool isJump, float jumpVec)
{
	//プレイヤーから一定範囲の衝突判定をとる
	checkCollisionPersonalArea(player,moveVec);
	//衝突したオブジェクトが乗り越えることが出来るオブジェクトか判断する
	checkStepDifference(player,playerHeight);
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

	hitDim_.clear();
}

void CheckCollisionModel::checkStepDifference(Player& player, float playerHeight)
{
	objectHeightY = 0;
	isGoUpStep_ = false;

	//乗り越えられる段差か判断するため
	bool overHeight = false;

	//プレイヤーを基準にしたカプセルと当たったポリゴンの当たり判定を行い
	//プレイヤーの高さよりもポリゴンの高さが低かったら段差を上る処理を作る
	if (hitWallNum != 0) {
		for (int i = 0; i < hitWallNum; i++) {
			hitPoly = wallHitDim_[i];
			if (!HitCheck_Capsule_Triangle(VGet(nowPos.x, nowPos.y, nowPos.z), VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly->Position[0], hitPoly->Position[1], hitPoly->Position[2])) continue;

			//衝突したポリゴンの一番Y軸の高い頂点を見つけ出し
			//乗り越えられる段差以上だったらoverHeightをtrueにする
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

			//乗り越えることができる高さで一番高いY軸の値を見つける
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

	//代入
	if (objectHeightY > 0.0f) {
		nowPos.y = objectHeightY;
	}

	if (!overHeight) {
		player.isClim_ = false;
	}

}
