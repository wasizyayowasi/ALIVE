#include "OrnamentBase.h"
#include "../util/Model.h"

OrnamentBase::OrnamentBase(const char* const filename, LoadObjectInfo objInfo):ObjectBase(filename,objInfo)
{
	model_->SetScale(objInfo.scale);
}

OrnamentBase::OrnamentBase(int handle, LoadObjectInfo objInfo) : ObjectBase(handle,objInfo)
{
	isCollCheck_ = true;
}

OrnamentBase::~OrnamentBase()
{
}

void OrnamentBase::temp(char aiu)
{
}

void OrnamentBase::Update(Player& player, const InputState& input)
{
}

void OrnamentBase::Draw()
{
	model_->Draw();
}

void OrnamentBase::HitColl(std::shared_ptr<ObjectBase> pointer)
{
}
