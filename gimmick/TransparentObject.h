#pragma once
#include "GimmickBase.h"
#include <memory>

class Switch;

class TransparentObject : public GimmickBase
{
public:
	//コンストラクタ
	TransparentObject(const char* const filename, LoadObjectInfo objInfo);
	TransparentObject(int handle, LoadObjectInfo objInfo);
	//デストラクタ
	virtual ~TransparentObject();

	//更新
	void Update(Player& player)override;
	//描画
	void Draw();

	//天秤モデルと死体との衝突判定を行う
	void HitColl(std::shared_ptr<ObjectBase> deadPerson);

private:

	float alphaValue_ = 0.0f;

	VECTOR InitialPosition_ = {};						//初期の高さを取得する

	std::shared_ptr<Switch> switch_;

};

