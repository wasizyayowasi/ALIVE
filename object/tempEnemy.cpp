#include "tempEnemy.h"
#include "../util/Model.h"

tempEnemy::tempEnemy(const char* const filename):EnemyBase(filename)
{
	model_->SetScale(scale_);
	model_->SetPos(pos_);
	model_->SetRot(rot_);
}

tempEnemy::tempEnemy(int handle) : EnemyBase(handle)
{
	model_->SetScale(scale_);
	model_->SetPos(pos_);
	model_->SetRot(rot_);
}

tempEnemy::~tempEnemy()
{
}

void tempEnemy::Update(Player& player)
{
	pos_.x += 1.0f;
	model_->SetPos(pos_);

	model_->Update();
}

void tempEnemy::Draw()
{
	model_->Draw();
}
