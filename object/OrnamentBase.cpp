#include "OrnamentBase.h"
#include "../util/Model.h"

OrnamentBase::OrnamentBase(int handle, LoadObjectInfo objInfo) : ObjectBase(handle,objInfo)
{
	isCollCheck_ = true;
}

OrnamentBase::~OrnamentBase()
{
}

void OrnamentBase::Update(Player& player, const InputState& input)
{
}

void OrnamentBase::Draw()
{
	model_->Draw();
	//DrawSphere3D(pos_, 16, 31, 0xff0000, 0xff0000, true);
}