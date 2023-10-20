#include "DeadPerson.h"
#include "Player.h"
#include "../util/Model.h"
#include "../util/PlayerData.h"

namespace {
	//���f���t���[����
	const char* const coll_frame_death = "CollisionDeath";
	const char* const coll_frame_Sit = "CollisionSit";
	const char* const coll_frame_Stand = "CollisionStand";
}

DeadPerson::DeadPerson(const char* const filename, LoadObjectInfo objInfo, int animNo) : CharacterBase(filename,objInfo)
{
	model_->SetPos(objInfo.pos);
	model_->SetRot(objInfo.rot);
	model_->SetScale(objInfo.scale);
	model_->SetAnimEndFrame(animNo);

	switch (static_cast<AnimType>(animNo)) {
	case AnimType::death:
		model_->SetCollFrame(coll_frame_death);
		break;
	case AnimType::sit:
		model_->SetCollFrame(coll_frame_Sit);
		break;
	}

}

DeadPerson::DeadPerson(int handle,LoadObjectInfo objInfo, int animNo) : CharacterBase(handle,objInfo),updateFunc_(&DeadPerson::NormalUpdate)
{
	model_->SetPos(objInfo.pos);
	model_->SetRot(objInfo.rot);
	model_->SetScale(objInfo.scale);
	model_->SetAnimEndFrame(animNo);

	switch (static_cast<AnimType>(animNo)) {
	case AnimType::death:
		model_->SetCollFrame(coll_frame_death);
		break;
	case AnimType::sit:
		model_->SetCollFrame(coll_frame_Sit);
		break;
	}

}

DeadPerson::~DeadPerson()
{
}

void DeadPerson::Update(Player& player)
{
	(this->*updateFunc_)(player);
}

void DeadPerson::Draw()
{
	model_->Draw();
}

void DeadPerson::NormalUpdate(Player& player)
{
	HitColl(player);

	MV1CollResultPolyDimTerminate(hitDim_);
}

void DeadPerson::DuringTransportUpdate(Player& player)
{
	model_->SetPos(player.GetStatus().pos);
}

void DeadPerson::HitColl(Player& player)
{
	hitDim_ = MV1CollCheck_Capsule(model_->GetModelHandle(), model_->GetColFrameIndex(), VAdd(player.GetStatus().pos, VGet(0.0f, 10.0f, 0.0f)), VAdd(player.GetStatus().pos, VGet(0.0f, 150.0f, 0.0f)), 20.0f);

	if (hitDim_.HitNum < 1) return;

}
