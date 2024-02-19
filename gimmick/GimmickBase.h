#pragma once
#include "../object/OrnamentBase.h"

class GimmickBase : public OrnamentBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="materialType">マテリアルのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	GimmickBase(const int handle, const Material materialType, const LoadObjectInfo objInfo);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~GimmickBase();
};

