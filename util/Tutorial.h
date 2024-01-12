#pragma once
#include <DxLib.h>
#include <unordered_map>

enum class UIGraph {
	xboxBotton,
	keyBord,
	max,
};

enum class XboxBotton {
	A,
	B,
	X,
	Y,
	max,
};

class Tutorial
{
public:

	Tutorial();
	virtual ~Tutorial();

	void Update(VECTOR pos);
	void Draw(bool inputDevice);

	void NoneDraw(bool inputDevice);
	void SwitchTutorialDraw(bool inputDevice);
	void CranckTutorialDraw(bool inputDevice);

private:
	
	std::unordered_map<UIGraph,int> UIHandle_;			//画像

	int xboxBottonSizeX = 0;							//xbox用ボタンのサイズX
	int xboxBottonSizeY = 0;							//xbox用ボタンのサイズY

	int keyBottonSizeX = 0;								//キーボード用ボタンのサイズX
	int keyBottonSizeY = 0;								//キーボード用ボタンのサイズY

	VECTOR tutorialDrawPos_ = {};

	void(Tutorial::* drawFunc_)(bool inputDevice);

};

