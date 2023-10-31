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
	Steelyard(const char* const filename, LoadObjectInfo objInfo);
	Steelyard(int handle, LoadObjectInfo objInfo);
	virtual ~Steelyard();

	void Update(Player& player)override;
	void Draw();

	//天秤モデルとプレイヤーの衝突判定を行う
	void HitCollPlayer(Player& player);
	//天秤モデルと死体との衝突判定を行う
	void HitColl(std::shared_ptr<ObjectBase> deadPerson);

	//リストに結果をまとめる
	void SummarizeList(std::list<MV1_COLL_RESULT_POLY_DIM> hitDim,std::list<HitState>& list);

	//最優先で処理を行うstateを取得する
	void PriorityState(std::list<HitState> list,HitState& hitState);

	//frameの移動
	void MoveFrame(HitState hitState,std::string string);

private:

	int time_ = 0;

	float moveDescent_ = 0.0f;
	float move_ = 0.0f;

	bool isStart_ = false;

	VECTOR InitialPosition_ = {};						//初期の高さを取得する
	VECTOR framePos_ = {};						//初期の高さを取得する

	HitState priorityStateLeft_;						//優先させるステータスを取得する
	HitState priorityStateRight_;						//優先させるステータスを取得する

	std::list<HitState> hitStateListLeft_;				//衝突した時のステータスを収集
	std::list<HitState> hitStateListRight_;				//衝突した時のステータスを収集
	std::list<MV1_COLL_RESULT_POLY_DIM> hitDimLeft_;	//衝突判定の結果を収集
	std::list<MV1_COLL_RESULT_POLY_DIM> hitDimRight_;	//衝突判定の結果を収集

};

