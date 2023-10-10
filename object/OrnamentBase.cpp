#include "OrnamentBase.h"
#include "../util/Model.h"

OrnamentBase::OrnamentBase(const char* const filename):ObjectBase(filename)
{
	model_->SetScale(scale_);
}

OrnamentBase::OrnamentBase(int handle) : ObjectBase(handle)
{
	model_->SetScale(scale_);
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
