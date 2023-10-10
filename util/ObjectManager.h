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

	void ObjectGenerator(ObjectBaseType baseType,ObjectType objType,const char* const filename);
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
	void CharacterGenerator(ObjectType objType, const char* const filename);
	//敵生成機
	void EnemyGenerator(ObjectType objType, const char* const filename);
	//置物生成機
	void OrnamentGenerator(ObjectType objType, const char* const filename);
	//運べる置物生成機
	void CarryObjectGenerator(ObjectType objType, const char* const filename);
	//ギミック生成機
	void GimmickObjectGenerator(ObjectType objType, const char* const filename);
private:

	std::unordered_map<ObjectType, std::list<std::shared_ptr<ObjectBase>>> objects_;
};

