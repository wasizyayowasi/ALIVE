#include "DeadPerson.h"
#include "../util/Model.h"

DeadPerson::DeadPerson(const char* const filename) : CharacterBase(filename)
{
}

DeadPerson::DeadPerson(int handle) : CharacterBase(handle)
{
}

DeadPerson::~DeadPerson()
{
}

void DeadPerson::update()
{
}

void DeadPerson::draw()
{
	model_->draw();
}

