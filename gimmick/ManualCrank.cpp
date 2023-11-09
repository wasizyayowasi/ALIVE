#include "ManualCrank.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../object/Player.h"
#include <algorithm>
#include <math.h>


namespace {
	const char* const box_filename = "data/model/box.mv1";

	const VECTOR center_point = VGet(-200, 100, 0);

	constexpr float max_rot_Z = 630.0f;
}

ManualCrank::ManualCrank()
{
	model_ = std::make_shared<Model>(box_filename);
	model_->SetRot(VGet(0.0f, 0.0f, 0.0f));
	model_->SetScale(VGet(50, 50, 50));

	float radian = 0.0f * DX_PI_F / 180.0f;
	float x = sin(radian);
	float y = cos(radian);

	pos_.x = pos_.x + x * 40;
	pos_.y = pos_.y + y * 40;

	model_->SetPos(pos_);

}

ManualCrank::~ManualCrank()
{
	
}

void ManualCrank::Draw()
{
	model_->Draw();
}

bool ManualCrank::HitCollPlayer(Player& player)
{
	
	MV1_COLL_RESULT_POLY_DIM result;

	VECTOR playerPos = player.GetStatus().pos;

	result = MV1CollCheck_Capsule(model_->GetModelHandle(), model_->GetColFrameIndex(), playerPos, VAdd(playerPos, VGet(0, player.GetStatus().height, 0)),30.0f);

	if (result.HitNum < 1) {
		MV1CollResultPolyDimTerminate(result);
		return false;
	}

	MV1CollResultPolyDimTerminate(result);

	return true;
}

float ManualCrank::GetMaxRotZ()
{
	return max_rot_Z;
}
