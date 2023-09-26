#include "Steelyard.h"
#include "../util/Model.h"

namespace {
	const VECTOR scale = { 0.5f,0.5f, 0.5f };
}

Steelyard::Steelyard()
{
	model_.push_back(std::make_shared<Model>("data/model/steelyard.mv1"));
	model_.push_back(std::make_shared<Model>(model_.front()->getModelHandle()));

	float x = 0;
	for (auto& model : model_) {
		model->setScale(scale);
		model->setPos({ 1550 + x,60,0 });
		x += 300;
	}
	
}

Steelyard::~Steelyard()
{
}

void Steelyard::update()
{
	
}

void Steelyard::draw()
{
	for (auto& model : model_) {
		model->draw();
	}
}
