#pragma once
#include "DxLib.h"
#include <memory>

class Model;

class CharacterBase
{
public:
	CharacterBase() {}
	virtual ~CharacterBase() {};

	virtual void update();
	virtual void draw() = 0;

	const VECTOR getPos() { return pos_; }

protected:

	int animNo_;			//アニメーション番号

	bool isAnimLoop_;		//アニメーションをループ再生させるか

	VECTOR pos_;			//ポジション
	VECTOR rot_;			//回転

	std::shared_ptr<Model> model_;

};

