#pragma once
#include "SceneBase.h"

class InputState;

class PopUpTextScene : public SceneBase
{
public:

	PopUpTextScene(SceneManager& manager);
	virtual ~PopUpTextScene();

	void loadText();

	void Init();
	void End();

	void Update(const InputState& input);
	void Draw();

private:


};

