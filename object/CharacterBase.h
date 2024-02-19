#pragma once
#include "DxLib.h"
#include "ObjectBase.h"
#include <memory>

class CharacterBase : public ObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="materialType">マテリアルのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	CharacterBase(const int handle, const Material materialType, const LoadObjectInfo objInfo);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CharacterBase() {};
protected:

	int animNo_ = 0;				//アニメーション番号

	bool isAnimLoop_ = false;		//アニメーションをループ再生させるか

};

