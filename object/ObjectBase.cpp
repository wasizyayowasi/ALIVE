#include "ObjectBase.h"

#include "Player.h"

#include "../util/Model.h"

//コンストラクタ
ObjectBase::ObjectBase(const int handle, const Material materialType, const LoadObjectInfo objInfo)
{
	//ポジション
	pos_ = objInfo.pos;

	//拡縮
	scale_ = objInfo.scale;

	//回転
	rot_ = objInfo.rot;

	//モデルの設定
	model_ = std::make_shared<Model>(handle,materialType);
	model_->SetPos(pos_);
	model_->SetRot(rot_);
	model_->SetScale(scale_);

	//存在しているフラグ
	isEnable_ = true;
}

//デストラクタ
ObjectBase::~ObjectBase()
{
}

//更新
void ObjectBase::Update(Player& player)
{
	model_->Update();
}

//描画
void ObjectBase::Draw()
{
	model_->Draw();
}

void ObjectBase::UpdateForCorpse(std::shared_ptr<ObjectBase>& pointer)
{
}

//モデルのスマートポインタを取得する
std::shared_ptr<Model> ObjectBase::GetModelPointer()const
{
	return model_;
}

//衝突判定を行うモデルを追加する
std::shared_ptr<Model> ObjectBase::AddCollModel() const
{
	return nullptr;
}