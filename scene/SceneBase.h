#pragma once

class SceneManager;

class SceneBase
{
public:
	SceneBase(SceneManager& manager) : manager_(manager){}
	virtual ~SceneBase() {};
	
	virtual void Init() = 0;
	virtual void End() = 0;

	virtual void Update() = 0;
	virtual void Draw() = 0;
protected:
	SceneManager& manager_;
};

