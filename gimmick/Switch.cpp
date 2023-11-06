#include "Switch.h"
#include "../util/Model.h"
#include "../OBJECT/Player.h"

namespace {
	const VECTOR scale = { 0.5f,0.5f, 0.5f };
}

Switch::Switch(const char* const filename, LoadObjectInfo objInfo)
{
	model_ = std::make_shared<Model>(filename);
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);
	model_->SetRot(objInfo.rot);
	model_->SetCollFrame();

	pos_ = model_->GetPos();

}

Switch::Switch(int handle, LoadObjectInfo objInfo)
{
	model_ = std::make_shared<Model>(handle);
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);
	model_->SetRot(objInfo.rot);
	model_->SetCollFrame();

	pos_ = model_->GetPos();
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
	HitCollPlayer(player);

	isTransit_ = player.GetStatus().isTransit;

}

void Switch::Draw()
{
	model_->Draw();
//	DrawFormatString(0, 48, 0x448844, "%.2f,%.2f,%.2f", pos_.x, pos_.y, pos_.z);
}

//衝突判定
void Switch::HitCollPlayer(Player& player)
{
	//プレイヤーの位置情報を元にしたカプセルとスイッチモデルの判定
	hitDim_.push_back(MV1CollCheck_Capsule(model_->GetModelHandle(), model_->GetColFrameIndex(), VAdd(player.GetStatus().pos, VGet(0.0f, 10.0f, 0.0f)), VAdd(player.GetStatus().pos, VGet(0.0f, 150.0f, 0.0f)), 20.0f));
}

void Switch::HitColl(std::shared_ptr<ObjectBase> deadPerson)
{
	MV1RefreshCollInfo(deadPerson->GetModelPointer()->GetModelHandle(), deadPerson->GetModelPointer()->GetColFrameIndex());

	//持ち運び中だったら以降の処理を行わない
	if (deadPerson->IsTransit()) {
		return;
	}

	//プレイヤーの位置情報を元にしたカプセルとスイッチモデルの判定
	hitDim_.push_back(MV1CollCheck_Capsule(deadPerson->GetModelPointer()->GetModelHandle(), deadPerson->GetModelPointer()->GetColFrameIndex(), pos_, VAdd(pos_, VGet(0.0f, 50.0f, 0.0f)), 50.0f));
}

bool Switch::CollResult()
{
	int hitNum = 0;
	//当たっている数を数える
	for (auto& result : hitDim_) {
		if (result.HitNum > 1) {
			hitNum++;
		}
	}

	//衝突結果の削除
	for (auto& result : hitDim_) {
		MV1CollResultPolyDimTerminate(result);
	}

	//衝突結果を保持する配列の削除
	hitDim_.clear();

	//当たっていなかったら
	//アニメーションを変更し終了
	if (hitNum < 1) {
		OffAnim();
		return false;
	}

	//アニメーションの変更
	OnAnim();

	return true;
}

//スイッチオンアニメーション
void Switch::OnAnim()
{
	MV1SetMaterialDifColor(model_->GetModelHandle(), 1, GetColorF(0.0f, 0.0f, 1.0f, 1.0f));
}

//スイッチオフアニメーション
void Switch::OffAnim()
{
	MV1SetMaterialDifColor(model_->GetModelHandle(), 1, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
}
