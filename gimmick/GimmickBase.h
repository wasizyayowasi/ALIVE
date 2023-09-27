#pragma once
#include "DxLib.h"
#include <memory>

class Model;
class Switch;
class Player;

constexpr int max_hit_coll = 2048;

class GimmickBase
{
	friend Switch;
public:
	//ファイルパスでモデルを作成するコンストラクタ
	GimmickBase(const char* filename);
	//作成済みのモデルをコピーするコンストラクタ
	GimmickBase(int handle);
	//デストラクタ
	virtual ~GimmickBase();

	//更新
	virtual void update();
	//描画
	void draw();

private:

	VECTOR pos_;		//ポジション

	std::shared_ptr<Model> model_;		//3Dモデル

};

