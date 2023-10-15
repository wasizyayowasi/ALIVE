#pragma once
#include "ObjectType.h"
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

	void ObjectGenerator(ObjectBaseType baseType,ObjectType objType);
	void DeadPersonGenerator(ObjectType objType,int handle,VECTOR pos,VECTOR rot,int animNo);

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
	void CharacterGenerator(ObjectType objType);
	//敵生成機
	void EnemyGenerator(ObjectType objType);
	//置物生成機
	void OrnamentGenerator(ObjectType objType);
	//運べる置物生成機
	void CarryObjectGenerator(ObjectType objType);
	//ギミック生成機
	void GimmickObjectGenerator(ObjectType objType);
private:

	int playerHandle_ = -1;
	int fieldHandle_ = -1;
	int boxHandle_ = -1;
	int switchHandle_ = -1;
	int steelyardHandle_ = -1;

	std::unordered_map<ObjectType, std::list<std::shared_ptr<ObjectBase>>> objects_;
};

