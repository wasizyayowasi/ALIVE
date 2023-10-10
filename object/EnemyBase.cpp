#include "EnemyBase.h"
#include "Player.h"
#include "../util/Model.h"

EnemyBase::EnemyBase(const char* fileName):CharacterBase(fileName)
{
}

EnemyBase::EnemyBase(int handle) : CharacterBase(handle)
{
}

void EnemyBase::StalkingUpdate()
{
	//pos_ = VAdd(pos_, VScale(VNorm(player->getPos()), 3.0f));
	//model_->setPos(pos_);
}

void EnemyBase::Update(Player& player)
{
	//stalkingUpdate();
}

