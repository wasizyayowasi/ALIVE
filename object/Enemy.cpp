#include "Enemy.h"
#include "Player.h"
#include "../util/Model.h"

Enemy::Enemy():CharacterBase("data/player/player.mv1")
{
}

Enemy::~Enemy()
{
}

void Enemy::stalkingUpdate()
{
	//pos_ = VAdd(pos_, VScale(VNorm(player->getPos()), 3.0f));
	//model_->setPos(pos_);
}

void Enemy::update()
{
	//stalkingUpdate();
}

