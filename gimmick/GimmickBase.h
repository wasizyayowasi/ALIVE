#pragma once
#include "DxLib.h"
#include <memory>

class Model;
class Switch;
class Steelyard;
class Player;

constexpr int max_hit_coll = 2048;

class GimmickBase
{
	friend Switch;
	friend Steelyard;
public:
	//ファイルパスでモデルを作成するコンストラクタ
	GimmickBase(const char* filename);
	//作成済みのモデルをコピーするコンストラクタ
	GimmickBase(int handle);
	//デストラクタ
	virtual ~GimmickBase();

	//更新
	virtual void update(Player& player);
	//描画
	void draw();

	std::shared_ptr<Model> getModelInfo() { return model_; }

private:

	VECTOR pos_;		//ポジション

	std::shared_ptr<Model> model_;		//3Dモデル

};

