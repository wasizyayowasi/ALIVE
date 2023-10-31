#pragma once
#include "../object/OrnamentBase.h"

class GimmickBase : public OrnamentBase
{
public:
	//ファイルパスでモデルを作成するコンストラクタ
	GimmickBase(const char* filename, LoadObjectInfo objInfo);
	//作成済みのモデルをコピーするコンストラクタ
	GimmickBase(int handle, LoadObjectInfo objInfo);
	//デストラクタ
	virtual ~GimmickBase();

	//更新
	virtual void Update(Player& player)override;
	//描画
	void Draw();

	//衝突判定
	virtual void HitColl(std::shared_ptr<ObjectBase> pointer);

	std::shared_ptr<Model> GetModelInfo() { return model_; }

protected:

	bool isTransit_ = false;

};

