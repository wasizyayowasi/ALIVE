#pragma once
#include "ObjectBase.h"

class OrnamentBase : public ObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="objInfo">配置データ</param>
	OrnamentBase(int handle, LoadObjectInfo objInfo);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~OrnamentBase();
};

