#include "Shot.h"
#include "../util/Model.h"

Shot::Shot(int handle,VECTOR initPos, VECTOR moveVec)
{
	model_ = std::make_shared<Model>(handle);
	pos_ = initPos;
	moveVec_ = moveVec;
	isEnable_ = true;
}

Shot::~Shot()
{
}

void Shot::Update()
{
	pos_ = VAdd(pos_, moveVec_);

	model_->SetPos(pos_);

	model_->Update();
}

void Shot::Draw()
{
	model_->Draw();
}
