#include "OrnamentBase.h"
#include "../util/Model.h"

OrnamentBase::OrnamentBase(const char* const filename):ObjectBase(filename)
{
	model_->setScale(scale_);
}

OrnamentBase::OrnamentBase(int handle) : ObjectBase(handle)
{
	model_->setScale(scale_);
}

OrnamentBase::~OrnamentBase()
{
}

void OrnamentBase::update(Player& player)
{
}

void OrnamentBase::draw()
{
	model_->draw();
}
