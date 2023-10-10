#include "Steelyard.h"
#include "../util/Model.h"

namespace {
	const VECTOR scale = { 0.5f,0.5f, 0.5f };
}

Steelyard::Steelyard(const char* const filename):GimmickBase(filename)
{
	model_->SetScale(scale);
	model_->SetPos({ 1845,0,0 });
	model_->SetCollFrame();
}

Steelyard::Steelyard(int handle):GimmickBase(handle)
{
}

Steelyard::~Steelyard()
{
}

void Steelyard::Update(Player& player)
{
}

void Steelyard::Draw()
{
	model_->Draw();
}
