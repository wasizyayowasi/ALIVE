#include "Enemy.h"
#include "Player.h"

Enemy::Enemy():CharacterBase("data/player/player.mv1")
{
}

Enemy::~Enemy()
{
}

void Enemy::update(Player& player)
{
	pos_ = VAdd(pos_, VScale(VNorm(player.getPos()), 3.0f));
}

