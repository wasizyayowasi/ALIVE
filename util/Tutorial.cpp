#include "Tutorial.h"
#include "ExternalFile.h"
#include "InputState.h"
#include "Util.h"
#include "DrawFunctions.h"

namespace {
	//xboxのボタン画像のファイルパス
	const char* const xbox_Botton_filepath = "data/graph/ControllerBotton.png";
	const char* const key_filepath = "data/graph/key2.png";
}

Tutorial::Tutorial():drawFunc_(&Tutorial::NoneDraw)
{
	//UI画像の読み込み
	UIHandle_[UIGraph::xboxBotton] = LoadGraph(xbox_Botton_filepath);
	UIHandle_[UIGraph::keyBord] = LoadGraph(key_filepath);

	GetGraphSize(UIHandle_[UIGraph::xboxBotton], &xboxBottonSizeX, &xboxBottonSizeY);
	GetGraphSize(UIHandle_[UIGraph::keyBord], &keyBottonSizeX, &keyBottonSizeY);
}

Tutorial::~Tutorial()
{
	//画像の削除
	for (auto& handle : UIHandle_) {
		DeleteGraph(handle.second);
	}
}

void Tutorial::Update(VECTOR pos)
{
	float distanceSize = 0.0f;

	auto tutorialInfo = ExternalFile::GetInstance().GetTutorialObjInfo(pos);

	distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(tutorialInfo.pos, pos);

	float range = VSize(tutorialInfo.scale) / 3;

	if (range > distanceSize) {
		if (tutorialInfo.name == "CrankTutorial") {
			drawFunc_ = &Tutorial::CranckTutorialDraw;
		}
		else if (tutorialInfo.name == "SwitchTutorial") {
			drawFunc_ = &Tutorial::SwitchTutorialDraw;
		}
		else {
			drawFunc_ = &Tutorial::NoneDraw;
		}
		tutorialDrawPos_ = tutorialInfo.pos;
	}

}

void Tutorial::Draw(bool inputDevice)
{
	(this->*drawFunc_)(inputDevice);
}

void Tutorial::NoneDraw(bool inputDevice)
{
}

void Tutorial::SwitchTutorialDraw(bool inputDevice)
{
	if (inputDevice) {
		DrawSphere3D(tutorialDrawPos_, 32, 32, 0xff0000, 0xff0000, true);
	}
	else {
		DrawSphere3D(tutorialDrawPos_, 32, 32, 0x0000ff, 0x0000ff, true);
	}
}

void Tutorial::CranckTutorialDraw(bool inputDevice)
{
	if (inputDevice) {
		DrawSphere3D(tutorialDrawPos_, 32, 32, 0xff0000, 0xff0000, true);
	}
	else {
		DrawSphere3D(tutorialDrawPos_, 32, 32, 0x0000ff, 0x0000ff, true);
	}
}
