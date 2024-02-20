#include "ShotManager.h"

#include "../object/Shot.h"

#include "../util/ModelManager.h"

namespace {
	//投擲物の速度
	constexpr float shot_speed = 20.0f;
}

ShotManager::ShotManager()
{
}

ShotManager::~ShotManager()
{
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

void ShotManager::Hit(Player& player)
{
	for (auto shot : shots_) {
		if (shot->IsEnabled()) {
			shot->HitCheck(player);
		}
	}
}

void ShotManager::Fire(const VECTOR framePos, const  VECTOR playerPos, const float height)
{
	//プレイヤーめがけてショットを撃つ
	VECTOR distance = VSub(VGet(playerPos.x, playerPos.y + height / 2, playerPos.z), framePos);
	VECTOR moveVec = VScale(VNorm(distance), shot_speed);

	shots_.push_back(std::make_shared<Shot>(ModelManager::GetInstance().GetModelHandle(objData[static_cast<int>(ObjectType::Rock)].name), framePos, moveVec));
}
