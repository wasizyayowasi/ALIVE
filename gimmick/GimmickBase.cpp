#include "GimmickBase.h"
#include "../util/Model.h"

GimmickBase::GimmickBase(const char* filename)
{
	model_ = std::make_shared<Model>(filename);
}

GimmickBase::GimmickBase(int handle)
{
	model_ = std::make_shared<Model>(handle);
}

GimmickBase::~GimmickBase()
{
}

void GimmickBase::update()
{
}

void GimmickBase::draw()
{
	model_->draw();
}
