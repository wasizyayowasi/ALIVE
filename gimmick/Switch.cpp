#include "Switch.h"
#include "../util/Model.h"

namespace {
	const VECTOR scale = { 0.5f,0.5f, 0.5f };
}

Switch::Switch() :GimmickBase("data/model/switch.mv1")
{
	model_->setScale(scale);
	model_->setPos({ 2245,0,0 });
}

Switch::~Switch()
{
}

void Switch::update()
{

}

void Switch::draw()
{

}
