#pragma once

class SceneManager;
class InputState;

class SceneBase
{
public:
	SceneBase(SceneManager& manager) : manager_(manager){}
	virtual ~SceneBase() {};
	
	virtual void init() = 0;
	virtual void end() = 0;

	virtual void update(const InputState& input) = 0;
	virtual void draw() = 0;
protected:
	SceneManager& manager_;
};

