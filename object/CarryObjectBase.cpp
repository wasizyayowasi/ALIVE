#include "CarryObjectBase.h"
#include "Player.h"
#include "../util/Model.h"

CarryObjectBase::CarryObjectBase(const char* filename):OrnamentBase(filename)
{
}

CarryObjectBase::CarryObjectBase(int handle) : OrnamentBase(handle)
{
}

CarryObjectBase::~CarryObjectBase()
{
}

void CarryObjectBase::Update(Player& player)
{
	
}

void CarryObjectBase::Draw()
{
	model_->Draw();
}

bool CarryObjectBase::CollInfo()
{
	// hitDim_ = MV1CollCheck_Capsule(object_->getModelHandle(), object_->getColFrameIndex(), VAdd(player.getPos(), VGet(0.0f, 10.0f, 0.0f)), VAdd(player.getPos(), VGet(0.0f, 150.0f, 0.0f)), 20.0f);

	 if (hitDim_.HitNum < 1) {
		 MV1CollResultPolyDimTerminate(hitDim_);
		 return false;
	 }
	 
	 MV1CollResultPolyDimTerminate(hitDim_);
	 return true;

}
