#include "tempEnemy.h"
#include "../util/Model.h"
#include "../object/Player.h"

TempEnemy::TempEnemy(const char* const filename, LoadObjectInfo objInfo):EnemyBase(filename,objInfo)
{
	scale_ = objInfo.scale;
}

TempEnemy::TempEnemy(int handle, LoadObjectInfo objInfo) : EnemyBase(handle,objInfo)
{
	scale_ = objInfo.scale;
}

TempEnemy::~TempEnemy()
{
}

//void tempEnemy::Draw()
//{
//	model_->Draw();
//}