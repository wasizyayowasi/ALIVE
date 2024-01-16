#pragma once
#include <DxLib.h>
#include <map>

class InputState;

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

	void Update(const InputState& input,VECTOR pos);
	void Draw(bool inputDevice);

	void NoneDraw(bool inputDevice);
	void SwitchTutorialDraw(bool inputDevice);
	void CranckTutorialDraw(bool inputDevice);

private:
	
	std::map<UIGraph,int> UIHandle_;					//画像

	int xboxBottonSizeX = 0;							//xbox用ボタンのサイズX
	int xboxBottonSizeY = 0;							//xbox用ボタンのサイズY

	int keyBottonSizeX = 0;								//キーボード用ボタンのサイズX
	int keyBottonSizeY = 0;								//キーボード用ボタンのサイズY

	VECTOR tutorialDrawPos_ = {};

	void(Tutorial::* drawFunc_)(bool inputDevice);

};

