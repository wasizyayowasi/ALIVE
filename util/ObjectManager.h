#pragma once
#include "ObjectData.h"
#include "DxLib.h"
#include <memory>
#include <list>
#include <unordered_map>

class ObjectBase;
class Model;
class Player;

class ObjectManager
{
public:

	virtual ~ObjectManager();

	static ObjectManager& GetInstance() {
		static ObjectManager instance;
		return instance;
	}

	void ObjectGenerator(ObjectBaseType baseType,ObjectType objType,LoadObjectInfo objInfo);
	void DeadPersonGenerator(ObjectType objType,int handle, LoadObjectInfo objInfo,int animNo);

	void Update(Player& player);
	void Draw();

	std::list<std::shared_ptr<Model>> GetAllCheckCollModel();
	std::list<std::shared_ptr<Model>> GetSpecificModel(ObjectType type);

private:

	ObjectManager();

	ObjectManager(const ObjectManager&) = delete;
	void operator = (const ObjectManager) = delete;

private:
	//キャラクター生成機
	void CharacterGenerator(ObjectType objType, LoadObjectInfo objInfo);
	//敵生成機
	void EnemyGenerator(ObjectType objType, LoadObjectInfo objInfo);
	//置物生成機
	void OrnamentGenerator(ObjectType objType, LoadObjectInfo objInfo);
	//運べる置物生成機
	void CarryObjectGenerator(ObjectType objType, LoadObjectInfo objInfo);
	//ギミック生成機
	void GimmickObjectGenerator(ObjectType objType, LoadObjectInfo objInfo);
private:

	int playerHandle_ = -1;
	int fieldHandle_ = -1;
	int boxHandle_ = -1;
	int switchHandle_ = -1;
	int steelyardHandle_ = -1;

	int tempSofaHandle_ = -1;
	int tempBedHandle_ = -1;

	std::unordered_map<ObjectType, std::list<std::shared_ptr<ObjectBase>>> objects_;
};

