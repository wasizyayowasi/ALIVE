#include "DeadPerson.h"
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

void DeadPerson::update()
{
}

void DeadPerson::draw()
{
	model_->draw();
}

