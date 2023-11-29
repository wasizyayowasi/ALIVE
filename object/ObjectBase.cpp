#include "ObjectBase.h"
#include "Player.h"
#include "../util/Model.h"
#include "../util/InputState.h"

ObjectBase::ObjectBase(const char* const filename, LoadObjectInfo objInfo)
{
	model_ = std::make_shared<Model>(filename);
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);
	model_->SetRot(objInfo.rot);
	model_->SetCollFrame("Coll");
	pos_ = model_->GetPos();
	isEnable_ = true;

}

ObjectBase::ObjectBase(int handle, LoadObjectInfo objInfo)
{
	model_ = std::make_shared<Model>(handle);
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);
	model_->SetRot(objInfo.rot);
	model_->SetCollFrame("Coll");
	pos_ = model_->GetPos();
	scale_ = objInfo.scale;
	isEnable_ = true;
}

ObjectBase::~ObjectBase()
{
}

void ObjectBase::Update(Player& player, const InputState& input)
{
	model_->Update();
}

void ObjectBase::Draw()
{
	model_->Draw();
}

void ObjectBase::HitColl(std::shared_ptr<ObjectBase> pointer)
{
}

std::shared_ptr<Model> ObjectBase::AddCollModel()
{
	return nullptr;
}
