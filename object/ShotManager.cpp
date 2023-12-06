#include "ShotManager.h"
#include "../object/Shot.h"


namespace {
	const char* const shot_filename = "data/model/other/mv1/rock.mv1";

	//投擲物の速度
	constexpr float shot_speed = 20.0f;
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

void ShotManager::Hit(Player& player)
{
	for (auto shot : shots_) {
		if (shot->IsEnabled()) {
			shot->HitCheck(player);
		}
	}
}

void ShotManager::Fire(VECTOR framePos, VECTOR playerPos,float height)
{
	//プレイヤーめがけてショットを撃つ
	VECTOR distance = VSub(VGet(playerPos.x, playerPos.y + height / 2, playerPos.z), framePos);
	VECTOR moveVec = VScale(VNorm(distance), shot_speed);

	//正面ベクトル
//	VECTOR frontVec = VGet(0.0f, 0.0f, -1.0f);
//	VECTOR moveVec = VScale(frontVec, shot_speed);

	shots_.push_back(std::make_shared<Shot>(shotHandle_, framePos, moveVec));
}
