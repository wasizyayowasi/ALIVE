#include "EffectManager.h"
#include "Effect2D.h"
#include <algorithm>

EffectManager::EffectManager()
{
	filepath_["hit"] = ("data/effect/BigHit.png");
	handleInfo_["hit"].numX = 9;
	handleInfo_["hit"].numY = 3;
	handleInfo_["hit"].chipSizeX = 516;
	handleInfo_["hit"].chipSizeY = 528;
	handleInfo_["hit"].size = 50.0f;
}

EffectManager::~EffectManager()
{
}

void EffectManager::Update()
{
	effect2D_.remove_if([](std::shared_ptr<Effect2D> effectPoint) {return !effectPoint->IsEnable(); });

	//XV
	for (auto& effect : effect2D_) {
		effect->Update();
	}
}

void EffectManager::Draw()
{
	//•`‰æ
	for (auto& effect : effect2D_) {
		effect->DrawBillboard();
	}
}

void EffectManager::AddEffect(std::string name,VECTOR pos)
{
	effect2D_.push_back(std::make_shared<Effect2D>(filepath_[name].c_str(),
												handleInfo_[name].numX, handleInfo_[name].numY,
												handleInfo_[name].chipSizeX, handleInfo_[name].chipSizeY,
												handleInfo_[name].size, pos));
}