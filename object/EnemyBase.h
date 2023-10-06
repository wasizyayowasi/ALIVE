#pragma once
#include "CharacterBase.h"

class EnemyBase : public CharacterBase
{
public:
	//ファイルパスでモデルクラスをインスタンス化するコンストラクタ
	EnemyBase(const char* fileName);
	//作成済みモデルでモデルクラスをインスタンス化するコンストラクタ
	EnemyBase(int handle);
	//デストラクタ
	virtual ~EnemyBase() {};

	void update(Player& player) override;
	void stalkingUpdate();

private:


};

