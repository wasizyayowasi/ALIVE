#pragma once
#include "DxLib.h"
#include "ObjectBase.h"
#include <memory>

class CharacterBase : public ObjectBase
{
public:
	//作成済みモデルでモデルクラスをインスタンス化するコンストラクタ
	CharacterBase(int handle, LoadObjectInfo objInfo);
	//デストラクタ
	virtual ~CharacterBase() {};

	//更新
	virtual void Update(Player& player, const InputState& input) override;
	//描画
	virtual void Draw();

	//衝突判定
	virtual void HitColl(std::shared_ptr<ObjectBase> pointer);

	//衝突判定を行うモデルを追加する
	virtual std::shared_ptr<Model> AddCollModel();

protected:

	int animNo_ = 0;				//アニメーション番号

	bool isAnimLoop_ = false;		//アニメーションをループ再生させるか

};

