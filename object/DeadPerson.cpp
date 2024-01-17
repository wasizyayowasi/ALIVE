#include "DeadPerson.h"
#include "Player.h"
#include "../util/Model.h"
#include "../object/PlayerData.h"

namespace {
	//ƒ‚ƒfƒ‹ƒtƒŒ[ƒ€–¼
	const char* const coll_frame_death = "CollDown";
	const char* const coll_frame_Sit = "CollSit";
}

DeadPerson::DeadPerson(int handle,LoadObjectInfo objInfo, int animNo) : CharacterBase(handle,objInfo)
{
	model_->SetPos(objInfo.pos);
	model_->SetRot(objInfo.rot);
	model_->SetScale(objInfo.scale);
	MV1SetMaterialDifColor(model_->GetModelHandle(), 8, GetColorF(1.0f,0.0f,0.0f,1.0f));

	isCollCheck_ = true;

	animNo_ = animNo;
	Init();
}

DeadPerson::~DeadPerson()
{
}

void DeadPerson::Init()
{
	model_->SetAnimation(animNo_, false, true);
	model_->SetAnimEndFrame(animNo_);
	switch (static_cast<PlayerAnimType>(animNo_)) {
	case PlayerAnimType::Death:
		model_->SetCollFrame(coll_frame_death);
		break;
	case PlayerAnimType::idleToSitup:
		model_->SetCollFrame(coll_frame_Sit);
		break;
	}
}

void DeadPerson::Update(Player& player)
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

