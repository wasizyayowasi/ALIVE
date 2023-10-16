#include "OrnamentBase.h"
#include "../util/Model.h"

OrnamentBase::OrnamentBase(const char* const filename, LoadObjectInfo objInfo):ObjectBase(filename,objInfo)
{
	model_->SetScale(objInfo.scale);
}

OrnamentBase::OrnamentBase(int handle, LoadObjectInfo objInfo) : ObjectBase(handle,objInfo)
{
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);
	model_->SetRot(objInfo.rot);
}

OrnamentBase::~OrnamentBase()
{
}

void OrnamentBase::Update(Player& player)
{
}

void OrnamentBase::Draw()
{
	DrawSphere3D(model_->GetPos(), 50, 32, 0xff0000, 0xff0000, true);
	model_->Draw();
}
