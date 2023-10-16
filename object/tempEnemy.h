#pragma once
#include "EnemyBase.h"

class tempEnemy : public EnemyBase
{
public:
	tempEnemy(const char* const filename, LoadObjectInfo objInfo);
	tempEnemy(int handle, LoadObjectInfo objInfo);
	virtual ~tempEnemy();

	void Update(Player& player) override;
	void Draw();

	void TrackingUpdate(Player& player);

private:

	int temp = 0;
	int count = 0;

};

