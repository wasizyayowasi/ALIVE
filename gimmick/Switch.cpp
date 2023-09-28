#include "Switch.h"
#include "../util/Model.h"
#include "../OBJECT/Player.h"

namespace {
	const VECTOR scale = { 0.5f,0.5f, 0.5f };
}

Switch::Switch() :GimmickBase("data/model/switch.mv1")
{
	model_->setScale(scale);
	model_->setPos({ 2245,0,0 });
	model_->setCollFrame();
	model_->setAnimation(0, true, false);
}

Switch::~Switch()
{
}

//更新
void Switch::update(Player& player)
{
	//アニメーションの更新
	model_->update();

	//衝突判定
	hitColl(player);

	//衝突結果の削除
	for (auto& result : hitDim_) {
		MV1CollResultPolyDimTerminate(result);
	}

	//衝突結果を保持する配列の削除
	hitDim_.erase(hitDim_.begin(), hitDim_.end());

}

VECTOR Switch::deadPersonCapsulePosAdjustment(std::shared_ptr<Model> person)
{
	VECTOR rot = person->getRot();
	rot = VNorm(rot);
	rot = VScale(VNorm(rot),-100.0f);
	rot.y = 20.0f;

	return rot;

}

//衝突判定
void Switch::hitColl(Player& player)
{
	//プレイヤーの位置情報を元にしたカプセルとスイッチモデルの判定
	hitDim_.push_back(MV1CollCheck_Capsule(model_->getModelHandle(), model_->getColFrameIndex(), VAdd(player.getPos(), VGet(0.0f, 10.0f, 0.0f)), VAdd(player.getPos(), VGet(0.0f, 150.0f, 0.0f)), 20.0f));

	//死体の位置情報を元にしたカプセルとスイッチモデルの判定
	for (auto& person : player.getDeadPerson()) {
		VECTOR rot = deadPersonCapsulePosAdjustment(person);
		hitDim_.push_back(MV1CollCheck_Capsule(model_->getModelHandle(), model_->getColFrameIndex(), VAdd(person->getPos(), VGet(0.0f, 0.0f, 0.0f)), VAdd(person->getPos(), rot), 20.0f));
	}

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
	model_->setAnimation(1, false, false);
}

//スイッチオフアニメーション
void Switch::OffAnim()
{
	model_->setAnimation(2, false, false);
}
