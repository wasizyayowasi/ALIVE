#include "Tutorial.h"
#include "ExternalFile.h"
#include "InputState.h"
#include "Util.h"
#include "DrawFunctions.h"
#include "game.h"

namespace {
	//xboxのボタン画像のファイルパス
	const char* const xbox_Botton_filepath = "data/graph/ControllerBotton.png";
	const char* const key_filepath = "data/graph/key2.png";
}

Tutorial::Tutorial():drawFunc_(&Tutorial::NoneDraw)
{
	//UI画像の読み込み
	UIHandle_[UIGraph::xboxBotton] = Graph::LoadGraph(xbox_Botton_filepath);
	UIHandle_[UIGraph::keyBord] = Graph::LoadGraph(key_filepath);

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

void Tutorial::Update(const InputState& input, VECTOR pos)
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
		tutorialDrawPos_ = tutorialInfo.pos;
	}
	else {
		drawFunc_ = &Tutorial::NoneDraw;
	}

	//inputMapTable_ = input.inputMapTable_;
	//inputNameTable_ = input.inputNameTable_;

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
		Graph::DrawRectRotaGraph(Game::screen_width / 2 - xboxBottonSizeY, Game::screen_height - xboxBottonSizeY, (xboxBottonSizeX / 4) * static_cast<int>(XboxBotton::B), 0, xboxBottonSizeX / 4, xboxBottonSizeY, 1.0f, 0.0f, UIHandle_[UIGraph::xboxBotton], true, false);
	}
}
