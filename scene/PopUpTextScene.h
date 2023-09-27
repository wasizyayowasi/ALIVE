#pragma once
#include "SceneBase.h"

class InputState;

class PopUpTextScene : public SceneBase
{
public:

	PopUpTextScene(SceneManager& manager);
	virtual ~PopUpTextScene();

	void loadText();

	void init();
	void end();

	void update(const InputState& input);
	void draw();

private:


};

