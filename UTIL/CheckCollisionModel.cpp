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
	//更新前のポジションを取得する
	oldPos = player.getPos();
	//更新後のポジションを取得する
	nowPos = VAdd(player.getPos(), moveVec);
	//モデルと球の当たり判定
	for (int i = 0; i < models.size();i++) {
		MV1RefreshCollInfo(models[i]->getModelHandle(), -1);
		hitDim_.push_back(MV1CollCheck_Sphere(models[i]->getModelHandle(), -1, oldPos, collition_radius + VSize(moveVec)));
	}
	

	//ベクトルの絶対値を取得し、動いたか動いていないかのフラグをとる
	if (fabs(moveVec.x) > 0.01f || fabs(moveVec.z) > 0.01f) {
		moveFlag = true;
	}
	else {
		moveFlag = false;
	}

	kabeNum = 0;
	yukaNum = 0;

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
					if (kabeNum < max_hit_coll) {
						kabe[kabeNum] = &result.Dim[i];
						kabeNum++;
					}

				}
			}
			else {
				//床ポリゴン情報の取得？
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
	//壁の処理
	if (kabeNum != 0) {
		hitFlag = false;
		//動いていたら
		if (moveFlag == true) {
			for (i = 0; i < kabeNum; i++) {
				poly = kabe[i];
				//プレイヤーを元にしたカプセルと壁ポリゴンの判定　　当たっていなかったらcontinue
				if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, poly->Position[0], poly->Position[1], poly->Position[2]) == FALSE) continue;

				hitFlag = true;

				VECTOR slideVec;
				//プレイヤーのベクトルとポリゴンの法線ベクトルの外積を取得
				slideVec = VCross(moveVec, poly->Normal);
				//プレイヤーのベクトルとポリゴンの法線ベクトルの外積とポリゴンの外積の法線ベクトルの外積を取得
				slideVec = VCross(poly->Normal, slideVec);
				//更新前のプレイヤーのポジションと上記の外積を取得
				nowPos = VAdd(oldPos, slideVec);

				//また当たり判定？
				for (j = 0; j < kabeNum; j++) {
					poly = kabe[j];
					if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE) break;
				}

				//当たっていなかったらフラグを折る
				if (j == kabeNum) {
					hitFlag = false;
					break;
				}
			}
		}
	}
	else {
		//一つも壁とのhit情報がなかった場合
		for (i = 0; i < kabeNum; i++) {
			poly = kabe[i];
			if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerHeight, 0.0f)), 20.0f, poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE) {
				hitFlag = false;
				break;
			}
		}

	}

	// 壁に当たっていたら壁から押し出す処理を行う
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

	//床との当たり判定
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
		hitDim_.erase(hitDim_.begin());
	}

	/*for (auto& result : hitDim_) {
		hitDim_.pop_back();
		hitDim_.erase(hitDim_.back());
	}*/
}
