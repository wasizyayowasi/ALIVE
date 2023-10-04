#pragma once
#include "CharacterBase.h"

class Model;

class DeadPerson : public CharacterBase
{
public:

	DeadPerson(const char* const filename,VECTOR pos,VECTOR rot,int animNo);
	DeadPerson(int handle, VECTOR pos, VECTOR rot, int animNo);
	virtual ~DeadPerson();

	void update();
	void draw();

private:


};

