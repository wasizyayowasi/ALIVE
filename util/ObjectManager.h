#pragma once
#include "ObjectData.h"
#include <DxLib.h>
#include <memory>
#include <list>
#include <unordered_map>

class ObjectBase;
class Model;
class Player;
class InputState;

class ObjectManager
{
public:

	ObjectManager();
	virtual ~ObjectManager();

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <param name="baseType">どのbaseクラスをもとに生成するか</param>
	/// <param name="objType">オブジェクトの種類</param>
	/// <param name="objInfo">オブジェクトの配置データ等</param>
	void ObjectGenerator();

	/// <summary>
	/// 死体生成
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="objInfo">オブジェクトの配置データ等</param>
	/// <param name="animNo">初期化時に使用するアニメーション番号</param>
	void DeadPersonGenerator(int handle, LoadObjectInfo objInfo,int animNo);

	//更新
	void Update(Player& player, const InputState& input);

	//描画
	void Draw(VECTOR pos);

	/// <summary>
	/// 衝突判定に使用するモデルを取得する
	/// </summary>
	/// <returns>衝突判定を行うモデルスマートポインタのリスト</returns>
	std::list<std::shared_ptr<Model>> GetAllCheckCollModel();

	void SortingObject(ObjectBaseType baseType, ObjectType objType, LoadObjectInfo objInfo);

	/// <summary>
	/// 特定のモデルポインタを取得する
	/// </summary>
	/// <param name="type">取得したいオブジェクトタイプ</param>
	/// <returns>特定のモデルポインタリスト</returns>
	std::list<std::shared_ptr<Model>> GetSpecificModel(ObjectType type);

	/// <summary>
	/// 特定のオブジェクトベースポインタを取得する
	/// </summary>
	/// <param name="type">取得したオブジェクトタイプ</param>
	/// <returns>特定のオブジェクトベースポインタ</returns>
	std::list<std::shared_ptr<ObjectBase>> GetSpecificObject(ObjectType type);

	//衝突判定を行うモデルを追加する
	void AddCheckCollModel();

private:
	//キャラクター生成機
	void CharacterGenerator(ObjectType objType, LoadObjectInfo objInfo);

	void EnemyGenerator(ObjectType objType, LoadObjectInfo objInfo);
	//置物生成機
	void OrnamentGenerator(ObjectType objType, LoadObjectInfo objInfo);
	//ギミック生成機
	void GimmickObjectGenerator(ObjectType objType, LoadObjectInfo objInfo);
private:

	int playerHandle_ = -1;
	int switchHandle_ = -1;
	int steelyardHandle_ = -1;
	int transObjHandle_ = -1;
	int elevatorHandle_ = -1;

	std::list<std::shared_ptr<Model>> checkCollList_;

	std::unordered_map<ObjectType,int> modelHandle_;

	std::unordered_map<ObjectType, std::list<std::shared_ptr<ObjectBase>>> objects_;			//オブジェクトをobjectTypeをキーにlistでもつ
};

