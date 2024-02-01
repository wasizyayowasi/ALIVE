#include "DeadPerson.h"
#include "Player.h"
#include "../util/Model.h"
#include "../object/PlayerData.h"

namespace {
	//ƒ‚ƒfƒ‹ƒtƒŒ[ƒ€–¼
	const char* const coll_frame_death = "CollDown";
	const char* const coll_frame_Sit = "CollSit";
}

DeadPerson::DeadPerson(int handle,Material materialType, LoadObjectInfo objInfo, int animNo) : CharacterBase(handle, materialType,objInfo)
{
	model_->SetPos(objInfo.pos);
	model_->SetRot(objInfo.rot);
	model_->SetScale(objInfo.scale);
	MV1SetMaterialDifColor(model_->GetModelHandle(), 8, GetColorF(1.0f,0.0f,0.0f,1.0f));

	isCollCheck_ = true;

	model_->SetAnimation(animNo, false, true);
	model_->SetAnimEndFrame(animNo);
	switch (static_cast<PlayerAnimType>(animNo)) {
	case PlayerAnimType::Death:
		model_->SetUseCollision(true,true,coll_frame_death);
		break;
	case PlayerAnimType::idleToSitup:
		model_->SetUseCollision(true, true, coll_frame_Sit);
		break;
	}
}

DeadPerson::~DeadPerson()
{
}

void DeadPerson::Update(Player& player)
{
	pos_ = model_->GetPos();
}

void DeadPerson::Draw()
{
	model_->Draw();
}