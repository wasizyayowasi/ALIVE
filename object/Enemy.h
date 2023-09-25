#pragma once
#include "CharacterBase.h"

class Player;

class Enemy : public CharacterBase
{
public:
	Enemy();
	virtual ~Enemy();

	void update() override;
	void stalkingUpdate();

private:


};

