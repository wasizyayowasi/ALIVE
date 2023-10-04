#pragma once
#include "CharacterBase.h"

class Model;

class DeadPerson : public CharacterBase
{
public:

	DeadPerson(const char* const filename);
	DeadPerson(int handle);
	virtual ~DeadPerson();

	void update();
	void draw();

private:


};

