#include "tempEnemy.h"
#include "../util/Model.h"
#include "../object/Player.h"

tempEnemy::tempEnemy(const char* const filename, LoadObjectInfo objInfo):EnemyBase(filename,objInfo)
{
	scale_ = objInfo.scale;
}

tempEnemy::tempEnemy(int handle, LoadObjectInfo objInfo) : EnemyBase(handle,objInfo)
{
	scale_ = objInfo.scale;
}

tempEnemy::~tempEnemy()
{
}

//void tempEnemy::Draw()
//{
//	model_->Draw();
//}