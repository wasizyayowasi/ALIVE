#pragma once
#include "EnemyBase.h"

class TempEnemy : public EnemyBase
{
public:
	TempEnemy(const char* const filename, LoadObjectInfo objInfo);
	TempEnemy(int handle, LoadObjectInfo objInfo);
	virtual ~TempEnemy();

	//•`‰æ
	//void Draw();

private:

	
};

