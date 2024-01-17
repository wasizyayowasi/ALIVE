#pragma once
#include "KeyData.h"
#include <DxLib.h>
#include <map>

class Tutorial
{
private:
	//UI�摜�̗�
	enum class UIGraph {
		xboxBotton,
		keyBord,
		max,
	};
	//pad�{�^���̗�
	enum class XboxBotton {
		A,
		B,
		X,
		Y,
		max,
	};
	//�`���[�g���A��
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
	
	std::map<UIGraph,int> UIHandle_;					//�摜
	std::map<int, Key> keyNum_;							//�L�[�ԍ�

	VECTOR tutorialDrawPos_ = {};

	void(Tutorial::* drawFunc_)(bool inputDevice);

};

