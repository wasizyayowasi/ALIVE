#include "ObjectBase.h"
#include "Player.h"
#include "../util/Model.h"
#include "../util/InputState.h"

ObjectBase::ObjectBase(const char* const filename)
{
	model_ = std::make_shared<Model>(filename);
	model_->SetCollFrame();
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

void ObjectBase::Update(Player& player)
{
	model_->Update();
}

void ObjectBase::Draw()
{
	model_->Draw();
}
