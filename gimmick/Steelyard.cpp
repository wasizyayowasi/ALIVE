#include "Steelyard.h"
#include "../util/Model.h"

namespace {
	const VECTOR scale = { 0.5f,0.5f, 0.5f };
}

Steelyard::Steelyard():GimmickBase("data/model/steelyard.mv1")
{
	model_->setScale(scale);
	model_->setPos({ 1845,0,0 });
	model_->setCollFrame();
}

Steelyard::~Steelyard()
{
}

void Steelyard::update(Player& player)
{
}

void Steelyard::draw()
{
}
