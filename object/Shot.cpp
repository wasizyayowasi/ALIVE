#include "Shot.h"
#include "../util/Model.h"

Shot::Shot(int handle,VECTOR initPos, VECTOR moveVec)
{
	model_ = std::make_shared<Model>(handle);
	model_->SetScale(VGet(50.0f, 50.0f, 50.0f));
	pos_ = initPos;
	initialisePos_ = initPos;
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

	VECTOR distance = VSub(initialisePos_, pos_);
	float distanceSize = VSize(distance);

	if (distanceSize > 600.0f) {
		isEnable_ = false;
	}

	model_->Update();
}

void Shot::Draw()
{
	model_->Draw();
}
