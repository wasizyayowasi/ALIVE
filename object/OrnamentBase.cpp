#include "OrnamentBase.h"
#include "../util/Model.h"

OrnamentBase::OrnamentBase(const char* const filename, LoadObjectInfo objInfo):ObjectBase(filename,objInfo)
{
	model_->SetScale(objInfo.scale);
}

OrnamentBase::OrnamentBase(int handle, LoadObjectInfo objInfo) : ObjectBase(handle,objInfo)
{
}

OrnamentBase::~OrnamentBase()
{
}

void OrnamentBase::Update(Player& player)
{
}

void OrnamentBase::Draw()
{
	model_->Draw();
}

void OrnamentBase::HitColl(std::shared_ptr<ObjectBase> pointer)
{
}
