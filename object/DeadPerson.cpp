#include "DeadPerson.h"
#include "Player.h"
#include "../util/Model.h"
#include "../util/PlayerData.h"

namespace {
	//ƒ‚ƒfƒ‹ƒtƒŒ[ƒ€–¼
	const char* const coll_frame_death = "Coll";
}

DeadPerson::DeadPerson(int handle,LoadObjectInfo objInfo, int animNo) : CharacterBase(handle,objInfo)
{
	model_->SetPos(objInfo.pos);
	model_->SetRot(objInfo.rot);
	model_->SetScale(objInfo.scale);
	MV1SetMaterialDifColor(model_->GetModelHandle(), 8, GetColorF(1.0f,0.0f,0.0f,1.0f));
	model_->SetAnimation(animNo, false, true);
	model_->SetAnimEndFrame(animNo);

	isCollCheck_ = true;

	switch (static_cast<AnimType>(animNo)) {
	case AnimType::death:
		model_->SetCollFrame(coll_frame_death);
		break;
	//case AnimType::sit:
	//	model_->SetCollFrame(coll_frame_Sit);
	//	break;
	}

}

DeadPerson::~DeadPerson()
{
}

void DeadPerson::Update(Player& player, const InputState& input)
{
	pos_ = model_->GetPos();
}

void DeadPerson::Draw()
{
	model_->Draw();
//	DrawSphere3D(pos_, 32, 32, 0xff0000, 0xff0000, true);
}

std::shared_ptr<Model> DeadPerson::AddCollModel()
{
	return nullptr;
}

void DeadPerson::HitColl(Player& player)
{
}

