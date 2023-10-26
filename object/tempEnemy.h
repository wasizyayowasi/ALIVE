#pragma once
#include "EnemyBase.h"

class tempEnemy : public EnemyBase
{
public:
	tempEnemy(const char* const filename, LoadObjectInfo objInfo);
	tempEnemy(int handle, LoadObjectInfo objInfo);
	virtual ~tempEnemy();

	//•`‰æ
	//void Draw();

private:

	
};

