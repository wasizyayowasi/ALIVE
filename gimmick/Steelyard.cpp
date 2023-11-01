#include "Steelyard.h"
#include "../util/Model.h"
#include "../object/Player.h"

namespace {
	const VECTOR scale = { 0.5f,0.5f, 0.5f };

	constexpr float move_speed = 0.02f;

}

Steelyard::Steelyard(const char* const filename, LoadObjectInfo objInfo):GimmickBase(filename,objInfo)
{
	InitialPosition_ = pos_;
	isCollCheck_ = true;
}

Steelyard::Steelyard(int handle, LoadObjectInfo objInfo):GimmickBase(handle,objInfo)
{
	//左
	InitialPosition_ = pos_;
	isCollCheck_ = true;
}

Steelyard::~Steelyard()
{
}

void Steelyard::Update(Player& player)
{
	playerPos_ = player.GetStatus().pos;
	playerHeight_ = player.GetStatus().height;

	HitCollPlayer(player);

	priorityStateLeft_ = HitState::noHit;
	priorityStateRight_ = HitState::noHit;

	isTransit_ = player.GetStatus().isTransit;

	//優先ステータスの取得
	PriorityState(hitStateListLeft_, priorityStateLeft_);
	PriorityState(hitStateListRight_, priorityStateRight_);

	if (priorityStateLeft_ == HitState::bottom && priorityStateRight_ == HitState::noHit) {
		moveState_ = HitState::bottom;
	}
	else if (priorityStateLeft_ == HitState::noHit && priorityStateRight_ == HitState::bottom) {
		moveState_ = HitState::top;
	}
	else if (priorityStateLeft_ != priorityStateRight_) {
		moveState_ = HitState::noHit;
	}

	//フレームを動かす
	MoveFrame(priorityStateLeft_, priorityStateRight_, moveDescent_, isStart_);

	model_->SetPos(pos_);

	hitStateListLeft_.clear();
	hitStateListRight_.clear();

	//isStart_ = false;
}

void Steelyard::Draw()
{
	model_->Draw();
}

void Steelyard::HitCollPlayer(Player& player)
{
	//指定のフレーム番号を取得する
	int leftFrameNo = MV1SearchFrame(model_->GetModelHandle(), "left");
	int RightFrameNo = MV1SearchFrame(model_->GetModelHandle(), "right");

	//コリジョン情報の更新
	MV1RefreshCollInfo(model_->GetModelHandle(), leftFrameNo);
	MV1RefreshCollInfo(model_->GetModelHandle(), RightFrameNo);

	//天秤の左右別で衝突判定を行う
	hitDimLeft_ = MV1CollCheck_Capsule(model_->GetModelHandle(), leftFrameNo, VAdd(player.GetStatus().pos,VGet(0,10,0)), VAdd(player.GetStatus().pos, VGet(0.0f, player.GetStatus().height, 0.0f)), 20.0f);
	hitDimRight_ = MV1CollCheck_Capsule(model_->GetModelHandle(), RightFrameNo, VAdd(player.GetStatus().pos,VGet(0,10,0)), VAdd(player.GetStatus().pos, VGet(0.0f, player.GetStatus().height, 0.0f)), 20.0f);

	//リストにまとめる
	SummarizeList(hitDimLeft_, hitStateListLeft_,isStart_);
	SummarizeList(hitDimRight_, hitStateListRight_, isStart_);

	//衝突判定の消去
	MV1CollResultPolyDimTerminate(hitDimLeft_);
	MV1CollResultPolyDimTerminate(hitDimRight_);
}

void Steelyard::HitColl(std::shared_ptr<ObjectBase> deadPerson)
{

	//持ち運び中だったら以降の処理を行わない
	if (isTransit_) {
		return;
	}

	//指定のフレーム番号を取得する
	int leftFrameNo = MV1SearchFrame(model_->GetModelHandle(), "left");
	int RightFrameNo = MV1SearchFrame(model_->GetModelHandle(), "right");

	//コリジョン情報の更新
	MV1RefreshCollInfo(model_->GetModelHandle(), leftFrameNo);
	MV1RefreshCollInfo(model_->GetModelHandle(), RightFrameNo);

	VECTOR deadPersonPos = deadPerson->GetModelPointer()->GetPos();

	//天秤の左右別で衝突判定を行う
	hitDimLeft_ = MV1CollCheck_Capsule(model_->GetModelHandle(), leftFrameNo, VAdd(deadPersonPos, VGet(0.0f, 10.0f, 0.0f)), VAdd(deadPersonPos, VGet(0.0f, playerHeight_, 0.0f)), 20.0f);
	hitDimRight_ = MV1CollCheck_Capsule(model_->GetModelHandle(), RightFrameNo, VAdd(deadPersonPos, VGet(0.0f, 10.0f, 0.0f)), VAdd(deadPersonPos, VGet(0.0f, playerHeight_ + 2.0f, 0.0f)), 20.0f);
	
	//リストにまとめる
	SummarizeList(hitDimLeft_, hitStateListLeft_, isStart_);
	SummarizeList(hitDimRight_, hitStateListRight_, isStart_);

	//衝突判定の消去
	MV1CollResultPolyDimTerminate(hitDimLeft_);
	MV1CollResultPolyDimTerminate(hitDimRight_);
}

void Steelyard::SummarizeList(MV1_COLL_RESULT_POLY_DIM hitDim, std::list<HitState>& list, bool& isStart)
{

	//法線を元に上に乗っているか
	//下からぶつかっているか
	//当たっていないのかを取得する
	if (hitDim.HitNum > 0) {
		if (hitDim.Dim->Normal.y > 0.99999f) {
			list.push_back(HitState::bottom);
			isStart = true;
		}
		else if (hitDim.Dim->Normal.y < -0.99999f) {
			list.push_back(HitState::top);
			isStart = true;
		}
	}
	else {
		list.push_back(HitState::noHit);
	}
}

//最優先で処理を行うstateを取得する
void Steelyard::PriorityState(std::list<HitState> list, HitState& hitState)
{
	//enum classの並び順で優先順位を表している
	for (auto& state : list) {
		if (hitState > state) {
			hitState = state;
		}
	}
}

void Steelyard::MoveFrame(HitState LeftHitState, HitState RightHitState, float& moveDescent, bool isStart)
{

	//指定フレームの番号を取得する
	int leftFrameNo = MV1SearchFrame(model_->GetModelHandle(), "left");
	int rightFrameNo = MV1SearchFrame(model_->GetModelHandle(), "right");
	//モデルからフレームの座標を取得する
	VECTOR leftFramePos = MV1GetFramePosition(model_->GetModelHandle(), leftFrameNo);
	VECTOR rightFramePos = MV1GetFramePosition(model_->GetModelHandle(), rightFrameNo);

	//下降制限
	float descendingLimit = InitialPosition_.y - 50.0f;
	//上昇制限
	float upwardRevisionLimit = InitialPosition_.y + 50.0f;

	if (isStart) {
		//if (HitState::bottom == LeftHitState && HitState::noHit == RightHitState) {
		if (moveState_ == HitState::bottom) {
			if (leftFramePos.y >= descendingLimit) {
				moveDescent -= move_speed;
			}
		}
		//else if(HitState::noHit == LeftHitState && HitState::bottom == RightHitState) {
		else if(moveState_ == HitState::top) {
			if (leftFramePos.y <= upwardRevisionLimit) {
				moveDescent += move_speed;
			}
		}
	}

	MATRIX mtx = {};

	//左
	mtx = MGetTranslate(VGet(0, moveDescent, 0));
	MV1SetFrameUserLocalMatrix(model_->GetModelHandle(), leftFrameNo, mtx);
	//右
	mtx = MGetTranslate(VGet(0, -moveDescent, 0));
	MV1SetFrameUserLocalMatrix(model_->GetModelHandle(), rightFrameNo, mtx);
}
