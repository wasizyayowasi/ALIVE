#include "Switch.h"
#include "../util/Model.h"
#include "../OBJECT/Player.h"

namespace {
	const VECTOR scale = { 0.5f,0.5f, 0.5f };
}

Switch::Switch(const char* const filename, LoadObjectInfo objInfo) :GimmickBase(filename,objInfo)
{
	model_->SetScale(scale);
	model_->SetPos({ 2245,0,0 });
	model_->SetCollFrame();
	model_->SetAnimation(0, true, false);
}

Switch::Switch(int handle, LoadObjectInfo objInfo) :GimmickBase(handle,objInfo)
{
	model_->SetScale(scale);
	model_->SetPos({ 2245,0,0 });
	model_->SetCollFrame();
	model_->SetAnimation(0, true, false);
}

Switch::~Switch()
{
}

//更新
void Switch::Update(Player& player)
{
	//アニメーションの更新
	model_->Update();

	//衝突判定
	HitColl(player);

	//衝突結果の削除
	for (auto& result : hitDim_) {
		MV1CollResultPolyDimTerminate(result);
	}

	//衝突結果を保持する配列の削除
	hitDim_.erase(hitDim_.begin(), hitDim_.end());

}

//衝突判定
void Switch::HitColl(Player& player)
{
	//プレイヤーの位置情報を元にしたカプセルとスイッチモデルの判定
	hitDim_.push_back(MV1CollCheck_Capsule(model_->GetModelHandle(), model_->GetColFrameIndex(), VAdd(player.GetStatus().pos, VGet(0.0f, 10.0f, 0.0f)), VAdd(player.GetStatus().pos, VGet(0.0f, 150.0f, 0.0f)), 20.0f));

	//死体の位置情報を元にしたカプセルとスイッチモデルの判定
	/*for (auto& person : player.getDeadPerson()) {
		hitDim_.push_back(MV1CollCheck_Capsule(model_->getModelHandle(), model_->getColFrameIndex(), VAdd(person->getPos(), VGet(0.0f, 0.0f, 0.0f)), person->getPos(), 20.0f));
	}*/

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
