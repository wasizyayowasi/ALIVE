#include "Shot.h"
#include "../util/Model.h"
#include "../util/Util.h"
#include "Player.h"

namespace {
	//è¡é∏ÉâÉCÉì
	constexpr float vanishing_line = 1000.0f;

	//èdóÕ
	constexpr float gravity = -1.0f;
}

Shot::Shot(int handle,VECTOR initPos, VECTOR moveVec)
{
	model_ = std::make_shared<Model>(handle);
	float scale = 10.0f;
	model_->SetScale(VGet(scale, scale, scale));
	pos_ = initPos;
	initialisePos_ = initPos;
	moveVec_ = moveVec;
	isEnable_ = true;
}

Shot::~Shot()
{
}

void Shot::Update()
{
	pos_ = VAdd(pos_, moveVec_);

	model_->SetPos(pos_);

	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(initialisePos_, pos_);

	if (distanceSize > vanishing_line) {
		isEnable_ = false;
	}

	model_->Update();
}

void Shot::Draw()
{
	model_->Draw();
}

void Shot::HitCheck(Player& player)
{

	VECTOR playerPos = player.GetStatus().pos;
	float playerHeight = player.GetStatus().height;

	bool hit = HitCheck_Sphere_Capsule(pos_, 50.0f, playerPos, VGet(playerPos.x, playerPos.y + playerHeight, playerPos.z),20.0f);

	if (hit) {
		VECTOR nockBack = VScale(VNorm(moveVec_),5.0f);
		player.BulletHitMe(nockBack);

		isEnable_ = false;
	}
}
