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
	//短縮化
	auto playerState = player->GetStatus();

	//更新前のポジションを取得する
	oldPos = playerState.pos;
	//更新後のポジションを取得する
	nowPos = VAdd(playerState.pos, playerState.moveVec);
	//モデルと球の当たり判定
	
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
	

	//ベクトルの絶対値を取得し、動いたか動いていないかのフラグをとる
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

void CheckCollisionModel::CheckCollisionWall(std::shared_ptr<Player> player)
{
	//短縮化
	auto playerState = player->GetStatus();

	//壁の処理
	if (hitWallNum != 0) {
		hitFlag = false;
		//動いていたら
		if (moveFlag) {
			int i = 0,j = 0;
			for (i = 0; i < hitWallNum; i++) {
				auto hitPoly = wallHitDim_[i];
				//プレイヤーを元にしたカプセルと壁ポリゴンの判定　　当たっていなかったらcontinue
				if (!HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerState.height, 0.0f)), 20.0f, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2])){
					continue;
				}

				hitFlag = true;
				if (!isGoUpStep_) {
					VECTOR slideVec;
					//プレイヤーのベクトルとポリゴンの法線ベクトルの外積を取得
					slideVec = VCross(playerState.moveVec, hitPoly.hitDim->Normal);
					//プレイヤーのベクトルとポリゴンの法線ベクトルの外積とポリゴンの外積の法線ベクトルの外積を取得
					slideVec = VCross(hitPoly.hitDim->Normal, slideVec);
					//更新前のプレイヤーのポジションと上記の外積を取得
					nowPos = VAdd(oldPos, slideVec);

					//また当たり判定？
					for (j = 0; j < hitWallNum; j++) {
						hitPoly = wallHitDim_[j];
						if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerState.height, 0.0f)), 20.0f, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2])) {
							player->SetMoveVec(VGet(0, 0, 0));
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
			if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerState.height, 0.0f)), 20.0f, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2])) {
				hitFlag = false;
				break;
			}
		}
	}


	//TODO：以下の処理を別の関数にする
	// 壁に当たっていたら壁から押し出す処理を行う
	//当たったポリゴンの法線ベクトルの５倍をプレイヤーのポジションに足している
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
	//短縮化
	auto playerState = player->GetStatus();

	float jumpVec = playerState.jump.jumpVec;
	bool isJump = playerState.jump.isJump;

	//床との当たり判定
	if (hitFloorNum != 0) {
		//ジャンプ中かつベクトルがマイナスでなければ
		if (isJump && jumpVec > 0.0f) {
			float minY = 0.0f;
			hitFlag = false;
			for (int i = 0; i < hitFloorNum; i++) {
				//データのコピー
				auto hitPoly = floorHitDim_[i];
				//衝突判定結果
				hitLineResult_.push_back(HitCheck_Line_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, playerState.height, 0.0f)), hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));

				//衝突判定結果で衝突してなかったら以降の処理を行わない
				if (hitLineResult_.back().HitFlag == false) {
					continue;
				}
				//衝突判定結果で衝突してなかったら以降の処理を行わない
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

	//プレイヤーから一定範囲の衝突判定をとる
	CheckCollisionPersonalArea(player, objManager);

	CheckCollSpecificModel(player, objManager);
	//衝突したオブジェクトが乗り越えることが出来るオブジェクトか判断する
	CheckStepDifference(player);
	//取得した衝突結果から壁に当たった場合の処理
	CheckCollisionWall(player);
	//取得した衝突結果から床に当たった場合の処理
	CheckCollisionFloor(player);

	FindThePolygonBelowThePlayer(player, objManager);

	//ポジションのセット
	player->SetPos(nowPos);

	//衝突判定の消去
	for (auto& hit : hitDim_) {
		MV1CollResultPolyDimTerminate(hit.hitDim);
	}

	hitDim_.clear();
}

void CheckCollisionModel::CheckStepDifference(std::shared_ptr<Player> player)
{
	//短縮化
	auto playerState = player->GetStatus();

	objectHeightY = 0;
	isGoUpStep_ = false;

	//乗り越えられる段差か判断するため
	bool overHeight = false;

	//プレイヤーを基準にしたカプセルと当たったポリゴンの当たり判定を行い
	//プレイヤーの高さよりもポリゴンの高さが低かったら段差を上る処理を作る
	if (hitWallNum != 0) {
		for (int i = 0; i < hitWallNum; i++) {
			auto hitPoly = wallHitDim_[i];
			if (!HitCheck_Capsule_Triangle(VGet(nowPos.x, nowPos.y, nowPos.z), VAdd(nowPos, VGet(0.0f, playerState.height, 0.0f)), 20.0f, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2])) {
				continue;
			}

			//衝突したポリゴンの一番Y軸の高い頂点を見つけ出し
			//乗り越えられる段差以上だったらoverHeightをtrueにする
			for (int i = 0; i < 3; i++) {
				if (nowPos.y + 60 < hitPoly.hitDim->Position[i].y) {
					overHeight = true;
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
}

void CheckCollisionModel::FindThePolygonBelowThePlayer(std::shared_ptr<Player> player, std::shared_ptr<ObjectManager> objManager)
{
	std::unordered_map<Material, std::list<MV1_COLL_RESULT_POLY>> hitLine;

	//オブジェクトのポリゴンを取得する
	for (auto& model : objManager->GetAllCheckCollModel()) {
		//プレイヤーが何かを持ち運んでいる場合
		//持ち運んでいるオブジェクトのポリゴンを取得しないようにしている
		if (player->GetStatus().situation.isInTransit) {
			if (player->GetDeadPersonModelPointer()->GetModelPointer() == model) {
				continue;
			}
		}

		//オブジェクトとプレイヤーの高さの差を取得する
		float distance = nowPos.y - model->GetPos().y;
		distance = (std::max)(distance, -distance);

		//高さの差が1000以上あったらcontinue
		//if (distance > 1000.0f) {
		//	continue;
		//}

		//モデルと線の当たり判定を取る
		MV1RefreshCollInfo(model->GetModelHandle(), model->GetColFrameIndex());
		VECTOR playerHeadPos = VGet(nowPos.x, nowPos.y + player->GetStatus().height, nowPos.z);
		VECTOR lowPos = VGet(nowPos.x, nowPos.y - distance, nowPos.z);
		hitLine[model->GetMaterialType()].push_back(MV1CollCheck_Line(model->GetModelHandle(), model->GetColFrameIndex(), playerHeadPos, lowPos));
	}

	float nearPosY = 5000.0f;
	float resultY = 0.0f;
	float distanceY = 0.0f;
	Material materialType = Material::max;

	//当たり判定の結果から一番近いポリゴンのY座標を取得する
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

	//一番近いY座標を丸影のY座標として使う
	//当たり判定の結果何とも当たっていなかった場合
	//Y座標を0にする
	player->SetRoundShadowHeightAndMaterial(resultY,materialType);

	hitLine.clear();
}

void CheckCollisionModel::CheckCollSpecificModel(std::shared_ptr<Player> player, std::shared_ptr<ObjectManager> objManager)
{

	//短縮化
	auto playerState = player->GetStatus();

	//プレイヤーが現状別の死体を持ち運んでいたら取得しない
	if (player->GetStatus().situation.isInTransit) {
		return;
	}

	//持ち運ぶ死体を取得する
	for (auto& obj : objManager->GetSpecificObject(ObjectType::DeadPerson)) {
		for (auto& hit : hitDim_)
		{
			//衝突結果が死体以外だったらcontinue
			if (hit.model != obj->GetModelPointer()) {
				continue;
			}

			//死体のモデルとプレイヤーの座標を基準に作成されたカプセルとの衝突判定
			auto result = MV1CollCheck_Capsule(hit.model->GetModelHandle(), hit.model->GetColFrameIndex(), playerState.pos, VAdd(playerState.pos, VGet(0, playerState.height, 0)),30);

			//上記の衝突判定の結果が1つでもあればプレイヤーに
			//その死体のポインターと持ち運べるフラグを送る
			if (result.HitNum > 0) {
				player->SetCarryInfo(true,obj);
			}

			MV1CollResultPolyDimTerminate(result);

		}
	}

}
