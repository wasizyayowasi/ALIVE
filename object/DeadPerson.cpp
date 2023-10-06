#include "DeadPerson.h"
#include "Player.h"
#include "../util/Model.h"
#include "../util/ObjAnimType.h"

namespace {
	//ƒ‚ƒfƒ‹ƒtƒŒ[ƒ€–¼
	const char* const coll_frame_death = "CollisionDeath";
	const char* const coll_frame_Sit = "CollisionSit";
	const char* const coll_frame_Stand = "CollisionStand";
}

DeadPerson::DeadPerson(const char* const filename,VECTOR pos, VECTOR rot, int animNo) : CharacterBase(filename)
{
	model_->setPos(pos);
	model_->setRot(rot);
	model_->setScale(scale_);
	model_->setAnimEndFrame(animNo);

	switch (static_cast<AnimType>(animNo)) {
	case AnimType::death:
		model_->setCollFrame(coll_frame_death);
		break;
	case AnimType::sit:
		model_->setCollFrame(coll_frame_Sit);
		break;
	}

}

DeadPerson::DeadPerson(int handle, VECTOR pos, VECTOR rot, int animNo) : CharacterBase(handle)
{
	model_->setPos(pos);
	model_->setRot(rot);
	model_->setScale(scale_);
	model_->setAnimEndFrame(animNo);

	switch (static_cast<AnimType>(animNo)) {
	case AnimType::death:
		model_->setCollFrame(coll_frame_death);
		break;
	case AnimType::sit:
		model_->setCollFrame(coll_frame_Sit);
		break;
	}

}

DeadPerson::~DeadPerson()
{
}

void DeadPerson::update(Player& player)
{

}

void DeadPerson::draw()
{
	model_->draw();
}

void DeadPerson::hitColl(Player& player)
{
	hitDim_ = MV1CollCheck_Capsule(model_->getModelHandle(), model_->getColFrameIndex(), VAdd(player.getStatus().pos, VGet(0.0f, 10.0f, 0.0f)), VAdd(player.getStatus().pos, VGet(0.0f, 150.0f, 0.0f)), 20.0f);

	if (hitDim_.HitNum < 1) return;

}

