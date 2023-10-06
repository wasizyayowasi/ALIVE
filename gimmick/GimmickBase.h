#pragma once
#include "../object/OrnamentBase.h"

constexpr int max_hit_coll = 2048;

class GimmickBase : public OrnamentBase
{
public:
	//ファイルパスでモデルを作成するコンストラクタ
	GimmickBase(const char* filename);
	//作成済みのモデルをコピーするコンストラクタ
	GimmickBase(int handle);
	//デストラクタ
	virtual ~GimmickBase();

	//更新
	virtual void update(Player& player)override;
	//描画
	void draw();

	std::shared_ptr<Model> getModelInfo() { return model_; }

private:

};

