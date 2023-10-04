#pragma once
#include "EnemyBase.h"

class tempEnemy : public EnemyBase
{
public:
	tempEnemy(const char* const filename);
	tempEnemy(int handle);
	virtual ~tempEnemy();

	void update();
	void draw();
private:



};

