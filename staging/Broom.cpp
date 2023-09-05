#include "broom.h"
#include "../OBJECT/Camera.h"
#include "../UTIL/game.h"
#include "../staging/Broom.h"

Broom::Broom()
{

	camera_ = std::make_shared<Camera>();

	gaussRation_ = 900;
	reductionScale_ = 8;
	reductionScreenWidth_ = Game::kScreenWidth / reductionScale_;
	reductionScreenHeight_ = Game::kScreenHeight / reductionScale_;

	normalScreen_ = MakeScreen(Game::kScreenWidth,Game::kScreenHeight,false);
	highBrightScreen_ = MakeScreen(Game::kScreenWidth, Game::kScreenHeight, false);
	reductioScaleScreen_ = MakeScreen(reductionScreenWidth_,reductionScreenHeight_,false);
	gaussScreen_ = MakeScreen(reductionScreenWidth_, reductionScreenHeight_, false);
	
}

Broom::~Broom()
{
	DeleteGraph(normalScreen_);
	DeleteGraph(highBrightScreen_);
	DeleteGraph(reductioScaleScreen_);
	DeleteGraph(gaussScreen_);
}

void Broom::writingScreenUpdate(VECTOR playerPos)
{
	//通常の描画結果を書き込むスクリーンを描画対象にする
	SetDrawScreen(normalScreen_);

	//画面のクリア
	ClearDrawScreen();

	camera_->fixedPointCamera(playerPos);
}

void Broom::graphFilterUpdate()
{
	//normalScreen_から高輝度部分のみを抜き出した画像を得る
	GraphFilterBlt(normalScreen_, highBrightScreen_, DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_LESS, 230, true, 0x000000, 255);

	//highBrightScreen_を8分の1に縮小した画像を得る
	GraphFilterBlt(highBrightScreen_, reductioScaleScreen_, DX_GRAPH_FILTER_DOWN_SCALE, reductionScale_);

	//reductionScaleScreen_にガウスフィルタ処理を施す
	GraphFilterBlt(reductioScaleScreen_, gaussScreen_, DX_GRAPH_FILTER_GAUSS, 16, gaussRation_);

	//描画対象を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);
}

void Broom::update()
{

}

void Broom::draw()
{
	//通常の描画結果を描画する
	DrawGraph(0, 0, normalScreen_, false);

	//描画モードをバイリニアフィルタリングにする
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	//描画ブレンドモードを加算にする
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);

	//高輝度部分を縮小してぼかした画像を画面全体に2回描画する(2回描画するのはより明るくするため)
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kScreenHeight, gaussScreen_, false);
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kScreenHeight, gaussScreen_, false);

	//描画ブレンドモードをブレンドなしに戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//描画モードをニアレストに戻す
	SetDrawMode(DX_DRAWMODE_NEAREST);

}


