#include "Effect2D.h"
#include <algorithm>

Effect2D::Effect2D(const char* const filepath, int numX, int numY, int graphSizeX, int graphSizeY,float size, VECTOR pos)
{
	//分割読み込み
	LoadDivGraph(filepath, numX * numY, numX, numY, graphSizeX, graphSizeY, handle_, true);

	//分割数を残す
	totalNum_ = numX * numY;

	//存在しているフラグをたてる
	isEnabled_ = true;

	//描画サイズ
	drawSize_ = size;

	//描画場所
	drawPos_ = pos;
}

Effect2D::~Effect2D()
{
	//画像の削除
	for (auto& graph : handle_) {
		DeleteGraph(graph);
	}
}

void Effect2D::Update()
{
	//画像の更新
	currentNum_ = (std::min)(currentNum_ + 1, 60);

	//現在の番号が画像の枚数と同数になったら存在を消す
	if (currentNum_ == totalNum_) {
		isEnabled_ = false;
	}
}

void Effect2D::DrawBillboard()
{
	DrawBillboard3D(drawPos_, 0.5f, 0.5f, drawSize_, 0.0f, handle_[currentNum_], true);
}
