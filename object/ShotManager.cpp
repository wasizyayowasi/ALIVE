#include "ShotManager.h"
#include "../object/Shot.h"

namespace {
	const char* const shot_filename = "data/model/box.mv1";

	//“Š±•¨‚Ì‘¬“x
	constexpr float shot_speed = 5.0f;
}

ShotManager::ShotManager()
{
	shotHandle_ = MV1LoadModel(shot_filename);
}

ShotManager::~ShotManager()
{
	MV1DeleteModel(shotHandle_);
}

void ShotManager::Update()
{
	for (auto& shot : shots_) {
		if (shot->IsEnabled()) {
			shot->Update();
		}
	}

	shots_.remove_if([](std::shared_ptr<Shot> shot) {return !shot->IsEnabled(); });

}

void ShotManager::Draw()
{
	for (auto& shot : shots_) {
		if (shot->IsEnabled()) {
			shot->Draw();
		}
	}
}

void ShotManager::Fire(VECTOR framePos, VECTOR playerPos,float height)
{
	VECTOR distance = VSub(VGet(playerPos.x, playerPos.y + height, playerPos.z), framePos);

	VECTOR moveVec = VScale(VNorm(distance), shot_speed);

	shots_.push_back(std::make_shared<Shot>(shotHandle_, framePos, moveVec));
}
