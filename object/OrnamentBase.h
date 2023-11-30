#pragma once
#include "ObjectBase.h"
class OrnamentBase : public ObjectBase
{
public:
	OrnamentBase(int handle, LoadObjectInfo objInfo);
	virtual ~OrnamentBase();

	//更新
	virtual void Update(Player& player, const InputState& input)override;
	//描画
	virtual void Draw();

	//衝突判定
	virtual void HitColl(std::shared_ptr<ObjectBase> pointer);

	//衝突判定を行うモデルを追加する
	virtual std::shared_ptr<Model> AddCollModel();

private:

};

