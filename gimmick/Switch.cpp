#include "Switch.h"
#include "../util/Model.h"
#include "../OBJECT/Player.h"

namespace {
	const VECTOR scale = { 0.5f,0.5f, 0.5f };
}

Switch::Switch(const char* const filename, LoadObjectInfo objInfo) :GimmickBase(filename,objInfo)
{
	model_->SetCollFrame();
	model_->SetAnimation(0, true, false);
}

Switch::Switch(int handle, LoadObjectInfo objInfo) :GimmickBase(handle,objInfo)
{
	model_->SetCollFrame();
	model_->SetAnimation(0, true, false);
}

Switch::~Switch()
{
}

//更新
void Switch::Update(Player& player)
{

	hit = false;

	//アニメーションの更新
	model_->Update();

	//衝突判定
	HitCollPlayer(player);

	isTransit_ = player.GetStatus().isTransit;

	//衝突結果の削除
	for (auto& result : hitDim_) {
		MV1CollResultPolyDimTerminate(result);
	}

	//衝突結果を保持する配列の削除
	hitDim_.erase(hitDim_.begin(), hitDim_.end());

}

void Switch::Draw()
{
	model_->Draw();
	if (hit) {
		DrawSphere3D(VAdd(pos_, VGet(0, 200, 0)), 50, 21, 0x00ffff, 0x00ff00, true);
	}
}

//衝突判定
void Switch::HitCollPlayer(Player& player)
{
	//プレイヤーの位置情報を元にしたカプセルとスイッチモデルの判定
	hitDim_.push_back(MV1CollCheck_Capsule(model_->GetModelHandle(), model_->GetColFrameIndex(), VAdd(player.GetStatus().pos, VGet(0.0f, 10.0f, 0.0f)), VAdd(player.GetStatus().pos, VGet(0.0f, 150.0f, 0.0f)), 20.0f));

	int hitNum = 0;
	//当たっている数を数える
	for (auto& result : hitDim_) {
		if (result.HitNum < 1) continue;
		hitNum++;
	}

	//当たっていなかったら
	//アニメーションを変更し終了
	if (hitNum < 1) {
		OffAnim();
		return;
	}

	//衝突判定の消去
	for (auto& hit : hitDim_) {
		MV1CollResultPolyDimTerminate(hit);
	}

	hitDim_.clear();

	hit = true;

	//アニメーションの変更
	OnAnim();
}

void Switch::HitColl(std::shared_ptr<ObjectBase> deadPerson)
{

	//持ち運び中だったら以降の処理を行わない
	if (isTransit_) {
		return;
	}

	//プレイヤーの位置情報を元にしたカプセルとスイッチモデルの判定
	hitDim_.push_back(MV1CollCheck_Capsule(deadPerson->GetModelPointer()->GetModelHandle(), deadPerson->GetModelPointer()->GetColFrameIndex(), pos_, VAdd(pos_, VGet(0.0f, 50.0f, 0.0f)), 50.0f));

	int hitNum = 0;
	//当たっている数を数える
	for (auto& result : hitDim_) {
		if (result.HitNum > 1) {
			hitNum++;
		}
	}

	//当たっていなかったら
	//アニメーションを変更し終了
	if (hitNum < 1) {
		OffAnim();
		return;
	}

	//衝突判定の消去
	for (auto& hit : hitDim_) {
		MV1CollResultPolyDimTerminate(hit);
	}

	hitDim_.clear();

	hit = true;

	//アニメーションの変更
	OnAnim();
}

//スイッチオンアニメーション
void Switch::OnAnim()
{
	model_->SetAnimation(1, false, false);
}

//スイッチオフアニメーション
void Switch::OffAnim()
{
	model_->SetAnimation(2, false, false);
}
