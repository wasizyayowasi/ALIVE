#include "tempEnemy.h"
#include "../util/Model.h"

tempEnemy::tempEnemy(const char* const filename):EnemyBase(filename)
{
	model_->setScale(scale_);
	model_->setPos(pos_);
	model_->setRot(rot_);
}

tempEnemy::tempEnemy(int handle) : EnemyBase(handle)
{
	model_->setScale(scale_);
	model_->setPos(pos_);
	model_->setRot(rot_);
}

tempEnemy::~tempEnemy()
{
}

void tempEnemy::update(Player& player)
{
	pos_.x += 1.0f;
	model_->setPos(pos_);

	model_->update();
}

void tempEnemy::draw()
{
	model_->draw();
}
