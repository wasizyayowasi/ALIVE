#pragma once
#include "KeyData.h"
#include <DxLib.h>
#include <map>

class Tutorial
{
private:
	//UI画像の列挙
	enum class UIGraph {
		xboxBotton,
		keyBord,
		max,
	};
	//padボタンの列挙
	enum class XboxBotton {
		A,
		B,
		X,
		Y,
		max,
	};
	//チュートリアル
	enum class TutorialGimmick {
		Switch,
		Lever,
		Crank,
		max,
	};
public:

	Tutorial();
	virtual ~Tutorial();

	void Update(VECTOR pos);
	void Draw(bool inputDevice);

	void NoneDraw(bool inputDevice);
	void SwitchTutorialDraw(bool inputDevice);
	void CranckTutorialDraw(bool inputDevice);

private:
	
	std::map<UIGraph,int> UIHandle_;					//画像
	std::map<int, Key> keyNum_;							//キー番号

	VECTOR tutorialDrawPos_ = {};

	void(Tutorial::* drawFunc_)(bool inputDevice);

};

