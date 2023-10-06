#include "Steelyard.h"
#include "../util/Model.h"

namespace {
	const VECTOR scale = { 0.5f,0.5f, 0.5f };
}

Steelyard::Steelyard(const char* const filename):GimmickBase(filename)
{
	model_->setScale(scale);
	model_->setPos({ 1845,0,0 });
	model_->setCollFrame();
}

Steelyard::Steelyard(int handle):GimmickBase(handle)
{
}

Steelyard::~Steelyard()
{
}

void Steelyard::update(Player& player)
{
}

void Steelyard::draw()
{
	model_->draw();
}
