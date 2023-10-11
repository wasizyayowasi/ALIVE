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
	//更新前のポジションを取得する
	oldPos = player.GetStatus().pos;
	//更新後のポジションを取得する
	nowPos = VAdd(player.GetStatus().pos, moveVec);
	//モデルと球の当たり判定
	
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
	

	//ベクトルの絶対値を取得し、動いたか動いていないかのフラグをとる
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

	//前にとったモデルと球の当たり判定処理
	int i = 0;
	for (auto& result : hitDim_) {
		for (i = 0; i < result.hitDim.HitNum; i++) {
			//モデルの法線ベクトル
			if (result.hitDim.Dim[i].Normal.y < 0.000001f && result.hitDim.Dim[i].Normal.y > -0.000001f) {
				if (result.hitDim.Dim[i].Position[0].y > oldPos.y + 1.0f ||
					result.hitDim.Dim[i].Position[1].y > oldPos.y + 1.0f ||
					result.hitDim.Dim[i].Position[2].y > oldPos.y + 1.0f)
				{
					//壁ポリゴン情報の取得？
					if (hitWallNum < max_hit_coll) {
						wallHitDim_[hitWallNum].hitDim = &result.hitDim.Dim[i];
						hitWallNum++;
					}

				}
			}
			else {
				//床ポリゴン情報の取得？
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
	//壁の処理
	if (hitWallNum != 0) {
		hitFlag = false;
		//動いていたら
		if (moveFlag) {
			int i = 0,j = 0;
			for (i = 0; i < hitWallNum; i++) {
				auto hitPoly = wallHitDim_[i];
				//プレイヤーを元にしたカプセルと壁ポリゴンの判定　　当たっていなかったらcontinue
				if (!HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2])){
					continue;
				}

				hitFlag = true;
				if (!isGoUpStep_) {
					VECTOR slideVec;
					//プレイヤーのベクトルとポリゴンの法線ベクトルの外積を取得
					slideVec = VCross(moveVec, hitPoly.hitDim->Normal);
					//プレイヤーのベクトルとポリゴンの法線ベクトルの外積とポリゴンの外積の法線ベクトルの外積を取得
					slideVec = VCross(hitPoly.hitDim->Normal, slideVec);
					//更新前のプレイヤーのポジションと上記の外積を取得
					nowPos = VAdd(oldPos, slideVec);

					//また当たり判定？
					for (j = 0; j < hitWallNum; j++) {
						hitPoly = wallHitDim_[j];
						if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2])) {
							break;
						}
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
		int i = 0;
		//一つも壁とのhit情報がなかった場合
		for (i = 0; i < hitWallNum; i++) {
			auto hitPoly = wallHitDim_[i];
			if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2])) {
				hitFlag = false;
				break;
			}
		}
	}

	// 壁に当たっていたら壁から押し出す処理を行う
	//当たったポリゴンの法線ベクトルの５倍をプレイヤーのポジションに足している
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

	//床との当たり判定
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
	//プレイヤーから一定範囲の衝突判定をとる
	CheckCollisionPersonalArea(player,moveVec);

	CheckCollSpecificModel(player);
	//衝突したオブジェクトが乗り越えることが出来るオブジェクトか判断する
	CheckStepDifference(player,playerHeight);
	//取得した衝突結果から壁に当たった場合の処理
	CheckCollisionWall(moveVec,playerHeight);
	//取得した衝突結果から床に当たった場合の処理
	CheckCollisionFloor(player, moveVec,isJump,playerHeight);

	//ポジションのセット
	player.SetPos(nowPos);

	//衝突判定の消去
	for (auto& hit : hitDim_) {
		MV1CollResultPolyDimTerminate(hit.hitDim);
	}

	hitDim_.clear();
}

void CheckCollisionModel::CheckStepDifference(Player& player, float playerHeight)
{
	objectHeightY = 0;
	isGoUpStep_ = false;

	//乗り越えられる段差か判断するため
	bool overHeight = false;

	//プレイヤーを基準にしたカプセルと当たったポリゴンの当たり判定を行い
	//プレイヤーの高さよりもポリゴンの高さが低かったら段差を上る処理を作る
	if (hitWallNum != 0) {
		for (int i = 0; i < hitWallNum; i++) {
			auto hitPoly = wallHitDim_[i];
			if (!HitCheck_Capsule_Triangle(VGet(nowPos.x, nowPos.y, nowPos.z), VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2])) {
				continue;
			}

			//衝突したポリゴンの一番Y軸の高い頂点を見つけ出し
			//乗り越えられる段差以上だったらoverHeightをtrueにする
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

			//乗り越えることができる高さで一番高いY軸の値を見つける
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

	//代入
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
