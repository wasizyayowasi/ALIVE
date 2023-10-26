#include "tempEnemy.h"
#include "../util/Model.h"
#include "../object/Player.h"

tempEnemy::tempEnemy(const char* const filename, LoadObjectInfo objInfo):EnemyBase(filename,objInfo)
{
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);
	model_->SetRot(objInfo.rot);
	scale_ = objInfo.scale;
}

tempEnemy::tempEnemy(int handle, LoadObjectInfo objInfo) : EnemyBase(handle,objInfo)
{
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);
	pos_ = VGet(-50, 0, -50);
	model_->SetPos(pos_);
	model_->SetRot(objInfo.rot);
	scale_ = objInfo.scale;
}

tempEnemy::~tempEnemy()
{
}

//void tempEnemy::Draw()
//{
//	model_->Draw();
//}