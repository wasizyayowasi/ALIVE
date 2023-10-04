#include "ObjectBase.h"
#include "../util/Model.h"
#include "../util/InputState.h"

ObjectBase::ObjectBase(const char* const filename)
{
	model_ = std::make_shared<Model>(filename);
	model_->setCollFrame();
	isEnable_ = true;

}

ObjectBase::ObjectBase(int handle)
{
	model_ = std::make_shared<Model>(handle);
	
	isEnable_ = true;
}

ObjectBase::~ObjectBase()
{
}

void ObjectBase::update()
{
	model_->update();
}

void ObjectBase::draw()
{
	model_->draw();
}
