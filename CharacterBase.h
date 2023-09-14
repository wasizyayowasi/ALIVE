#pragma once
#include "DxLib.h"
#include <memory>

class Model;

class CharacterBase
{
public:
	CharacterBase() {}
	virtual ~CharacterBase() {};

	virtual void update() = 0;
	virtual void draw() = 0;
private:

	int animNo_;			//アニメーション番号

	bool isAnimLoop_;		//アニメーションをループ再生させるか

	VECTOR pos_;			//ポジション
	VECTOR rot_;			//回転

	std::shared_ptr<Model> model_;

};

