#include "Switch.h"
#include "../util/Model.h"
#include "../OBJECT/Player.h"

namespace {
	//モデルのファイルパス
	const char* const switch_filename = "data/model/other/mv1/switch.mv1";
}

//コンストラクタ
Switch::Switch(LoadObjectInfo objInfo)
{
	//モデルクラスの初期化
	model_ = std::make_shared<Model>(switch_filename);
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);
	model_->SetRot(objInfo.rot);
	model_->SetCollFrame();

	//ポジションの初期化
	pos_ = objInfo.pos;

}

//デストラクタ
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

	isTransit_ = player.GetStatus().situation.isInTransit;

}

//描画
void Switch::Draw()
{
	model_->Draw();
//	DrawCapsule3D(pos_, VAdd(pos_, VGet(0.0f, 50.0f, 0.0f)), 50, 32, 0x00ff00, 0x00ff00, true);
//	DrawFormatString(0, 48, 0x448844, "%.2f,%.2f,%.2f", pos_.x, pos_.y, pos_.z);
}

void Switch::DeleteHitResult()
{
	//衝突結果の削除
	for (auto& result : hitDim_) {
		MV1CollResultPolyDimTerminate(result);
	}

	//衝突結果を保持する配列の削除
	hitDim_.clear();
}

//衝突判定
void Switch::HitCollPlayer(Player& player)
{

	VECTOR playerPos = player.GetStatus().pos;

	MV1RefreshCollInfo(model_->GetModelHandle(), model_->GetColFrameIndex());

	int model = model_->GetModelHandle();

	//プレイヤーの位置情報を元にしたカプセルとスイッチモデルの判定
	hitDim_.push_back(MV1CollCheck_Capsule(model_->GetModelHandle(), model_->GetColFrameIndex(), playerPos, VAdd(playerPos, VGet(0.0f, player.GetStatus().height, 0.0f)), 20.0f));
}

void Switch::HitColl(std::shared_ptr<ObjectBase> deadPerson)
{
	MV1RefreshCollInfo(deadPerson->GetModelPointer()->GetModelHandle(), deadPerson->GetModelPointer()->GetColFrameIndex());

	int temp = deadPerson->GetModelPointer()->GetColFrameIndex();

	//持ち運び中だったら以降の処理を行わない
	if (deadPerson->IsTransit()) {
		return;
	}

	//プレイヤーの位置情報を元にしたカプセルとスイッチモデルの判定
	hitDim_.push_back(MV1CollCheck_Capsule(deadPerson->GetModelPointer()->GetModelHandle(), deadPerson->GetModelPointer()->GetColFrameIndex(), pos_, VAdd(pos_, VGet(0.0f, 50.0f, 0.0f)), 50.0f));
}

//衝突判定結果の初期化
bool Switch::CollResult()
{
	int hitNum = 0;
	//当たっている数を数える
	for (auto& result : hitDim_) {
		if (result.HitNum > 0) {
			hitNum++;
		}
	}

	DeleteHitResult();

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

//衝突判定を行うモデルの追加
std::shared_ptr<Model> Switch::GetModelPointer()
{
	return model_;
}

//スイッチオンアニメーション
void Switch::OnAnim()
{
	model_->ChangeAnimation(0, false, false, 10);
	MV1SetMaterialDifColor(model_->GetModelHandle(), 1, GetColorF(0.0f, 0.0f, 1.0f, 1.0f));
}

//スイッチオフアニメーション
void Switch::OffAnim()
{
	model_->ChangeAnimation(1, false, false, 10);
	MV1SetMaterialDifColor(model_->GetModelHandle(), 1, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
}
