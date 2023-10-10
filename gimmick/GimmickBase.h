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
	virtual void Update(Player& player)override;
	//描画
	void Draw();

	std::shared_ptr<Model> GetModelInfo() { return model_; }

private:

};

