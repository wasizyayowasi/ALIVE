#pragma once
#include "DxLib.h"
#include <memory>

class Model;
class Enemy;

class CharacterBase
{
	friend Enemy;
public:
	CharacterBase(const char* fileName);
	CharacterBase(int modelHandle);
	virtual ~CharacterBase() {};

	virtual void update();
	virtual void draw();

	const VECTOR getPos() { return pos_; }

protected:

	int animNo_;			//アニメーション番号

	bool isAnimLoop_;		//アニメーションをループ再生させるか

	VECTOR pos_;			//ポジション
	VECTOR rot_;			//回転

	std::shared_ptr<Model> model_;

};

