#pragma once
#include "GimmickBase.h"
#include <memory>
#include <list>
#include <string>

class Model;

enum class HitState {
	top,
	bottom,
	noHit,
};

class Steelyard : public GimmickBase
{
public:
	Steelyard(int handle, LoadObjectInfo objInfo);
	virtual ~Steelyard();

	void Update(Player& player, const InputState& input)override;
	void Draw();

	//天秤モデルとプレイヤーの衝突判定を行う
	void HitCollPlayer(Player& player);
	//天秤モデルと死体との衝突判定を行う
	void HitColl(std::shared_ptr<ObjectBase> deadPerson);

	//リストに結果をまとめる
	void SummarizeList(MV1_COLL_RESULT_POLY_DIM hitDim,std::list<HitState>& list,bool& isStart);

	//最優先で処理を行うstateを取得する
	void PriorityState(std::list<HitState> list,HitState& hitState);

	//frameの移動
	void MoveFrame(HitState LeftHitState, HitState RightHitState, float& moveDescent, bool isStart);
private:

	int time_ = 0; 

	float moveDescent_ = 0.0f;
	float playerHeight_ = 0.0f;

	bool isStart_ = false;

	VECTOR InitialPosition_ = {};						//初期の高さを取得する
	VECTOR framePos_ = {};								//初期の高さを取得する
	VECTOR playerPos_ = {};								//初期の高さを取得する

	HitState priorityStateLeft_;						//優先させるステータスを取得する
	HitState priorityStateRight_;						//優先させるステータスを取得する
	HitState moveState_;

	MV1_COLL_RESULT_POLY_DIM hitDimRight_;				//衝突判定の結果を収集
	MV1_COLL_RESULT_POLY_DIM hitDimLeft_;				//衝突判定の結果を収集
	std::list<HitState> hitStateListLeft_;				//衝突した時のステータスを収集
	std::list<HitState> hitStateListRight_;				//衝突した時のステータスを収集

};

